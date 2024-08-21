#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>

#define _SSID ""
#define _PASSWORD ""
#define DATABASE_URL "iot-project-b170f-default-rtdb.asia-southeast1.firebasedatabase.app"

Firebase firebase(DATABASE_URL);

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String direction = firebase.getString("vehicle/direction");
  String mode = firebase.getString("vehicle/mode");
  int speed = firebase.getInt("vehicle/speed");
  Serial.print("DIRECTION:");
  Serial.print(direction);
  Serial.print(";");
  Serial.print("MODE:");
  Serial.print(mode);
  Serial.print(";");
  Serial.print("SPEED:");
  Serial.println(speed);
  delay(1000);
}

