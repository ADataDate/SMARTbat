from datetime import datetime
import json
import numpy as np
from constant import EMOTIONS, GESTURES, STRESS, NAMES


def eye_msg():
    n = len(EMOTIONS)
    nums = np.random.rand(n)
    emotion_percentages = nums/np.sum(nums)

    vals = {
        'time': str(datetime.now()),
        'name': np.random.choice(NAMES),
        EMOTIONS[0]: str(emotion_percentages[0]),
        EMOTIONS[1]: str(emotion_percentages[1]),
        EMOTIONS[2]: str(emotion_percentages[2]),
        EMOTIONS[3]: str(emotion_percentages[3]),
        EMOTIONS[4]: str(emotion_percentages[4]),
        EMOTIONS[5]: str(emotion_percentages[5])
    }
    vals_json = json.dumps(vals)

    return vals_json

def gesture_msg():
    return np.random.choice(GESTURES)

def stress_msg():
    return np.random.choice(STRESS)
