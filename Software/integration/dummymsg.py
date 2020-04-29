from datetime import datetime
import json
import numpy as np


def eye_msg():
    emotions = ['angry', 'scared', 'happy', 'sad', 'surprised', 'neutral']
    names = ['kandai', 'cj', 'alex', 'joewie', 'mary', 'caleb']

    n = len(emotions)
    nums = np.random.rand(n)
    emotion_percentages = nums/np.sum(nums)

    vals = {
        'time': str(datetime.now()),
        'name': np.random.choice(names),
        emotions[0]: str(emotion_percentages[0]),
        emotions[1]: str(emotion_percentages[1]),
        emotions[2]: str(emotion_percentages[2]),
        emotions[3]: str(emotion_percentages[3]),
        emotions[4]: str(emotion_percentages[4]),
        emotions[5]: str(emotion_percentages[5])
    }
    vals_json = json.dumps(vals)

    return vals_json

def gesture_msg():
    gestures = ['CON', 'NO']
    return np.random.choice(gestures)

def stress_msg():
    stress = ['HIGH', 'LOW']
    return np.random.choice(stress)


