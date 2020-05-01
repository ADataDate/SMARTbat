Meet Austin: the SMARTbat

Shoulder Mounted Anxiety Relieving Therapy Bat
==================================================================

Austin, the SMARTBat, is a shoulder mounted robot that monitors the wearer's **heart rate**, **oxygen saturation**, **respiration**, and **GSR** to build a live approximate model of the level of stress experienced by the wearer and autonomoulsy administers deep touch pressure therapy through a pnuematic vest when stress levels are out of normal range. Austin also monitors the social environment with verbal feedback on the "emotion" of the room using computer vision and tone analysis and names the faces it recognizes when queried. Communicating with Austin is possible thanks to Google cloud services:

- Speech to text
- Text to speech

Austin is wrapped up in an adorably-cuddleable brown bat plushie that magnetically attaches to the wearer's shoulder and is programmed with physical(gestures) and emotional(RGB LED eyes) responses.  

Repository Contents
-------------------

* **/Documents** - Images, reports, presentations, papers, etc
* **/Software** - Arduino sketches, Python scripts, and Log files 
* **/Hardware** - Eagle files
* **/sensors_test** - UART Communication tests between devices  

Dependencies & Installation
--------------

### Common Requirements

All components require the `CLOUDMQTT_URL` environment variable to be correctly set. The required `paho-mqtt` Python package can be installed as follows:
```
pip install --upgrade paho-mqtt
```

### Voice User Interface (VUI)

The VUI requires a Google Cloud service account with access to the Cloud Speech-to-Text API and Cloud Text-to-Speech API. The location of the key file for the service account should be specified with the `GOOGLE_APPLICATION_CREDENTIALS` environment variable.

The required Python packages specific to the VUI can be installed as follows:
```
pip install --upgrade google-cloud-texttospeech
pip install --upgrade google-cloud-speech
pip install --upgrade pyaudio
pip install --upgrade pygame
```

### Stress Classifier

The stress classifier has no additional dependencies.

Running the Software
--------------

### Voice User Interface (VUI)
```
cd ./Software/voice_user_interface
python main.py
```

### Stress Classifier
```
cd ./Software/stress_classifier
python stress_classifier.py
```


Documentation
--------------
* **Video to come**


License Information
-------------------

This product is _**open source**_! 

Distributed as-is; no warranty is given. This is not a medical device and should not be used for the detection or prevention of disease, illness or otherwise. 
