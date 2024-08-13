#include <Servo.h>

const int servoPin = 11;
const int echoPin = 9;
const int triggerPin = 10;
const int in1Pin = 8;
const int in2Pin = 7;
const int in3Pin = 6;
const int in4Pin = 4;
const int enaPin = 5;
const int enbPin = 3;
Servo myServo;

const int speed = 100;
bool movingForward = false;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);
  pinMode(enbPin, OUTPUT);
  myServo.write(90);
  delay(1000);
}

void loop() {
  long distance = getDistance();
  Serial.println(String(distance) + " cm");
  if (movingForward && distance > 0 && distance < 20) {
    stop();
    long distanceRight = scanRight();
    long distanceLeft = scanLeft();
    Serial.println("Distance Right: " + String(distanceRight) + " cm");
    Serial.println("Distance Left: " + String(distanceLeft) + " cm");
    if (distanceLeft > distanceRight) {
      Serial.println("Turning Left");
      left();
    } else {
      Serial.println("Turning Right");
      right();
    }
    delay(500);
  } else {
    forward();
    delay(100);
  }
}

unsigned int getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  unsigned int distance = duration * 0.034 / 2;
  return distance;
}

long scanRight() {
  myServo.write(30);
  delay(300);
  long distance = getDistance();
  myServo.write(90);
  delay(300);
  return distance;
}

long scanLeft() {
  myServo.write(150);
  delay(300);
  long distance = getDistance();
  myServo.write(90);
  delay(300);
  return distance;
}

void forward() {
  movingForward = true;
  digitalWrite(in2Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void backward() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void left() {
  movingForward = true;
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in1Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void right() {
  movingForward = true;
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void stop() {
  movingForward = false;
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enaPin, 0);
  analogWrite(enbPin, 0);
}