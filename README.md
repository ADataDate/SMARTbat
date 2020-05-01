Meet Austin: the SMARTbat

Shoulder Mounted Anxiety Relieving Therapy Bat
==================================================================

Austin, the SMARTBat, is a shoulder mounted robot that monitors the wearer's **heart rate**, **oxygen saturation**, **respiration**, and **GSR** to build a live approximate model of the level of stress experienced by the wearer and autonomoulsy administers deep touch pressure therapy through a pnuematic vest when stress levels are out of normal range. Austin also monitors the social environment with verbal feedback on the "emotion" of the room using computer vision and tone analysis and names the faces it recognizes when queried. Communicating with Austin is possible thanks to the following Google Cloud services:

- [Speech-to-Text](https://cloud.google.com/speech-to-text)
- [Text-to-Speech](https://cloud.google.com/text-to-speech)

Austin is wrapped up in an adorably-cuddleable brown bat plushie that magnetically attaches to the wearer's shoulder and is programmed with physical(gestures) and emotional(RGB LED eyes) responses.  

Repository Contents
-------------------

* **/Documents** - Images, reports, presentations, papers, etc
* **/Software** - Arduino sketches, Python scripts, and Log files 
* **/Hardware** - Eagle files
* **/sensors_test** - UART Communication tests between devices  

Documentation
-------------
To publish and subscribe to messages via MQTT, make sure you have an environment variable **CLOUDMQTT_URL:**
```bash
export CLOUDMQTT_URL=<url of your mqtt broker>
```
Python scripts also require the Eclipse Paho MQTT Python client library to be installed:
```bash
pip install --upgrade paho-mqtt
```

### Austin

### Facial and Emotion Recognition
See [Facial and Emotion Recognition Documentation](https://github.com/Sayter99/face_recognition_ultra_light/blob/master/README.md).

### Voice User Interface (VUI)

The VUI requires a Google Cloud service account with access to the Cloud Speech-to-Text API and Cloud Text-to-Speech API. The location of the key file for the service account should be specified with the `GOOGLE_APPLICATION_CREDENTIALS` environment variable.

The VUI works with Python 3.7.1, and the required packages specific to the VUI can be installed as follows:
```bash
pip install --upgrade google-cloud-texttospeech
pip install --upgrade google-cloud-speech
pip install --upgrade pyaudio
pip install --upgrade pygame
```

To start the VUI, run the following:
```bash
cd ./Software/voice_user_interface
python main.py
```

### Stress Classifier

The stress classifier works with Python 3.7.1, and has no additional dependencies.

To start the stress classifier, run the following:
```bash
cd ./Software/stress_classifier
python stress_classifier.py
```
### Wearable Sensors

The arduino sketch **_Sensors_** serially outputs the heart rate, blood oxygen saturation, respiration and GSR data values which is read 
the script **_mqtt_on_body_sensors_pub_emotions_sub_**. This script then publishes the sensor data to a broker via MQTT. 

Demonstration
-------------
* **Video to come**


License Information
-------------------

This product is _**open source**_! 

Distributed as-is; no warranty is given. This is not a medical device and should not be used for the detection or prevention of disease, illness or otherwise. 
