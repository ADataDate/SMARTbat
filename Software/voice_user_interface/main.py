# Adapted from examples in Google Cloud documentation.

# Dependencies.
# pip install --upgrade google-cloud-texttospeech
# pip install --upgrade google-cloud-speech
# pip install --upgrade paho-mqtt
# pip install --upgrade pyaudio
# pip install --upgrade pygame

# Required environment variables.
# GOOGLE_APPLICATION_CREDENTIALS
# CLOUDMQTT_URL

import os
import re
import six
import sys
import time
import urllib.parse as urlparse

import pyaudio
import paho.mqtt.client as paho
from google.cloud import speech

from text_to_speech import *


# Global variables.
shutdown = False
detection_time = -10.0
detected_person = 'no one'
detected_emotion = 'having an identity crisis'

# Set to True to test detection.
TEST_MODE = False

# Specify detection time threshold.
DETECTION_THRESHOLD = 1.0
SLEEP_DURATION = 1.0

# Specity audio recording parameters.
RATE = 16000
CHUNK = int(RATE / 10)  # 100 ms


############################## MQTT ##############################
def on_connect(client, userdata, flags, rc):
    print("CONNACK received with code %d." % (rc))

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

    if msg.topic == 'austin/eye/emotion':
        global detection_time
        global detected_person
        global detected_emotion
        
        message = str(msg.payload)
        detection_time = time.time()
        detected_person, detected_emotion = message.split(' ')
        detected_person = detected_person[2:] # Super hacky, don't judge.

def on_publish(client, userdata, mid):
    print("mid: " + str(mid))

url_str = os.environ.get('CLOUDMQTT_URL')
url = urlparse.urlparse(url_str)

client = paho.Client('AustinTalk')
client.on_connect = on_connect
client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_publish = on_publish
client.username_pw_set(url.username, url.password)
client.connect(url.hostname, url.port)

client.subscribe('austin/eye/emotion', qos=1)

client.loop_start()
############################## MQTT ##############################


class MicrophoneStream(object):
    """Opens a recording stream as a generator yielding the audio chunks."""
    def __init__(self, rate, chunk):
        self._rate = rate
        self._chunk = chunk

        # Create a thread-safe buffer of audio data.
        self._buff = six.moves.queue.Queue()
        self.closed = True

    def __enter__(self):
        self._audio_interface = pyaudio.PyAudio()
        self._audio_stream = self._audio_interface.open(
            format=pyaudio.paInt16,
            channels=1,
            rate=self._rate,
            input=True,
            frames_per_buffer=self._chunk,
            # Run the audio stream asynchronously to fill the buffer object.
            # This is necessary so that the input device's buffer doesn't
            # overflow while the calling thread makes network requests, etc.
            stream_callback=self._fill_buffer,
        )

        self.closed = False
        return self

    def __exit__(self, type, value, traceback):
        self._audio_stream.stop_stream()
        self._audio_stream.close()
        self.closed = True
        # Signal the generator to terminate so that the client's
        # streaming_recognize method will not block the process termination.
        self._buff.put(None)
        self._audio_interface.terminate()

    def _fill_buffer(self, in_data, frame_count, time_info, status_flags):
        """Continuously collect data from the audio stream, into the buffer."""
        self._buff.put(in_data)
        return None, pyaudio.paContinue

    def generator(self):
        while not self.closed:
            # Use a blocking get() to ensure there's at least one chunk of
            # data, and stop iteration if the chunk is None, indicating the
            # end of the audio stream.
            chunk = self._buff.get()
            if chunk is None:
                return
            data = [chunk]

            # Now consume whatever other data's still buffered.
            while True:
                try:
                    chunk = self._buff.get(block=False)
                    if chunk is None:
                        return
                    data.append(chunk)
                except six.moves.queue.Empty:
                    break

            yield b''.join(data)


def listen(responses):
    for response in responses:
        if not response.results:
            continue
        
        # The `results` list is consecutive. For streaming, we only care about
        # the first result being considered, since once it's `is_final`, it
        # moves on to considering the next utterance.
        result = response.results[0]
        if not result.alternatives:
            continue

        # Display the transcription of the top alternative.
        transcript = result.alternatives[0].transcript

        if result.is_final and 'Austin' in transcript:
            print('Transcript:', transcript)

            trigger = ''
            for trigger_word in ['joke', 'who', 'emotion', 'calibration', 'shutdown', 'shut down']:
                if trigger_word in transcript:
                    print('Detected:', trigger_word)
                    trigger = trigger_word

            if trigger == 'joke':
                client.publish('austin_gestures/body/gesture', 'YES', qos=1)
                tell_a_joke()
                print('Done: Told a joke')
            
            elif trigger == 'who':
                if (time.time() - detection_time < DETECTION_THRESHOLD) or TEST_MODE:
                    client.publish('austin_gestures/body/gesture', 'YES', qos=1)
                    name_people([detected_person])
                    print('Done: Identified person')
                else:
                    client.publish('austin_gestures/body/gesture', 'NO', qos=1)
                    no_one_detected()
                    print('Fail: No detected person in scene')
            
            elif trigger == 'emotion':
                if (time.time() - detection_time < DETECTION_THRESHOLD) or TEST_MODE:
                    client.publish('austin_gestures/body/gesture', 'YES', qos=1)
                    state_emotion(detected_person, detected_emotion)
                    print('Done: Identified emotion')
                else:
                    client.publish('austin_gestures/body/gesture', 'NO', qos=1)
                    no_one_detected()
                    print('Fail: No detected person in scene')
            
            elif trigger == 'calibration':
                client.publish('austin_gestures/body/gesture', 'YES', qos=1)
                announce_calibration()
                client.publish('austin/calibration', 'CAL', qos=1)
                print('Done: Initiated calibration')
            
            elif trigger == 'shutdown' or trigger == 'shut down':
                announce_shutdown()
                global shutdown
                shutdown = True
                quit()
            
            else:
                client.publish('austin_gestures/body/gesture', 'CON', qos=1)
                did_not_understand()
            
            time.sleep(SLEEP_DURATION)


def main():
    self_introduction()

    language_code = 'en-US'
    speech_context = speech.types.SpeechContext(phrases=['Austin',
                                                         'joke',
                                                         'tell me a joke'
                                                         'who',
                                                         'who is that',
                                                         'read',
                                                         'emotion',
                                                         'read emotion',
                                                         'calibration',
                                                         'shutdown',
                                                         'shut down'])

    client = speech.SpeechClient()
    config = speech.types.RecognitionConfig(
        encoding=speech.enums.RecognitionConfig.AudioEncoding.LINEAR16,
        sample_rate_hertz=RATE,
        language_code=language_code,
        speech_contexts=[speech_context])
    streaming_config = speech.types.StreamingRecognitionConfig(
        config=config,
        interim_results=True)

    while True:
        if shutdown:
            break
        
        try:
            with MicrophoneStream(RATE, CHUNK) as stream:
                audio_generator = stream.generator()
                requests = (speech.types.StreamingRecognizeRequest(audio_content=content)
                            for content in audio_generator)
                responses = client.streaming_recognize(streaming_config, requests)
                listen(responses)

        except:
            pass


if __name__ == '__main__':
    main()
