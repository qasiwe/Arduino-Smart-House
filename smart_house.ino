//#include <Adafruit_CircuitPlayground.h>
//#include <Adafruit_Circuit_Playground.h>

/*

*/

#include <Servo.h> 
#include <dht.h>

int trigPin =  9;
int echoPin = 10;
int lightSensorPin = A0;
int modeLightSwitchPin = 8;
int manualLightSwitchPin = 3;
int pirPin = 2;
int ledPin = 5;
int accelPin = A2;
int speakerPin = 11;
int humidityPin = 4;
int gasSensorPin = A1;
#define LIGHT_AUTOMATIC 1
#define LIGHT_MANUAL 0
const int aNote = 440;
bool isGateOpen = false;

Servo gateServo;
Servo windowServo;
int pos = 0;
int lightSensorValue = 0;
dht sensor;
void setup() {
  gateServo.attach(7);
  windowServo.attach(12);
  pinMode(trigPin, OUTPUT); // setting the output pin with fast output (will output signals in high speed)
  pinMode(echoPin, INPUT); // setting the input pin with fast input (will get signals in high speed)
  pinMode(lightSensorPin, INPUT);
  pinMode(modeLightSwitchPin, INPUT);
  pinMode(manualLightSwitchPin, INPUT);
  pinMode(pirPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(accelPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(gasSensorPin, INPUT);
  digitalWrite(pirPin, LOW);
  pinMode(humidityPin, INPUT);
  delayMicroseconds(50);
  Serial.begin(9600);
}

void loop() {
handleLight();
  handleGate();
//digitalWrite(ledPin, HIGH);
  handleAlarm();
   // openGate(gateServo);
  handleHumidity();
}

void handleHumidity() {
  delay(2000);
  bool hum = digitalRead(humidityPin);
  if (hum) {
    closeGate(windowServo);
  } else {
    openGate(windowServo);
  }
}

void handleAlarm() {
  int val = analogRead(accelPin);
  delay(500);
  bool hasGas = analogRead(gasSensorPin) > 600;
  bool isMoving = val < 900;
  if (hasGas) {
    analogWrite(speakerPin, aNote);
  } else {
    noTone(speakerPin);
  }
}

void handleLight() {
  bool isDark = analogRead(lightSensorPin) > 300;
  int isHuman = digitalRead(pirPin);
  if (digitalRead(modeLightSwitchPin) == LIGHT_MANUAL) {
    digitalWrite(ledPin, digitalRead(manualLightSwitchPin));
  } else if (digitalRead(modeLightSwitchPin) == LIGHT_AUTOMATIC) {
      if (isDark && isHuman) {
        digitalWrite(ledPin, HIGH);
      } else {
        digitalWrite(ledPin, LOW);
      }
  }
}

void handleGate(){
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.println(distance);
  if (distance >= 150 || distance <= 0) {
    closeGate(gateServo);
  } else {
    openGate(gateServo);
  }
}

void openGate(Servo servo) {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos); // tell servo to go to position in variable 'pos'
   //delay(15); // waits 15ms for the servo to reach the position
  }
}

void closeGate(Servo servo) {
  for (pos = 180; pos >= 45; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos); // tell servo to go to position in variable 'pos'
    //delay(15); // waits 15ms for the servo to reach the position
  }

  
  
}