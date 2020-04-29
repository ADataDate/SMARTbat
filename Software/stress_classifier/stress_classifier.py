import paho.mqtt.client as paho

# Set to True to test detection.
TEST_MODE = False


def on_connect(client, userdata, flags, rc):
    print("CONNACK received with code %d." % (rc))

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

    if msg.topic == 'bio_sensors/sensors/sensor_data':
        time, hr, o2, gsr = msg.payload.split(',')
        if hr > 180 or o2 > 105 or gsr > 600 or TEST_MODE: # Hard-coded for now.
            client.publish('austin_anxiety/belt/therapy', 'HIGH', qos=1)

def on_publish(client, userdata, mid):
    print("mid: " + str(mid))

url_str = os.environ.get('CLOUDMQTT_URL')
url = urlparse.urlparse(url_str)

client = paho.Client('AustinStress')
client.on_connect = on_connect
client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_publish = on_publish
client.username_pw_set(url.username, url.password)
client.connect(url.hostname, url.port)

client.subscribe('bio_sensors/sensors/sensor_data', qos=1)

client.loop_start()
