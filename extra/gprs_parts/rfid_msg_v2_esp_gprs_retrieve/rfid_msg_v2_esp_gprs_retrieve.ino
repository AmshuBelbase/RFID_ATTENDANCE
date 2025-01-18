#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient
#include <HardwareSerial.h>
#include <Arduino.h>
#include <FirebaseJson.h>  

String key, value;

HardwareSerial sim(1); //TXD1 and RXD1 is used 

const char FIREBASE_HOST[]  = "narcs-1805c.firebaseio.com";
const String FIREBASE_AUTH  = "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f"; 

const String FIREBASE_PATH  = "/rfid/users/";
const int SSL_PORT          = 443;
 
char apn[]  = "portalnmms";  // type your APN here
char user[] = "";
char pass[] = "";
 
 
TinyGsm modem(sim);
 
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

unsigned long previousMillis = 0;
 
 
void setup()
{
  Serial.begin(115200);
  //sim800.begin(9600);
  sim.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  delay(1000);
  Serial.println("\nInitializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  http_client.setHttpResponseTimeout(10 * 1000); //^0 secs timeout
}
 
void loop()
{
 
  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  Serial.println(" OK");
 
  http_client.connect(FIREBASE_HOST, SSL_PORT);
 
  while (true) {
    if (!http_client.connected())
    {
      Serial.println();
      http_client.stop();// Shutdown
      Serial.println("HTTP  not connect");
      break;
    }
    else
      GetFirebase("PATCH", FIREBASE_PATH, &http_client);  
  }
 
} 

void GetFirebase(const char* method, const String & path ,  HttpClient* http)
{
  String response;
  int statusCode = 0;
  http->connectionKeepAlive(); // Currently, this is needed for HTTPS
 
  String url;
  if (path[0] != '/')
  {
    url = "/";
  }
  url += path + ".json";
  String rf = "57113140101";
  url += "?orderBy=\"rfid\"&equalTo=\""+rf+"\"&auth=" + FIREBASE_AUTH;

  http->get(url);
 
 //statusCode = http->responseStatusCode();
 // Serial.print("Status code: ");
  //Serial.println(statusCode);
  response = http->responseBody();
 
  Serial.print("Response: ");
  Serial.println(response);
  FirebaseJson json;
  json.setJsonData(response);      
  size_t len = json.iteratorBegin();
  String keyph, value = "";
  int type = 0;
  for (size_t i = 0; i < len; i++) {
    json.iteratorGet(i, type, keyph, value);
    // Parse JSON
    FirebaseJson jsonObject;
    jsonObject.setJsonData(value); 
    FirebaseJsonData rfidData;
    FirebaseJsonData usernameData;
    FirebaseJsonData userregData;

    if (jsonObject.get(rfidData, "rfid")) {
      if (rfidData.success) {
        const char* rfid = rfidData.to<const char*>(); 
        Serial.print(rfid);
      } else {
        Serial.println("No Success - RFID"); 
      }  
    } 
    if (jsonObject.get(usernameData, "username")) {
      if (usernameData.success) {
        const char* usernameChar = usernameData.to<const char*>();  
        Serial.print(usernameChar);
      } else {
        Serial.println("No Success - Username"); 
      }   
    }  
  } 
  if (!http->connected())
  {
    Serial.println();
    http->stop();// Shutdown
    Serial.println("HTTP POST disconnected");
  }
}
 