import numpy as np
from subscriber import Subscriber


class StressSubscriber(Subscriber):
    def __init__(self, topic_name, client_url_env, serial_port):
        super().__init__(topic_name, client_url_env, serial_port)

    def process(self, msg):
        raise NotImplementedError()


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin_anxiety/belt/therapy"
    serial_port = '/dev/ttyACM0'

    subscriber = StressSubscriber(
        topic_name,
        client_url_env,
        serial_port,
    )
    subscriber.loop()
