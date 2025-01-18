#include <FirebaseESP32.h> 
#include <TinyGsmClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>  
#include <HardwareSerial.h>
#include <WiFi.h> 
HardwareSerial sim800lSerial(1); 
// Firebase credentials
#define FIREBASE_HOST "https://narcs-1805c.firebaseio.com/"
#define FIREBASE_AUTH "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f"
#define API_KEY "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630" 

FirebaseConfig config;
FirebaseAuth auth; 

// GPRS credentials
#define APN "www"
#define GPRS_USER ""
#define GPRS_PASS ""

// Time Stamp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

TinyGsm modem(sim800lSerial);
TinyGsmClient gsmClient(modem);

FirebaseData firebaseData;

void setup(){
  Serial.begin(115200);
  sim800lSerial.begin(115200, SERIAL_8N1, 27, 14); //rx, tx 
  delay(1000);

  // Ensure the SIM800L is responsive
  sendCommand("AT");

  // Set SMS mode to text
  sendCommand("AT+CMGF=1");

  // Set APN (skip username and password if not required)
  sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendCommand("AT+SAPBR=3,1,\"APN\",\"www\"");

  // Enable GPRS
  sendCommand("AT+SAPBR=1,1");

  // Check GPRS connection status
  sendCommand("AT+SAPBR=2,1");

  Serial.println("GPRS configuration complete.");
  // Firebase initialization
  // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, gsmClient);
  config.api_key = API_KEY;
  config.database_url = FIREBASE_HOST; 
  /* Sign up */
  // if (Firebase.signUp(&config, &auth, "", "")){
  //   Serial.println("Sign up Done");
  //   signupOK = true;
  // }
  // else{
  //   Serial.printf("%s\n", config.signer.signupError.message.c_str());
  // } 
  // Serial.print("signupOK = ");
  // Serial.print(signupOK);    
  Firebase.begin(&config, &auth, gsmClient);
  Firebase.reconnectWiFi(true);
  timeClient.begin(); 

  Serial.println("Firebase initialization complete.");
}
void sendCommand(String command) {
  sim800lSerial.println(command);
  delay(500);
  while (sim800lSerial.available()) {
    char c = sim800lSerial.read();
    Serial.write(c);
  }
  Serial.println();
}
void loop() {
    delay(5000); // Adjust the delay according to your application requirements
}






// #include<HardwareSerial.h>
// HardwareSerial sim800lSerial(1); 

// void setup() {
//   Serial.begin(115200);
//   sim800lSerial.begin(115200, SERIAL_8N1, 27, 14); //rx, tx 
//   delay(1000);

//   // Ensure the SIM800L is responsive
//   sendCommand("AT");

//   // Set SMS mode to text
//   sendCommand("AT+CMGF=1");

//   // Set APN (skip username and password if not required)
//   sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
//   sendCommand("AT+SAPBR=3,1,\"APN\",\"www\"");

//   // Enable GPRS
//   sendCommand("AT+SAPBR=1,1");

//   // Check GPRS connection status
//   sendCommand("AT+SAPBR=2,1");

//   Serial.println("GPRS configuration complete.");
// }

// void loop() {
//   while (sim800lSerial.available()) {
//     char c = sim800lSerial.read();
//     Serial.write(c);
//   } 
//   delay(100);
// }

// void sendCommand(String command) {
//   sim800lSerial.println(command);
//   delay(500);
//   while (sim800lSerial.available()) {
//     char c = sim800lSerial.read();
//     Serial.write(c);
//   }
//   Serial.println();
// }