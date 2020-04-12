/* This sketch saves data from 4 on-body sensors
    Serial data will be collected by Raspberry Pi or similar in final form

    Code is adapted from SparkFun's example code for the MAX30105 heart rate monitor
    SP02 algorithm is written by Maxim Integrated
*/
#include <SPI.h>
#include <heartRate.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>
#include <SparkFunDS3234RTC.h>
MAX30105 hro2;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
float respiration;

int GSR = A0;
//int RSP = A1;
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


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  rtc.begin(DS13074_CS_PIN);
  rtc.set12Hour(); // Use rtc.set12Hour to set to 12-hour mode
  rtc.autoTime();
  rtc.update();

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

      int gsrValue = analogRead(GSR);
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
      Serial.print(gsrValue);
      //Serial.print(",");
      //Serial.print(F(", Respiration: "));
      // Serial.print(respiration);
      //Serial.print(",");
      //Serial.print(F(", Pressure: "));
      //Serial.print(pressure);
      Serial.println(",");



    }
    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    /*
       Print data in the following format:
       Time, Heart rate, SPO2, GSR, Respiration, pressure
       Comma deliminited for easy CSV to Excel conversion
       Name prints for naming data
    */


  }
}
