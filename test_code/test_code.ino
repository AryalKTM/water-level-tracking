
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <WiFiManager.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "aryalktm_2.4";
#define WIFI_PASSWORD "9841636846";
#define API_KEY "AIzaSyCR9mTEFhGj0wdd5rHs7zimyDRN373OsJI" 
#define DATABASE_URL "https://water-level-tracking-c7d82-default-rtdb.asia-southeast1.firebasedatabase.app";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMills = 0;

const int triggerPIN = 5;
const int echoPIN = 18;
const int inbuiltLED = 2;
const int relayPIN = 15;

bool signUPOK;
int distanceCM;

//***************************SETUP*******************************

void pinSetup() {
  Serial.begin(115200);
  pinMode(triggerPIN, OUTPUT);
  pinMode(echoPIN, INPUT);
  pinMode(relayPIN, OUTPUT);
  pinMode(inbuiltLED, OUTPUT);
}

void wifiSetup() {
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("WLTD");
  if (!res) {
    Serial.println("Failed to Connect to WiFi");
  }
}

void firebaseSetup() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("OK");
    signUPOK = true;
    pinMode(inbuiltLED, uint8_t HIGH);
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

//***************************SETUP*FUNCTION**********************

void setup() {
  pinSetup();
  wifiSetup();
  firebaseSetup();
}

//***************************LOOP*********************************

void calculateDistance(){
  digitalWrite(triggerPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPIN, LOW);

  long duration = pulseIn(echoPIN, HIGH);
  distanceCM = (duration/2) / 29.09;
  Serial.printf("Distance: %d cm", distanceCM);
}

void loop () {
  calculateDistance();
}