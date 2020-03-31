erial #include <SPI.h>
#include <SparkFunBME280.h>
#include <heartRate.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

#define GSR A1

BME280 mySensor;
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

float temp;
float sweat;
int gsrValue;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  if (mySensor.beginI2C() == false) //Begin communication over I2C for BME280
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1); //Freeze
  }

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}


void loop() {
  // put your main code here, to run repeatedly:
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }

    sweat = mySensor.readFloatHumidity();
    temp = mySensor.readTempF();
    gsrValue = analogRead(GSR);

  }

  Serial.print("BPM: ");
  Serial.println(beatsPerMinute);

  if (irValue < 50000)
    Serial.println(" No finger?");

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Sweat: ");
  Serial.println(sweat);
  Serial.print("GSR: ");
  Serial.print(gsrValue);

  Serial.println();

}
