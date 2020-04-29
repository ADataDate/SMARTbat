#include <Adafruit_NeoPixel.h>
#include <Servo.h>


int contrlVar;


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
  strip1.begin(); //initialize LED strip
  strip1.setBrightness(50);
  twist.attach(6); //initialize twist servo to pin 6
  tilt.attach(3); //initialize tilt servo to pin 3
  shake();
  quirk();
}


void loop() {

  contrlVar = Serial.read() - '0';
  Serial.println(contrlVar);
  if (contrlVar == 1) {
    //Happy Eyes
    //Quick yes gesture
    nod();
    colorEyes(happy);
  }
  if (contrlVar == 2) {
    // Sad Eyes
    nod();
    colorEyes(sad);
  }
  if (contrlVar == 3) {
    // Angry Eyes
    //Quick yes gesture
    nod();
    colorEyes(angry);
  }
  if (contrlVar == 4) {
    //Surprised Eyes
    //Quick Yes gesture
    nod();
    colorEyes(surprised);
  }
  if (contrlVar == 5) {
    // Scared Eyes
    //Quick yes gesture
    nod();
    colorEyes(scared);
  }
  if (contrlVar == 6) {
    //Neutral Eyes
    //Quick Yes Gesture
    nod();
    colorEyes(neutral);
  }
  if (contrlVar == 7) {
    //Confused Gesture - Send 7 when Austin is querried verbally (Speech to text activated)
    quirk();
    delay(1000);
  }
  if (contrlVar == 8) {
    //Failure to get speech to text recognized
    shake();
    delay(1000);
  }
  if (contrlVar == 9) {
    // Stress Detected activate pump for 2 miniutes, keep track of time and turn on vacuum after 20 minutes
    //add pump and vacuum code

  }
  else {
    colorEyes(standard);
    //motors off
    tilt.write(0);
    twist.write(0);
  }
}

void colorEyes(emotion color) { //function to color eye LEDs. Reads in emotion struct
  strip1.setPixelColor(0, color.green, color.red, color.blue);
  strip1.setPixelColor(1, color.green, color.red, color.blue);
  strip1.show();
  delay(1000);
  strip1.setPixelColor(0, 0, 0, 0);
  strip1.setPixelColor(1, 0, 0, 0);
  strip1.show();
  delay(750);
  strip1.setPixelColor(0, color.green, color.red, color.blue);
  strip1.setPixelColor(1, color.green, color.red, color.blue);
  strip1.show();

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
