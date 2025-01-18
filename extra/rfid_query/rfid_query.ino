#include <Arduino.h>
#include <FirebaseESP32.h>
#include <WiFiClientSecure.h> 
// #include <FirebaseJson.h>
 

#define FIREBASE_HOST "https://narcs-1805c.firebaseio.com/"
#define FIREBASE_AUTH "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f"
#define API_KEY "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630"
#define WIFI_SSID "AmshuBelbase"
#define WIFI_PASSWORD "amshu0128"

FirebaseData fbdo;
// QueryFilter query;
bool signupOK = false;

FirebaseConfig config;
FirebaseAuth auth;
// FirebaseJson json;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected");
  // config.api_key = API_KEY;  
  // Firebase.begin(&config, &auth);
  config.api_key = API_KEY;
  config.database_url = FIREBASE_HOST; 
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }     
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
bool checkPhone(String rf_id) {
  QueryFilter query;
  query.orderBy("rfid");
  query.startAt(rf_id);
  query.endAt(rf_id);  

  if (Firebase.getJSON(fbdo, "/rfid/users/", query)) {
    FirebaseJson* json = fbdo.jsonObjectPtr(); 
    size_t len = json->iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++) {
      json->iteratorGet(i, type, key, value); 
        Serial.print("reached");
        Serial.print("key = ");
        Serial.print(key);
        Serial.print(" value = ");
        Serial.println(value);
        // Parse JSON
        FirebaseJson jsonObject;
        jsonObject.setJsonData(value); 
        FirebaseJsonData rfidData;
        FirebaseJsonData usernameData;

        if (jsonObject.get(rfidData, "rfid")) {
          if (rfidData.success) {
            const char* rfid = rfidData.to<const char*>();

            Serial.print("RFID: ");
            Serial.println(rfid);
          } else {
            Serial.println("Failed to extract RFID value.");
          }  
        }
        if (jsonObject.get(usernameData, "username")) {
          if (usernameData.success) {
            const char* username = usernameData.to<const char*>();

            Serial.print("Username: ");
            Serial.println(username);
          } else {
            Serial.println("Failed to extract Username value.");
          }  
        } 
        json->iteratorEnd();
        return true; 
    }
    json->iteratorEnd(); 
    Serial.print("reached3");
    return false;

  } else {
    Serial.println("Error querying Firebase");
    return false;
  } 
} 
void loop() {
  if (Firebase.ready()) {
    String rf_number = "571131101";

    if (checkPhone(rf_number)) {
      Serial.println("Number exists");
    }
    else {
      Serial.println("Number does not exist");
    }
  } else {
  Serial.println("Error, Firebase not ready");
  }
  delay(5000);
}


// #include <WiFiClientSecure.h>
// #include <HTTPClient.h>
// // #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"
// #include <ArduinoJson.h>

// #include <Arduino.h>
// #include <FirebaseESP32.h>

// #define APIKEY "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630"

// #define AUTHKEY  "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f"
// #define DB_URL "https://narcs-1805c.firebaseio.com/"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;
// bool signupOK = false;

// String ssid = "AmshuBelbase";
// String pass = "amshu0128"; 

// void setup() { 
//   Serial.begin(115200);
//   Serial.println("Current Network to Connect : "+ssid);
//   WiFi.begin(ssid, pass);
//   Serial.print("Connecting to WiFi");
//   while(WiFi.status() != WL_CONNECTED){
//     Serial.print(".");
//     delay(300);
//   } 
//   Serial.println("");
//   Serial.println("Connected to WiFi : "+String(ssid)+". IP : "+String(WiFi.localIP()));
//   config.api_key = APIKEY;
//   config.database_url = DB_URL; 
//   /* Sign up */
//   if (Firebase.signUp(&config, &auth, "", "")){
//     Serial.println("ok");
//     signupOK = true;
//   }
//   else{
//     Serial.printf("%s\n", config.signer.signupError.message.c_str());
//   }    
//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);
//   delay(100);
// }

// void loop(){

// }