
#define respSensorPin A1 
 
void setup(void) {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
}
 
void loop(void) {
  float respirationReading;
  float averageRespiration = 0;
  float numMeas = 25;

  for(int i = 0; i<numMeas; i++)
    {
      respirationReading = analogRead(sensorPin);
      averageRespiration = averageRespiration + respirationReading;
      delay(50);
    }
    averagePressure = averagePressure/numMeas;

  Serial.print("Respiration: "); 
  Serial.println(averageRespiration);    
  delay(25);
}
