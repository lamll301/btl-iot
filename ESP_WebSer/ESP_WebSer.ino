#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>

const char* ssid = "Tenda_325880";
const char* password = "12345678";
#define REFERENCE_URL "https://iot-project-b170f-default-rtdb.asia-southeast1.firebasedatabase.app/"

Firebase firebase(REFERENCE_URL);

void setup() {
  Serial.begin(9600);
  
  // Kết nối WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Kết nối Firebase
  if (firebase.begin()) {
    Serial.println("Successfully connected to Firebase");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void loop() {
  String fireDirection = firebase.getString("vehicle/direction");
  String fireMode = firebase.getString("vehicle/mode");
  int fireSpeed = firebase.getInt("vehicle/speed");

  Serial.print("DIRECTION:");
  Serial.print(fireDirection);
  Serial.print(";");
  
  Serial.print("MODE:");
  Serial.print(fireMode);
  Serial.print(";");
  
  Serial.print("SPEED:");
  Serial.println(fireSpeed);
  
  delay(1000);
}
