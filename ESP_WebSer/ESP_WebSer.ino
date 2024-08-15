#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

const char* ssid = "VNPT_LyLam";
const char* password = "30012003";
#define FIREBASE_HOST "iot-project-b170f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH ""

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

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
  // Kết nối Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, )
}

void setup() {
  Serial.begin(9600);
  setupServer();
}

void loop() {
  
}
