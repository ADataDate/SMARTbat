import json
import numpy as np
from subscriber import Subscriber


class EmotionSubscriber(Subscriber):
    def __init__(self, topic_name, client_url_env, serial_port):
        super().__init__(topic_name, client_url_env, serial_port)
        self.emotions = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
        self.commands = {'happy': 1,
                         'sad': 2,
                         'angry': 3,
                         'surprised': 4,
                         'scared': 5,
                         'neutral': 6}

    def process(self, emotion_dict):
        # Extract Emotion Values Only
        emotion_dict = json.loads(emotion_dict)
        values = np.array([float(emotion_dict[k]) for k in self.emotions])
        # take max emotion
        idx = np.argmax(values)
        emotion = self.emotions[idx]
        print(emotion_dict['name'], emotion)
        return str(self.commands[emotion])


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin/eye/motion"
    serial_port = '/dev/tty.usbmodem141401'

    subscriber = EmotionSubscriber(
        topic_name,
        client_url_env,
        serial_port,
    )
    subscriber.loop()
