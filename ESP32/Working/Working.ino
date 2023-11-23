#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <WiFiClient.h>
#include <WiFiManager.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyCR9mTEFhGj0wdd5rHs7zimyDRN373OsJI"
#define DATABASE_URL "https://water-level-tracking-c7d82-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_PROJECT_ID "water-level-tracking-c7d82"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson content;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

int trigger_pin = 5;
int echo_pin = 18;
int LED = 2;
int relay = 15;
double depth_cm;
int safe_distance = 15;
int distance_cm;
bool relay_status;
bool relay_status_phone;
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
  WiFiManager wm;
  bool res;
  wm.erase();
  res = wm.autoConnect("WLTD");
  if (!res) {
    Serial.println("Failed to Connect to Wi-Fi");
  }
}

void firebaseSetup() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void calculateDistance() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  distance_cm = (duration / 2) / 29.09;
  depth_cm = 125 - distance_cm;
  Serial.println(distance_cm);
  delay(500);
}

void relayON() {
  relay_status = true;
  digitalWrite(LED, HIGH);
  digitalWrite(relay, LOW);
}

void relayOFF() {
  relay_status = false;
  digitalWrite(LED, LOW);
  digitalWrite(relay, HIGH);
}

void relayControl() {
  if (distance_cm >= safe_distance) {
    relayON();
  } else if (distance_cm < safe_distance) {
    relayOFF();
  } else {
    relayON();
  }
}

void sendFirebaseData() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Firebase.RTDB.pushDouble(&fbdo, "test/depth", depth_cm);
    Firebase.RTDB.setBool(&fbdo, "test/relay_status", relay_status);
  }
}

//*********************************SETUP*********************************

void setup() {
  serialSetup();
  wifiSetup();
  firebaseSetup();
}

//*********************************LOOP**********************************

void loop() {
  FirebaseAuth auth;
  FirebaseJson json;

  relayControl();
  calculateDistance();

  sendFirebaseData();
}
