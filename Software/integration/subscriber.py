import json
import numpy as np
from client import MQTTClient
from trigger import ArduinoTrigger
import time
from callback import *


class Subscriber(MQTTClient):
    def __init__(self, client_url_env, serial_port, subscriber_callbacks):
        super().__init__(client_url_env)
        self.topic_names = list(subscriber_callbacks.keys())
        for topic_name in self.topic_names:
            self.client.subscribe(topic_name, qos=1)
        self.arduino_trigger = ArduinoTrigger(serial_port)
        self.subscriber_callbacks = subscriber_callbacks

    def process(self, msg):
        return msg

    def on_subscribe(self, client, userdata, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos))

    # process a message when recived
    def on_message(self, client, userdata, msg):
        msg_decode = str(msg.payload.decode("utf-8"))
        msg_processed = self.subscriber_callbacks[msg.topic](msg_decode)
        self.arduino_trigger.publish(msg_processed)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("connected OK")
        else:
            print("Bad connection return code = ", rc)

    def loop(self):
        self.client.loop_forever()


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    serial_port = '/dev/tty.usbmodem143401'
    subscriber_callbacks = {
        "austin/eye/motion": eye_callback,
        "austin_gestures/body/gesture": gesture_callback,
        "austin_anxiety/belt/therapy": stress_callback
    }

    subscriber = Subscriber(
        client_url_env,
        serial_port,
        subscriber_callbacks
    )
    subscriber.loop()
