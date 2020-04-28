import numpy as np
from subscriber import Subscriber


class GestureSubscriber(Subscriber):
    def __init__(self, topic_name, client_url_env, serial_port):
        super().__init__(topic_name, client_url_env, serial_port)
        self.commands = {'CON': 7, 'NO': 8}

    def process(self, msg):
        """
        Arguments
        ----------
        msg: str
        """
        return str(self.commands[msg])



if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    topic_name = "austin_gestures/body/gesture"
    serial_port = '/dev/ttyACM0'

    subscriber = GestureSubscriber(
        topic_name,
        client_url_env,
        serial_port,
    )
    subscriber.loop()
