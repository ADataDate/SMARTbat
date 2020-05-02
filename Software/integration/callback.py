import json
import numpy as np
from constant import EMOTIONS, COMMANDS


def calibrate_callback(msg):
    print('Start Calibration ...')
    return str(COMMANDS[msg])

def eye_callback(msg):
    print('Emotion: ', msg)
    return str(COMMANDS[msg])

    # detected_person, detected_emotion = msg.split(' ')
    # print(detected_person, detected_emotion)
    # return str(COMMANDS[detected_emotion])

def gesture_callback(msg):
    print('Gesture: ', msg)
    return str(COMMANDS[msg])

def stress_callback(msg):
    print('Stress: ', msg)
    return str(COMMANDS[msg])
