import json
import numpy as np
from client import MQTTClient
from trigger import ArduinoTrigger
import time


class Subscriber(MQTTClient):
    def __init__(self, topic_name, client_url_env, serial_port):
        super().__init__(client_url_env)
        self.topic_name = topic_name
        self.client.subscribe(self.topic_name, qos=1)
        self.arduino_trigger = ArduinoTrigger(serial_port)

    def process(self, msg):
        return msg

    def on_subscribe(self, client, userdata, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos))

    # process a message when recived
    def on_message(self, client, userdata, msg):
        msg_decode = str(msg.payload.decode("utf-8"))
        msg_processed = self.process(msg_decode)
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
    topic_name = "austin/eye/motion"
    serial_port = '/dev/ttyACM0'

    subscriber = Subscriber(
        topic_name,
        client_url_env,
        serial_port,
    )
    subscriber.loop()
