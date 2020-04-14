
#define sensorPin A0 
 
void setup(void) {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
}
 
void loop(void) {
  float pressureReading;
  float averagePressure = 0;
  float numMeas = 25;

  for(int i = 0; i<numMeas; i++)
    {
      pressureReading = analogRead(sensorPin);
      averagePressure = averagePressure + pressureReading;
      delay(50);
    }
    averagePressure = averagePressure/numMeas;

  Serial.print("Vest Pressure: "); 
  Serial.println(averagePressure);    
  delay(250);
}
