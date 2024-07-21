#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256 
#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient
#include <HardwareSerial.h>
//-- 
HardwareSerial sim(1); //TXD1 and RXD1 is used 

const char FIREBASE_HOST[]  = "narcs-1805c.firebaseio.com";
const String FIREBASE_AUTH  = "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f";

const String FIREBASE_PATH  = "/rfid/logs/";
const int SSL_PORT          = 443; 
String labNum = "2";
String epochTime = "12345";

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "portalnmms";
char user[] = "";
char pass[] = "";
//--
 
TinyGsm modem(sim);  
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT); 

unsigned long previousMillis = 0;
long interval = 10000;

//--
void setup() {
  Serial.begin(115200); 
  
  sim.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  Serial.println("SIM800L serial initialize"); 
  delay(500);
  //Restart takes quite some time
  //To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo); 

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
  
  http_client.setHttpResponseTimeout(90 * 1000); //^0 secs timeout
}

void loop() {
  //--
  //Restart takes quite some time
  //To skip it, call init() instead of restart()
  //Serial.println("Initializing modem...");
  //modem.init();
  //String modemInfo = modem.getModemInfo();
  //Serial.print("Modem: ");
  //Serial.println(modemInfo);
  //--
  
  //--
  //Serial.print(F("Waiting for network..."));
  //if (!modem.waitForNetwork()) {
    //Serial.println(" fail");
    //delay(1000);
    //return;
  //}
  //Serial.println(" OK");
  //--
    
  //--
  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  Serial.println(" OK");
  //--
  
  http_client.connect(FIREBASE_HOST, SSL_PORT);
  
  //--
  while (true) {
    if (!http_client.connected()) {
      Serial.println();
      http_client.stop();// Shutdown
      Serial.println("HTTP  not connect");
      break;
    }
    else{
      gps_loop();
    }
  }
  //--
} 

void PostToFirebase(const char* method, const String & path , const String & data, HttpClient* http) {
  String response;
  int statusCode = 0;
  http->connectionKeepAlive(); // Currently, this is needed for HTTPS
  
  //--------
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  
  String sub_path = labNum+"/"+epochTime+"/";
  url += path + sub_path + ".json";
  url += "?auth=" + FIREBASE_AUTH;
  Serial.print("POST:");
  Serial.println(url);
  Serial.print("Data:");
  Serial.println(data);
  
  String contentType = "application/json";
  http->patch(url, contentType, data); 

  // read the status code and body of the response
  //statusCode-200 (OK) | statusCode -3 (TimeOut)
  statusCode = http->responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  response = http->responseBody();
  Serial.print("Response: ");
  Serial.println(response); 

  if (!http->connected()) {
    Serial.println();
    http->stop();// Shutdown
    Serial.println("HTTP POST disconnected");
  } 
} 

void gps_loop()
{   

  //PUT   Write or replace data to a defined path, like messages/users/user1/<data>
  //PATCH   Update some of the keys for a defined path without replacing all of the data.
  //POST  Add to a list of data in our Firebase database. Every time we send a POST request, the Firebase client generates a unique key, like messages/users/<unique-id>/<data>
  //https://firebase.google.com/docs/database/rest/save-data
  
  String username = "123";
  String userreg = "131";
  String rf = "121";

  String fb_log = "{\""+(rf)+"\":{"; 
  fb_log += "\"rf\":\"" + rf + "\",";
  fb_log += "\"timestamp\":\"" + epochTime + "\",";
  fb_log += "\"username\":\"" + username + "\",";
  fb_log += "\"userreg\":\"" + userreg + "\"";
  fb_log += "}}";
  
  PostToFirebase("PATCH", FIREBASE_PATH, fb_log, &http_client);
  
  } 