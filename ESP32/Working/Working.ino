#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
// #include <FirebaseJson.h>
#include <WiFiClient.h>
#include "time.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

const char *ntpServer = "asia.pool.ntp.org";
const long gmtOffset_sec = 20700;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec);

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "aryalktm_2.4"
#define WIFI_PASSWORD "9841613846"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCR9mTEFhGj0wdd5rHs7zimyDRN373OsJI"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://water-level-tracking-c7d82-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_PROJECT_ID "water-level-tracking-c7d82"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// FirebaseJson json;
// String jsonString;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

int trigger_pin = 5;
int echo_pin = 18;
int LED = 2;
int relay = 12;
int unix_time;
int distance_cm;
int temp_cm;
double depth_cm;
int safe_distance = 15;
int relay_status;
String documentPath = "wltd_relay";

void serialSetup() {
  Serial.begin(115200);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(LED, OUTPUT);
}

void wifiSetup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void timeSetup() {
  timeClient.begin();
  timeClient.setUpdateInterval(500);
}

void firebaseSetup() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void HCSR04loop() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
}

void distanceCalculation() {
  long duration = pulseIn(echo_pin, HIGH);
  distance_cm = (duration / 2) / 29.09;
  depth_cm = 125 - distance_cm;
  Serial.println(distance_cm);
  delay(500);
}

void relayON() {
  relay_status = 1;
  digitalWrite(LED, HIGH);
  digitalWrite(relay, LOW);
}

void relayOFF() {
  relay_status = 0;
  digitalWrite(LED, LOW);
  digitalWrite(relay, HIGH);
}

void relayControl() {
  relay_status = Firebase.RTDB.getInt(&fbdo, "relay_status");
  if (relay_status == 1) {
    relayON();
  } else if (relay_status == 0) {
    relayOFF();
  } else {
    relayOFF();
  }

  if (distance_cm >= safe_distance) {
    relayON();
  } else if (distance_cm < safe_distance) {
    relayOFF();
  } else {
    relayON();
  }
}

void getRelayStatus() {
  relay_status = Firebase.RTDB.getInt(&fbdo, "/test/relay_status");
}

void setup() {
  serialSetup();
  wifiSetup();
  timeSetup();
  firebaseSetup();
}

void loop() {
  relayControl();
  FirebaseAuth auth;
  FirebaseJson json;
  timeClient.update();
  unsigned long currentTime = timeClient.getEpochTime();

  HCSR04loop();
  distanceCalculation();

  json.set("fields/relay_status", relay_status);
  if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), json.raw())) {
    Serial.println("Relay Status Pushed");
    return;
  } else {
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    // FirebaseJson json;
    sendDataPrevMillis = millis();
    // json.set("/distance", distance_cm);
    // json.set("/relay_status", relay_status);
    // json.set("/unix_time", static_cast<int>(currentTime));
    // Firebase.RTDB.pushInt(&fbdo, "relay_status", relay_status);
    // Firebase.RTDB.setJSON(&fbdo, parentPath, &json);
    Firebase.RTDB.pushDouble(&fbdo, "test/depth", depth_cm);
    Firebase.RTDB.pushInt(&fbdo, "test/unix_time", static_cast<int>(currentTime));
    Firebase.RTDB.pushInt(&fbdo, "test/relay_status", relay_status);
    return;
  }
}
