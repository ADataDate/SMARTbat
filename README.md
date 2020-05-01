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

Documentation
-------------
To publish messages via MQTT, make sure you have an environment variable **CLOUDMQTT_URL:**
```bash
export CLOUDMQTT_URL=<url of your mqtt broker>
```

### Austin

### Facial and Emotion Recognition
See [Facial and Emotion Recognition Documentation](https://github.com/Sayter99/face_recognition_ultra_light/blob/master/README.md)

### Voice Interface

### Stress Classifier

Demonstration
-------------
* **Video to come**


License Information
-------------------

This product is _**open source**_! 

Distributed as-is; no warranty is given. This is not a medical device and should not be used for the detection or prevention of disease, illness or otherwise. 
