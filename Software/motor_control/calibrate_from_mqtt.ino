
const int pumpPin = 9; //Pump control
const int vacuumPin = 6; //Vacuum control
const int pressure = A0; //pressure sensor pin
int pressureReading = 0;
int minPressure = 0;
int newMinPressure = 0;
int maxPressure = 0; // Varibale to store desired belt pressure
int maxCalc = 0;
int interruptCalled = 0;
bool first_call = true;

const byte interruptPin = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(vacuumPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  digitalWrite(pumpPin, LOW);
  digitalWrite(vacuumPin, HIGH);

  attachInterrupt(digitalPinToInterrupt(interruptPin), pressureSensor, CHANGE);
  //Press red button to set maxPressure
  Serial.println("Exiting setup");
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Start of loop");

  command = Serial.read() - '0';

  // command == 0 is 'start calibrate' command
  if (first_call==true && command=='0'){
    // start claibration
    fist_call = false;
    //get on-body, unpumped pressure reading
    minPressure = analogRead(pressure);
    Serial.println(minPressure);

    //Start pump
    digitalWrite(pumpPin, HIGH);
    delay(500);
    digitalWrite(pumpPin, LOW);
  }

  #if 1
  //delay(2000);
  //Test turning on the belt to max pressure
  pressureReading = analogRead(pressure);
  Serial.println(pressureReading);
  if (pressureReading < maxPressure)
  {
    Serial.println("pressureREading < maxPressure");
    Serial.print(pressureReading); Serial.print(' '); Serial.println(maxPressure);

    #if 0
    digitalWrite(pumpPin, HIGH);
    delay(500);
    digitalWrite(pumpPin, LOW); //Turn on pump until max pressure is achieved
    #endif
  }
  if (pressureReading >= maxPressure)
  {
    Serial.println("pressureReading >= maxPressure");
    Serial.print(pressureReading); Serial.print(' '); Serial.println(maxPressure);

    #if 0
    digitalWrite(pumpPin, HIGH);
    delay(500);
    digitalWrite(pumpPin, LOW); //Turn off pump once it reached max pressure

    if (newMinPressure >= minPressure) {
      delay(10000); //delay 10 seconds to check for leaks
      digitalWrite(vacuumPin, LOW);
      delay(500);
      digitalWrite(vacuumPin, HIGH);
      delay(5000);
      digitalWrite(vacuumPin, LOW);
      delay(500);
      digitalWrite(vacuumPin, HIGH);
    }
    #endif

  }

  if ( interruptCalled == 1 )
  {
    //turn off pump
    Serial.println("Processing interrupt");
    digitalWrite(pumpPin, HIGH);
    delay(500);
    digitalWrite(pumpPin, LOW);

  #if 1
    digitalWrite(vacuumPin, LOW);
    delay(1000);
    digitalWrite(vacuumPin, HIGH);
    delay(2000); //wait for belt to vacuum air out
    //Turn vacuum off
    digitalWrite(vacuumPin, LOW);
    delay(1000);
    digitalWrite(vacuumPin, HIGH);
  #else
    digitalWrite(vacuumPin, HIGH);
    delay(1000);
    digitalWrite(vacuumPin, LOW);
    delay(2000); //wait for belt to vacuum air out
    //Turn vacuum off
    digitalWrite(vacuumPin, HIGH);
    delay(1000);
    digitalWrite(vacuumPin, LOW);
  #endif
    interruptCalled = 0;

    Serial.println("Done processing interrupt");
  }

  #endif
}

/*
  on interrupt: Press the red button to set max pressure and turn off pump and
  turn on vacuum.
*/
void pressureSensor() {
  Serial.println("Interrupt triggered");
  maxCalc = 0;
  for (int i = 0; i < 3; i++) //get an average reading for the max pressure
  {
    maxPressure = analogRead(pressure);
    maxCalc += maxPressure;
  }
  maxPressure = maxCalc / 3; //get an average
  interruptCalled = 1;

  Serial.println(maxPressure);
  Serial.println("Exiting interrupt");
  return maxPressure;
}
