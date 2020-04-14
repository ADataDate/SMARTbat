
#include <SPI.h>
#include <heartRate.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>
#include <SparkFunDS3234RTC.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
MAX30105 hro2;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
float respiration;
int contrlVar;

int GSR = A0;
int RSP = A1;
//int PRS = A2;

#define MAX_BRIGHTNESS 255
#define PRINT_USA_DATE
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

Adafruit_NeoPixel strip1(2, 9, NEO_RGB);//setup LED strip, (#LEDs, port, NEO_RGB)
Servo twist; //declare servo called twist
Servo tilt; //declare serco called tilt

struct emotion { //struct containg RGB values for each emotion
  int red;
  int green;
  int blue;
};

emotion angry {168, 64, 50};
emotion scared {255, 75, 3};
emotion happy {255, 200, 0};
emotion sad {41, 75, 242};
emotion surprised {150, 0, 255};
emotion neutral {255, 255, 255};
emotion standard {0, 255, 20};

int posTwist = 0; //Head lateral position
int posTilt = 0;//Head vertical position
int n = 0;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  rtc.begin(DS13074_CS_PIN);
  rtc.set12Hour(); // Use rtc.set12Hour to set to 12-hour mode
  rtc.autoTime();
  rtc.update();
  strip1.begin(); //initialize LED strip
  strip1.setBrightness(50);


  // Initialize sensor
  if (!hro2.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  Serial.println(F("Attach sensor to finger"));

  hro2.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
  colorEyes(neutral);
  strip1.setBrightness(50);
  twist.attach(6); //initialize twist servo to pin 6
  tilt.attach(3); //initialize tilt servo to pin 3
  shake();
  quirk();

}


void loop() {

  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (hro2.available() == false) //do we have new data?
      hro2.check(); //Check the sensor for new data

    redBuffer[i] = hro2.getRed();
    irBuffer[i] = hro2.getIR();
    hro2.nextSample(); //We're finished with this sample so move to next sample

    
  }


  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (hro2.available() == false) //do we have new data?
        hro2.check(); //Check the sensor for new data

      //digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = hro2.getRed();
      irBuffer[i] = hro2.getIR();
      hro2.nextSample(); //We're finished with this sample so move to next sample
      
      rtc.update();//updat clock

     
     // int respiration = analogRead(RSP);
     // int pressure = analogRead(PRS);

      //Removing title print statements for easier conversion to excel
      //Time Log
      Serial.print((rtc.hour())); // Print hour
      Serial.print(":");
      if (rtc.minute() < 10)
        Serial.print('0'); // Print leading '0' for minute
      Serial.print((rtc.minute())); // Print minute
      Serial.print(",");

      Serial.print(heartRate, DEC);
      Serial.print(",");

      // Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);
      Serial.print(",");


      // Serial.print(F(", GSR: "));
       int gsrValue = analogRead(GSR);
      Serial.print(gsrValue);
      Serial.print(",");
      //Serial.print(F(", Respiration: "));
      Serial.print(respiration);
      //Serial.print(",");
      //Serial.print(F(", Pressure: "));
     // Serial.print(pressure);
      Serial.println();

      contrlVar = Serial.read();
      if(contrlVar == 1){
        //Happy Eyes
        //Quick yes gesture
        nod(); 
        colorEyes(happy);
      }
      if(contrlVar == 2){
        // Sad Eyes 
        nod();
        colorEyes(sad);
      }
      if(contrlVar == 3){
        // Angry Eyes
        //Quick yes gesture
        nod();
        colorEyes(angry);
      }
      if(contrlVar == 4){
        //Surprised Eyes 
        //Quick Yes gesture
        nod();
        colorEyes(surprised);
      }
      if(contrlVar == 5){
        // Scared Eyes 
        //Quick yes gesture
        nod();
        colorEyes(scared);
      }
      if(contrlVar == 6){
        //Neutral Eyes
        //Quick Yes Gesture
        nod();
        colorEyes(neutral);
      }
      if(contrlVar == 7){
        //Confused Gesture - Send 7 when Austin is querried verbally (Speech to text activated)
        quirk();
        delay(1000); 
      }
      if(contrlVar == 8){
        //Failure to get speech to text recognized 
      }
      if(contrlVar == 9){
        // Stress Detected activate pump for 2 miniutes, keep track of time and turn on vacuum after 20 minutes
        //add pump and vacuum code
        
      }
      else{
        colorEyes(standard);
        //motors off 
        tilt.write(0);
        twist.write(0);
      }
    }
    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    /*
       Print data in the following format:
       Time, Heart rate, SPO2, GSR, Respiration,
       Comma deliminited for easy CSV to Excel conversion
       Name prints for naming data
    */


  }
}

void quirk() {//leans forward and twists head in a "huh?" gesture holds it for 3 seconds then returns to original position
  tilt.write(60);
  delay(100);
  twist.write(60);
  delay(3000);
  tilt.write(90);
  delay(100);
  twist.write(90);
  delay(1000);
}

void colorEyes(emotion color) { //function to color eye LEDs. Reads in emotion struct
  strip1.setPixelColor(0, color.green, color.red, color.blue);
  strip1.setPixelColor(1, color.green, color.red, color.blue);
  strip1.show();
  delay(1000);
  strip1.setPixelColor(0,0,0,0);
  strip1.setPixelColor(1,0,0,0);
  strip1.show();
  delay(750);
  strip1.setPixelColor(0, color.green, color.red, color.blue);
  strip1.setPixelColor(1, color.green, color.red, color.blue);
  strip1.show();
  
}

void shake() {//shakes head 180 degrees 7 times
  for (n = 0; n < 3; n += 1) {
    for (posTwist = 0; posTwist <= 180; posTwist += 1) {
      twist.write(posTwist);
      delay(1);
    }
    for (posTwist = 180; posTwist >= 0; posTwist -= 1) {
      twist.write(posTwist);
      delay(1);
    }
  }
  twist.write(90);
}

void nod() {//nods head/body 60 degrees 7 times (note, "nod" does not have as much freedom as "shake" does
  for (n = 0; n < 3; n += 1) {
    for (posTilt = 60; posTilt <= 120; posTilt += 1) {
      tilt.write(posTilt);
      delay(2);
    }
    for (posTilt = 120; posTilt >= 60; posTilt -= 1) {
      tilt.write(posTilt);
      delay(2);
    }
  }
  tilt.write(90);
}
