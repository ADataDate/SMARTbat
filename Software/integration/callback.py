import json
import numpy as np


emotions = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
commands = {'happy': 1,
            'sad': 2,
            'angry': 3,
            'surprised': 4,
            'scared': 5,
            'neutral': 6,
            'CON': 7,
            'NO': 8,
            'HIGH': 9}


def eye_callback(emotion_dict):
    # Extract Emotion Values Only
    emotion_dict = json.loads(emotion_dict)
    values = np.array([float(emotion_dict[k]) for k in emotions])
    # take max emotion
    idx = np.argmax(values)
    emotion = emotions[idx]
    print(emotion_dict['name'], emotion)
    return str(commands[emotion])

def gesture_callback(msg):
    print('Gesture: ', msg)
    return str(commands[msg])

def stress_callback(msg):
    print('Stress: ', msg)
    return str(commands[msg])
