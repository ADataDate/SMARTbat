# Adapted from examples in Google Cloud documentation.

import html
import os
import random
import time

from google.cloud import texttospeech
import pygame


client = texttospeech.TextToSpeechClient()
pygame.mixer.init()


LANGUAGE = 'en-US'
VOICE = 'en-US-Wavenet-A'
SPEAKING_RATE = 1.25
PITCH = 6.0
INTRO_PAUSE = 8.0


def text_to_speech(text,
                   output_filename='output.mp3',
                   speaking_rate=SPEAKING_RATE,
                   pitch=PITCH):
    try:
        ssml = '<speak>{}</speak>'.format(
            html.escape(text).replace('\pause', '<break time="0.5s"/>'))
        synthesis_input = texttospeech.types.SynthesisInput(ssml=ssml)
        # Specify language and voice model.
        voice = texttospeech.types.VoiceSelectionParams(
            language_code=LANGUAGE,
            name=VOICE)
        # Specify output audio format. Options are LINEAR16, MP3, and OGG_OPUS.
        # Also specify speech speed and pitch.
        audio_config = texttospeech.types.AudioConfig(
            audio_encoding=texttospeech.enums.AudioEncoding.MP3,
            speaking_rate=speaking_rate,
            pitch=pitch)
        # Query the text-to-speech API.
        response = client.synthesize_speech(synthesis_input, voice, audio_config)
        # Write the binary response to file.
        with open(output_filename, 'wb') as file:
            file.write(response.audio_content)
        print('Text to speech query successful.')
        print('Text:', text)
        pygame.mixer.music.load(output_filename)
        pygame.mixer.music.play()

    except:
        print('Error encountered during text to speech query.')


def self_introduction():
    time.sleep(1.0)
    text_to_speech("Hello! I'm Austin the SMARTbat. I am your personal Shoulder Mounted Anxiety Relieving Therapy Bat. \pause Let's start the day with a joke! \pause")
    time.sleep(INTRO_PAUSE)
    tell_a_joke()

def tell_a_joke():
    jokes=["What's the best thing about Switzerland? \pause\pause I don't know, but the flag is a big plus.",
           "Why was the bicycle sleeping? \pause\pause It was two tired.",
           "Why doesn't Kandai like my jokes? \pause\pause Because he thinks I tell bat jokes."]
    text_to_speech(random.choice(jokes))

def no_one_detected():
    text_to_speech("Sorry, I don't see anyone.")

def name_people(people_list):
    num = len(people_list)
    if num == 0:
        text_to_speech("Sorry, I don't see anyone.")
    elif num == 1:
        text_to_speech("That's " + people_list[0] + "!")
    elif num == 2:
        text_to_speech("That's " + people_list[0] + " and " + people_list[1] + "!")
    else:
        text = "I see"
        for i, person in enumerate(people_list):
            if i < num - 2:
                text = text + " " + person + ","
            if i == num - 2:
                text = text + " " + person + ", and " + people_list[-1] + "."
        text_to_speech(text)

def state_emotion(person, emotion):
    text_to_speech("Right now, " + person + " seems to be " + emotion + ".")

def announce_calibration():
    text_to_speech("Calibration, starting now!")

def did_not_understand():
    text_to_speech("My apologies, I did not quite understand what you just said.")

def announce_shutdown():
    text_to_speech("Okay, nighty nighty! Don't let the bed bats bite!")
    time.sleep(5.0)
