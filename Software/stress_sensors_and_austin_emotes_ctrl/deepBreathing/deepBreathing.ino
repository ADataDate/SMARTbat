//Breathe Deep: vibrotactile stress mitigation through breathing exercises 
int vibe = 6; //Attach vibration motor to pin 6
int breathe = 0; //PWM value sent to vibration motor 
int breatheStep = 10; //PWM increments 

void setup() {
  // put your setup code here, to run once:
pinMode(vibe, OUTPUT);
}

void loop() {
  analogWrite(vibe, breathe);
  //increment PWM value (intensity of vibration)
  //Simulate inhale 
  breathe += breatheStep; 

  //Once max PWM is reached start decrementing
  //Simulate exhale 

  if(breathe <=0 || breathe >=175){
    delay(1000); //pause between enhale and exhale
    breatheStep = -breatheStep;
  }
  delay (175);

}
