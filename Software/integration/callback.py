import json
import numpy as np
from constant import EMOTIONS, COMMANDS


def calibrate_callback(msg):
    print('Start Calibration ...')
    return str(COMMANDS[msg])

def eye_callback(emotion_dict):
    # Extract Emotion Values Only
    emotion_dict = json.loads(emotion_dict)
    values = np.array([float(emotion_dict[k]) for k in EMOTIONS])
    # take max emotion
    idx = np.argmax(values)
    emotion = EMOTIONS[idx]
    print(emotion_dict['name'], emotion)
    return str(COMMANDS[emotion])

def gesture_callback(msg):
    print('Gesture: ', msg)
    return str(COMMANDS[msg])

def stress_callback(msg):
    print('Stress: ', msg)
    return str(COMMANDS[msg])
