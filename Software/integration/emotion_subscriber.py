import numpy as np
from subscriber import Subscriber


class EmotionSubscriber(Subscriber):
    def __init__(self, topic_name, client_url_env, serial_port):
        super().__init__(topic_name, client_url_env, serial_port)
        self.emotions = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
        self.leds = {'angry': 'red',
                     'scared': 'red',
                     'happy': 'yellow',
                     'sad': 'blue',
                     'surprised': 'purple',
                     'neutral': 'white'}

    def process(self, emotion_dict):
        # Extract Emotion Values Only
        values = np.array([float(emotion_dict[k]) for k in self.emotions])
        # take max emotion
        idx = np.argmax(values)
        emotion = self.emotions[idx]
        return 'LED: ' + self.leds[emotion]


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin/eye/motion"
    serial_port = '/dev/ttyACM0'

    subscriber = EmotionSubscriber(
        topic_name,
        client_url_env,
        serial_port,
    )
    subscriber.loop()
