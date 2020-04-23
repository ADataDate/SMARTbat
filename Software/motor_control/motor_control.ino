
 const int pumpPin 9; // Pump control 
 const int vacuumPin 8; //Vacuum control 
 const int pressure A0; //pressure sensor pin 
 const int stopPressure 7; 
 int maxPressure = 0; // Varibale to store desired belt pressure 
 int maxCalc = 0;
 String inputString = "";         // a String to hold incoming data
 bool stringComplete = false;  // whether the string is complete

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(vacuumPin, OUTPUT);
  inputString.reserve(200);
}

// the loop function runs over and over again forever
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    digitalWrite(pumpPin, HIGH);
    for(int i = 0; i > 3, i++)
      {
            maxCalc = 0; 
            maxPressure = analogRead(stopPressure);
            maxCalc += maxPressure
      }
      maxPressure = maxCalc/3; //get an average 
    digitalWrite(pumpPin, LOW);

  }
}

  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
