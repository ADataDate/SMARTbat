#!/usr/bin/env python3
import serial
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import json
export CLOUDMQTT_URL='mqtt://rwjanqog:0wo-qFBN9zOs@soldier.cloudmqtt.com:16801'

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

# process a message when recived
def on_message(client, userdata, msg):
    m_decode = str(msg.payload.decode("utf-8"))
    emotion_dict = json.loads(m_decode)
    for key in emotion_dict:
        print(key, '->', emotion_dict[key])
    print('')


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK")
    else:
        print("Bad connection return code = ", rc)

def on_publish(client, userdata, result):
	print("data published \n")
	pass

# Note: client name must be unique across devices 
client_name = "batbot1"
broker = "mqtt://rwjanqog:0wo-qFBN9zOs@soldier.cloudmqtt.com:16801"
client = mqtt.Client(client_name)


# Add all methods defined at the start of the file to the client
client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_connect = on_connect
client.on_publish = on_publish 

client.connect(broker)
client.subscribe("austin/eye/emotion", qos=1)


# Read sensor data from Arduino serial 
ser = serial.Serial('/dev/ttyUSB0', 9600)
print(ser.name)

#Read instruction line: "Attach Finger to sensor" but do not send
print(ser.readline())

while 1:
    line = ser.readline()
    line = line.decode("utf-8") #convert line to string
    print(line)	
    #Publish data after single read
    publish.single("bio_sesnors/sensors/sensor_data", "line", hostname="mqtt://rwjanqog:0wo-qFBN9zOs@soldier.cloudmqtt.com:16801")	

ser.close() 

''' To Do:'''
 

client.loop_forever()
