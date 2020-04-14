import os
import urllib.parse as urlparse
import paho.mqtt.client as mqtt


class MQTTClient():
    def __init__(self, client_url_env="CLOUDMQTT_URL"):
        self.client = mqtt.Client()
        try:
            url_str = os.environ.get(client_url_env)
            print("If the end of this line doesn't have the URL run command to set environment variable: {}".format(url_str))
        except:
            raise ValueError('Please set a valid Environmental Variable for the client URL')
        self.url_str = url_str
        url = urlparse.urlparse(self.url_str)
        # Add all methods defined at the start of the file to the client
        self.client.on_subscribe = self.on_subscribe
        self.client.on_message = self.on_message
        self.client.on_connect = self.on_connect
        self.client.on_publish = self.on_publish
        self.client.username_pw_set(url.username, url.password)
        self.client.connect(url.hostname, url.port)

    def on_subscribe(self, client, userdata, mid, granted_qos):
        raise NotImplementedError()

    def on_message(self, client, userdata, msg):
        raise NotImplementedError()

    def on_connect(self, client, flags, rc):
        raise NotImplementedError()

    def on_publish(self, client, userdata, result):
        raise NotImplementedError()
