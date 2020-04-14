from emotion_subscriber import EmotionSubscriber
from gesture_subscriber import GestureSubscriber
from stress_subscriber import StressSubscriber


if __name__ == "__main__":
    client_url_env = "CLOUDMQTT_URL"
    eye_topic_name = "austin/eye/motion"
    serial_port = '/dev/ttyACM0'

    subscriber = GestureSubscriber(
        client_url_env,
        eye_topic_name,
        serial_port
    )

    subscriber.loop()

    subscriber = EmotionSubscriber(
        client_url_env,
        eye_topic_name,
        serial_port
    )

    subscriber.loop()

    subscriber = StressSubscriber(
        client_url_env,
        eye_topic_name,
        serial_port
    )

    subscriber.loop()
