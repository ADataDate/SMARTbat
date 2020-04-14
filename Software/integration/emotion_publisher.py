import json
import numpy as np
from client import MQTTClient
from time import sleep


class EmotionPublisher(MQTTClient):
    def __init__(self, client_url_env, eye_topic_name):
        super().__init__(client_url_env)
        self.eye_topic_name = eye_topic_name

        self.EMOTIONS = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
        self.emotion_percentages = np.arange(len(self.EMOTIONS))
        self.emotion_percentages = self.emotion_percentages/np.sum(self.emotion_percentages)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("connected OK")
        else:
            print("Bad connection return code = ", rc)

    def on_publish(self, client, userdata, result):
        # pass
        print('Published: {}, {}'.format(userdata, result))

    def loop(self):
        vals = {
            'time': str(10),
            'name': str('kandai'),
            self.EMOTIONS[0]: str(self.emotion_percentages[0]),
            self.EMOTIONS[1]: str(self.emotion_percentages[1]),
            self.EMOTIONS[2]: str(self.emotion_percentages[2]),
            self.EMOTIONS[3]: str(self.emotion_percentages[3]),
            self.EMOTIONS[4]: str(self.emotion_percentages[4]),
            self.EMOTIONS[5]: str(self.emotion_percentages[5])
        }
        vals_json = json.dumps(vals)

        self.client.loop_start()

        while True:
            self.client.publish(self.eye_topic_name, vals_json)
            sleep(1)


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    eye_topic_name = "austin/eye/motion"

    publisher = EmotionPublisher(
        client_url_env,
        eye_topic_name
    )
    publisher.loop()
