import json
import numpy as np
from client import MQTTClient
from time import sleep
from datetime import datetime


class EmotionPublisher(MQTTClient):
    def __init__(self, client_url_env, topic_name):
        super().__init__(client_url_env)
        self.topic_name = topic_name

        self.EMOTIONS = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
        self.names = ['kandai', 'cj', 'alex', 'joewie', 'mary', 'caleb']

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("connected OK")
        else:
            print("Bad connection return code = ", rc)

    def on_publish(self, client, userdata, result):
        # pass
        print('Published: {}, {}'.format(userdata, result))

    def loop(self):
        self.client.loop_start()

        while True:
            n = len(self.EMOTIONS)
            nums = np.random.rand(n)
            emotion_percentages = nums/np.sum(nums)

            vals = {
                'time': str(datetime.now()),
                'name': np.random.choice(self.names),
                self.EMOTIONS[0]: str(emotion_percentages[0]),
                self.EMOTIONS[1]: str(emotion_percentages[1]),
                self.EMOTIONS[2]: str(emotion_percentages[2]),
                self.EMOTIONS[3]: str(emotion_percentages[3]),
                self.EMOTIONS[4]: str(emotion_percentages[4]),
                self.EMOTIONS[5]: str(emotion_percentages[5])
            }
            vals_json = json.dumps(vals)

            self.client.publish(self.topic_name, vals_json)
            sleep(5)


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin/eye/motion"

    publisher = EmotionPublisher(
        client_url_env,
        topic_name
    )
    publisher.loop()
