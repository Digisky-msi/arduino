//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 150;
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int cw;
int ccw;
int cnt;
int serial_read;


// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // initialize serial: use serial for minor adjust when board connected to dev platform
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input: use switch for power up alignment
  pinMode(buttonPin, INPUT);

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  while (buttonState == HIGH){
    myStepper.setSpeed(50);
    myStepper.step(-stepsPerRevolution/50);
    digitalWrite(ledPin, LOW);
    buttonState = digitalRead(buttonPin);
  }

  digitalWrite(ledPin, HIGH);
  myStepper.setSpeed(50);
  myStepper.step(stepsPerRevolution/10);
  digitalWrite(ledPin, LOW);
  
}

void loop() {
  cnt = 0;
  
  // Rotate CW
  myStepper.setSpeed(150);
  myStepper.step(stepsPerRevolution);
  delay(1000);
  
  // Rotate CCW
  myStepper.setSpeed(150);
  myStepper.step(-stepsPerRevolution);

  // turn off pin save power
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  
  // pause time
  while (cnt < 6000){
    delay(10);
    cnt += 1;

    // detect if serial available and user input
    cw = 0;
    ccw = 0;
    while (Serial.available() > 0) {
      serial_read = Serial.read();
      if (serial_read == ',') {
        ccw = 1;
        cw = 0;
      }
      else if (serial_read == '.'){
        cw = 1;
        ccw = 0;
      }
    }

    // user adjust position
    if (cw == 1){
      myStepper.setSpeed(150);
      myStepper.step(stepsPerRevolution/10);
    }
  
    if (ccw == 1){
      myStepper.setSpeed(150);
      myStepper.step(-stepsPerRevolution/10);
    }
    
  }
}
