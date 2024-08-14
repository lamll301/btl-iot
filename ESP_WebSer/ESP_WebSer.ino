#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Cấu hình WiFi
const char* ssid = "VNPT_LyLam";
const char* password = "30012003";

// Cấu hình Firebase
#define FIREBASE_HOST "iot-project-b170f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "YMaGGxjnJwbXeqYWp9gVh2txwBNGm3XNfdOJ0brA"

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

String fireDirection = "";
String fireMode = "";
int fireSpeed = 0;

void setupServer() {
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

  // Cấu hình Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.api_key = FIREBASE_AUTH;

  // Khởi động Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(9600);
  setupServer();
}

void loop() {
  if (Firebase.getString(firebaseData, "/vehicle/direction")) {
    fireDirection = firebaseData.stringData();
    Serial.print("Direction: ");
    Serial.println(fireDirection);
  } else {
    Serial.print("Failed to get direction: ");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.getString(firebaseData, "/vehicle/mode")) {
    fireMode = firebaseData.stringData();
    Serial.print("Mode: ");
    Serial.println(fireMode);
  } else {
    Serial.print("Failed to get mode: ");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.getInt(firebaseData, "/vehicle/speed")) {
    fireSpeed = firebaseData.intData();
    Serial.print("Speed: ");
    Serial.println(fireSpeed);
  } else {
    Serial.print("Failed to get speed: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(2000);  // Đợi 2 giây trước khi lặp lại
}
