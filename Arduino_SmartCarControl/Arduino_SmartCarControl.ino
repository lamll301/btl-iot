#include <Servo.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int servoPin = 11;
const int echoPin = 9;
const int triggerPin = 10;
const int in1Pin = 8;
const int in2Pin = 7;
const int in3Pin = 6;
const int in4Pin = 4;
const int enaPin = 5;
const int enbPin = 3;
const int maxDistance = 200;

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myServo;
NewPing sonar(triggerPin, echoPin, maxDistance);

bool movingForward = false;
String fireDirection = "";
String fireMode = "";
int fireSpeed = -1;

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
  lcd.init();
  lcd.backlight();
  delay(1000);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    data(input);
    Serial.println("Chế dộ: " + fireMode + ", Hướng: " + fireDirection + ", Tốc độ: " + String(fireSpeed));
    loadLcd(fireMode, fireDirection, fireSpeed);
  }
  if (fireMode == "manual") {
    if (fireDirection == "forward") {
      forward(fireSpeed);
    } else if (fireDirection == "backward") {
      backward(fireSpeed);
    } else if (fireDirection == "left") {
      left(fireSpeed);
    } else if (fireDirection == "right") {
      right(fireSpeed);
    } else if (fireDirection == "stopped") {
      stop();
    }
  } else if (fireMode == "automatic") {
    automatic(fireSpeed);
  }
}

void data(String input) {
  int start = 0;
  while (start < input.length()) {
    int end = input.indexOf(';', start);
    if (end == -1) end = input.length();

    String part = input.substring(start, end);
    if (part.startsWith("DIRECTION:")) {
      fireDirection = part.substring(10);
    } else if (part.startsWith("MODE:")) {
      fireMode = part.substring(5);
    } else if (part.startsWith("SPEED:")) {
      fireSpeed = part.substring(6).toInt();
    }

    start = end + 1;
  }
}

void automatic(int speed) {
  int distance = sonar.ping_cm();
  if (movingForward && distance > 0 && distance < 20) {
    backward(speed);
    delay(100);
    stop();
    delay(500);
    int distanceRight = scanRight();
    int distanceLeft = scanLeft();
    if (distanceLeft > distanceRight) {
      left(speed);
    } else {
      right(speed);
    }
    delay(500);
  } else {
    forward(speed);
    delay(100);
  }
}

void loadLcd(String mode, String direction, int speed) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mode);
  lcd.setCursor(0, 1);
  lcd.print(direction + ", " + String(speed));
}

int scanRight() {
  myServo.write(30);
  delay(300);
  int distance = sonar.ping_cm();
  myServo.write(90);
  delay(300);
  return distance;
}

int scanLeft() {
  myServo.write(150);
  delay(300);
  int distance = sonar.ping_cm();
  myServo.write(90);
  delay(300);
  return distance;
}

void forward(int speed) {
  movingForward = true;
  digitalWrite(in2Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void backward(int speed) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void left(int speed) {
  movingForward = true;
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in1Pin, LOW);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void right(int speed) {
  movingForward = true;
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  digitalWrite(in3Pin, LOW);
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