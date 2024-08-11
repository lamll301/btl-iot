#include <Servo.h>
#include <NewPing.h>

const int servoPin = 11;
const int echoPin = 9;
const int triggerPin = 10;
const int maxDistance = 200;

const int in1Pin = 8;
const int in2Pin = 7;
const int in3Pin = 6;
const int in4Pin = 4;
const int enaPin = 5;
const int enbPin = 3;
const int speed = 255;
// Define servo angles to check
const int leftAngle = 30;
const int rightAngle = 150;
const int centerAngle = 90;

Servo myServo;
NewPing sonar(triggerPin, echoPin, maxDistance);

void setup() {
  myServo.attach(servoPin);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);
  pinMode(enbPin, OUTPUT);
  myServo.write(centerAngle);
  delay(1000);
}

void loop() {
  if (checkObstacle()) {
    chooseDirection();
  } else {
    moveForward();
  }
  delay(100);
}

bool checkObstacle() {
  unsigned int distance = sonar.ping_cm();
  if (distance > 0 && distance < 20) {
    return true;
  }
  return false;
}

void chooseDirection() {
  stopMovement();
  delay(500);

  moveBackward();
  delay(1000);
  stopMovement();
  delay(500);

  // Check left
  myServo.write(leftAngle);
  delay(500);
  unsigned int leftDistance = sonar.ping_cm();
  
  // Check right
  myServo.write(rightAngle);
  delay(500);
  unsigned int rightDistance = sonar.ping_cm();
  
  // Return to center position
  myServo.write(centerAngle);
  delay(500);
  
  // Choose direction based on distances
  if (leftDistance > rightDistance) {
    turnLeft();
  } else {
    turnRight();
  }
  delay(1000);
  moveForward();
}

void moveForward() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPin, speed);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enbPin, speed);
}

void moveBackward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, speed);

  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enbPin, speed);
}

void turnLeft() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPin, speed);

  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enbPin, speed);
}

void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, speed);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enbPin, speed);
}

void stopMovement() {
  analogWrite(enaPin, 0);
  analogWrite(enbPin, 0);
}
