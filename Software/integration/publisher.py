import numpy as np
from client import MQTTClient
from time import sleep
from dummymsg import *


class Publisher(MQTTClient):
    def __init__(self, client_url_env, dummymsg_funcs):
        super().__init__(client_url_env)
        self.topic_names = list(dummymsg_funcs.keys())
        self.dummymsg_funcs = dummymsg_funcs

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
            n = len(self.topic_names)
            i = np.random.choice(n)
            topic_name = self.topic_names[i]

            self.client.publish(
                topic_name,
                self.dummymsg_funcs[topic_name]())
            sleep(5)


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    dummymsg_funcs = {
        "austin/eye/motion": eye_msg,
        "austin_gestures/body/gesture": gesture_msg,
        "austin_anxiety/belt/therapy": stress_msg
    }

    publisher = Publisher(
        client_url_env,
        dummymsg_funcs
    )
    publisher.loop()
