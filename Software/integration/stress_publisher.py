import json
import numpy as np
from client import MQTTClient
from time import sleep
from datetime import datetime


class StressPublisher(MQTTClient):
    def __init__(self, client_url_env, topic_name):
        super().__init__(client_url_env)
        self.topic_name = topic_name
        self.STRESS = ['HIGH', 'LOW']

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
            n = len(self.STRESS)
            nums = np.random.rand(n)
            self.client.publish(self.topic_name, self.STRESS[nums])
            sleep(5)


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin_gestures/body/gesture"

    publisher = StressPublisher(
        client_url_env,
        topic_name
    )
    publisher.loop()
