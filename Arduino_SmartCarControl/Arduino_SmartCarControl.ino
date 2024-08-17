#include <Servo.h>
#include <NewPing.h>

// const int txPin = 0;
// const int rxPin = 1;
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
const int speed = 100;

Servo myServo;
NewPing sonar(triggerPin, echoPin, maxDistance);

bool movingForward = false;
String fireDirection = "";
String fireMode = "";
int fireSpeed = 0;

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
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    
    // Phân tích dữ liệu nhận được từ ESP8266
    if (input.startsWith("DIRECTION:")) {
      fireDirection = input.substring(10, input.indexOf(';'));
    } else if (input.startsWith("MODE:")) {
      fireMode = input.substring(5, input.indexOf(';'));
    } else if (input.startsWith("SPEED:")) {
      fireSpeed = input.substring(6).toInt();
    }
    Serial.println("Chế dộ: " + fireMode + ", Hướng: " + fireDirection + ", Tốc độ: " + String(fireSpeed));

    // Thực hiện hành động dựa trên dữ liệu nhận được
    if (fireMode == "manual") {
      if (fireDirection == "forward") {
        forward();
      } else if (fireDirection == "backward") {
        backward();
      } else if (fireDirection == "left") {
        left();
      } else if (fireDirection == "right") {
        right();
      } else if (fireDirection == "stopped") {
        stop();
      }
    } else if (fireMode == "automatic") {
      autoControl();
    }
  }
}

void autoControl() {
  int distance = sonar.ping_cm();
  // Serial.println(String(distance) + " cm");
  if (movingForward && distance > 0 && distance < 20) {
    backward();
    delay(100);
    stop();
    delay(500);
    int distanceRight = scanRight();
    int distanceLeft = scanLeft();
    if (distanceLeft > distanceRight) {
      left();
    } else {
      right();
    }
    delay(500);
  } else {
    forward();
    delay(100);
  }
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
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  digitalWrite(in1Pin, LOW);
  analogWrite(enaPin, speed);
  analogWrite(enbPin, speed);
}

void right() {
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