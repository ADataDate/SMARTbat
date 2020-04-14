//TODO: Add manual joystick override  

#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Adafruit_NeoPixel strip1(2, 6, NEO_RGB);//setup LED strip, (#LEDs, port, NEO_RGB)
Servo twist; //declare servo called twist
Servo tilt; //declare serco called tilt

struct emotion { //struct containg RGB values for each emotion
  int red;
  int green;
  int blue;
};

emotion angry {255, 0, 0};
emotion scared {255, 75, 3};
emotion happy {255, 200, 0};
emotion sad {0, 0, 255};
emotion surprised {150, 0, 255};
emotion neutral {255, 255, 255};
emotion standard {0, 255, 20};

int posTwist = 0; //Head lateral position
int posTilt = 0;//Head vertical position
int n = 0;

void setup() {
  strip1.begin(); //initialize LED strip

  twist.attach(11); //initialize twist servo to pin 11
  tilt.attach(10); //initialize tilt servo to pin 10
}
void loop() {
  colorEyes(angry);
  delay(1000);
  colorEyes(scared);
  delay(1000);
  colorEyes(happy);
  delay(1000);
  colorEyes(sad);
  delay(1000);
  colorEyes(surprised);
  delay(1000);
  colorEyes(neutral);
  delay(1000);
  colorEyes(standard);
  delay(1000);

  shake();
  delay(1000);
  nod();
  delay(1000);
  quirk();
  delay(1000);
}

void colorEyes(emotion color) { //function to color eye LEDs. Reads in emotion struct
  strip1.setPixelColor(0, color.green, color.red, color.blue);
  strip1.setPixelColor(1, color.green, color.red, color.blue);
  strip1.show();
}

void shake() {//shakes head 180 degrees 7 times
  for (n = 0; n < 7; n += 1) {
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
  for (n = 0; n < 7; n += 1) {
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
