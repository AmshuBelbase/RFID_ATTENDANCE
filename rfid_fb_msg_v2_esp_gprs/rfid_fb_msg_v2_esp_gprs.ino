//RFID PINS
// MOSI-23
// MISO-19
// SDA-21
// SCK-18
// RST-22

//LCD PINS
// SDA-33
// SCL-32

//KEYPAD
// ROWS - 15, 2, 0, 25
// COLUMNS - 16, 4, 5, 17  // 4 and 25 swapped 4 for column and 25 for row

//GSM MODULE
// 27-rx1 of esp, 14-tx1 of esp
 
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256 
#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>
#include <unordered_map>
#include <string>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h> 
// #include <HTTPClient.h>
#include <Base64.h>  
#include <Arduino.h>
// #include <FirebaseESP32.h>
#include <HardwareSerial.h>
#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient  
#include <FirebaseJson.h> 

HardwareSerial GSM(1); //TXD1 and RXD1 is used
int _timeout;
String _buffer; 

// #include <cstdint>
const char FIREBASE_HOST[]  = "narcs-1805c.firebaseio.com";
const String FIREBASE_AUTH  = "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f";   
const String FIREBASE_PATH  = "/rfid/users/";
const String FIREBASE_PATH_LOGS  = "/rfid/logs/";
const int SSL_PORT          = 443;

// #define API_KEY "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630" 

unsigned long previousMillis = 0; 
  
const char* accountSID = "AC7c0cb5e6666910438941e035b896543c";
const char* authToken = "a8935a85c0be0b19952969f911254a78";

char recipientPhoneNumber[20];
char message[200]; 
char choice;
char incomingChar;

//DISPLAY
#define scl 32
#define sda 33
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int flag;
// int pos = 0;

//RFID
#define RST_PIN 22
#define SS_PIN 21
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

// OTP
bool scan_flag = true;
String otp = "3928584";
String password = "";  // Variable to store the entered password
String labNum = "";  // Variable to store the entered lab Number
bool otpMode = false;  // Flag to indicate OTP mode
bool labMode = true;  // Flag to indicate OTP mode
String epochTime = "";
String eTime = "";
long val = 0;
//KEYPAD
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the symbols on the buttons of the keypads
// char keys[ROWS][COLS] = {
//   { '1', '2', '3', 'A' },
//   { '4', '5', '6', 'B' },
//   { '7', '8', '9', 'C' },
//   { '*', '0', '#', 'D' }
// };

char keys[ROWS][COLS] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' },
  { 'A', 'B', 'C', 'D' }
};

byte rowPins[ROWS] = { 5, 17, 16, 4 };
byte colPins[COLS] = { 0, 2, 15, 25 };

//initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

// Global Variables
String detectedName = "";
String detectedPhone = "";
String detectedRfid = "";
String detectedReg = "";

char apn[]  = "portalnmms";
char user[] = "";
char pass[] = "";

enum _parseState {
  PS_DETECT_MSG_TYPE,

  PS_IGNORING_COMMAND_ECHO,

  PS_HTTPACTION_TYPE,
  PS_HTTPACTION_RESULT,
  PS_HTTPACTION_LENGTH,

  PS_HTTPREAD_LENGTH,
  PS_HTTPREAD_CONTENT
};

byte parseState = PS_DETECT_MSG_TYPE;
char buffer[80];
byte pos = 0;

int contentLength = 0;  

TinyGsm modem(GSM);
 
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

void resetBuffer() {
  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}

void sendGSM(const char* msg, int waitMs = 500) {
  GSM.println(msg);
  delay(waitMs);
  while(GSM.available()) {
    parseATText(GSM.read());
  }
}


void parseATText(byte b) {

  buffer[pos++] = b;

  if ( pos >= sizeof(buffer) )
    resetBuffer(); // just to be safe

  /*
   // Detailed debugging
   Serial.println();
   Serial.print("state = ");
   Serial.println(state);
   Serial.print("b = ");
   Serial.println(b);
   Serial.print("pos = ");
   Serial.println(pos);
   Serial.print("buffer = ");
   Serial.println(buffer);*/

  switch (parseState) {
  case PS_DETECT_MSG_TYPE: 
    {
      if ( b == '\n' )
        resetBuffer();
      else {        
        if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
          parseState = PS_IGNORING_COMMAND_ECHO;
        }
        else if ( b == ':' ) {
          //Serial.print("Checking message type: ");
          //Serial.println(buffer);

          if ( strcmp(buffer, "+HTTPACTION:") == 0 ) {
            Serial.println("Received HTTPACTION");
            parseState = PS_HTTPACTION_TYPE;
          }
          else if ( strcmp(buffer, "+HTTPREAD:") == 0 ) {
            Serial.println("Received HTTPREAD");            
            parseState = PS_HTTPREAD_LENGTH;
          }
          resetBuffer();
        }
      }
    }
    break;

  case PS_IGNORING_COMMAND_ECHO:
    {
      if ( b == '\n' ) {
        Serial.print("Ignoring echo: ");
        Serial.println(buffer);
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_TYPE:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION type is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_RESULT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_RESULT:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION result is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_LENGTH;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_LENGTH:
    {
      if ( b == '\n' ) {
        Serial.print("HTTPACTION length is ");
        Serial.println(buffer);
        
        // now request content
        GSM.print("AT+HTTPREAD=0,");
        GSM.println(buffer);
        
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_LENGTH:
    {
      if ( b == '\n' ) {
        contentLength = atoi(buffer);
        Serial.print("HTTPREAD length is ");
        Serial.println(contentLength);
        
        Serial.print("HTTPREAD content: ");
        
        parseState = PS_HTTPREAD_CONTENT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_CONTENT:
    {
      // for this demo I'm just showing the content bytes in the serial monitor
      Serial.write(b);
      epochTime += char(b);
      contentLength--;
      
      if ( contentLength <= 0 ) {

        // all content bytes have now been read

        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;
  }
}


void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight(); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting GPRS"); 
  delay(15000);
  // GSM.begin(9600);
  GSM.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  delay(2000);   
  sendGSM("AT+SAPBR=3,1,\"APN\",\"portalnmms\"");  
  sendGSM("AT+SAPBR=1,1",3000);
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
  // sendGSM("AT+HTTPPARA=\"URL\",\"http://smartamshu.000webhostapp.com/get_unixtime.php\"");
  sendGSM("AT+HTTPPARA=\"URL\",\"http://worldtimeapi.org/api/timezone/Asia/Kolkata\"");
  sendGSM("AT+HTTPACTION=0"); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setting Time");
  Serial.println("Getting Time in setup");
  while(epochTime==""){
    previousMillis = millis();
    while(GSM.available()) {
      parseATText(GSM.read());
    }
  }

  // char buf[150];
  // epochTime.toCharArray(buf, 150);

  char buf[50];
  int length = epochTime.length(); // Get the length of the epochTime string

  // Calculate the starting position to get the last 100 characters
  int startPos = max(0, length - 110); 

  // Copy the last 100 characters (or fewer) to buf
  epochTime.substring(startPos).toCharArray(buf, 50);

  Serial.print("buffer variable:");
  Serial.println(buf);

  String input = String(buf); 
  int startIndex = input.indexOf("unixtime\":") + 10; // 10 is the length of "unixtime\":"
  int endIndex = input.indexOf(",", startIndex);
  String unixTimeString = input.substring(startIndex, endIndex);
  // Serial.println(unixTimeString);
  long unixTime = unixTimeString.toInt();
  val = unixTime;
  Serial.println(val);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(val);
  delay(3000);
  // for(int i=0; i<epochTime.length(); i++){ 
  //   val = val*10 + (buf[i] - '0');
  // }

  // Serial.print("Val");
  // Serial.println(val);

  _buffer.reserve(50);
  delay(1000);
  Serial.println("\nInitializing modem...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing Modem"); 
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  http_client.setHttpResponseTimeout(90 * 1000); 
  pinMode(26, OUTPUT); //relayPin = 26
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Connected");
  Serial.println("Connected");
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  randomSeed(analogRead(0)); 
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lab Number : ");
  while(labMode){ 
    // Serial.println("Lab Number : ");
    getLabNum();  
  }
  if(labNum == ""){
    labNum = "0";
  }
}

void SendMessage(String recipientPhoneNumber, String message)
{
  //Serial.println ("Sending Message");
  GSM.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  recipientPhoneNumber = "+"+recipientPhoneNumber;
  //Serial.println ("Set SMS Number");
  GSM.println("AT+CMGS=\"" + recipientPhoneNumber + "\"\r"); //Mobile phone number to send message
  delay(200); 
  GSM.println(message);
  delay(100);
  GSM.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
  Serial.println(_buffer);
}

String _readSerial() {
  _timeout = 0;
  while  (!GSM.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (GSM.available()) {
    return GSM.readString();
  }
} 
 
bool GetFirebase(const char* method, const String & path ,  HttpClient* http, String rf)
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
  url += "?orderBy=\"rfid\"&equalTo=\""+rf+"\"&auth=" + FIREBASE_AUTH;

  http->get(url);
 
 //statusCode = http->responseStatusCode();
 // Serial.print("Status code: ");
  //Serial.println(statusCode);
  response = http->responseBody();
 
  Serial.print("Response: ");
  Serial.println(response);
  if(response != "" && response != "{}"){ 
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
          detectedRfid = rfid;
        } else {
          Serial.println("No Success - RFID");
          return false;
        }  
      } else {
          Serial.println("Failed to extract RFID value.");
          return false;
      }      
      if (jsonObject.get(usernameData, "username")) {
        if (usernameData.success) {
          const char* usernameChar = usernameData.to<const char*>(); 
          detectedName = usernameChar;
        } else {
          Serial.println("No Success - Username");
          return false;
        }   
      } else {
          Serial.println("Failed to extract Username value.");
          return false;
      }        
      if (jsonObject.get(userregData, "userreg")) {
        if (userregData.success) {
          const char* regChar = userregData.to<const char*>(); 
          detectedReg = regChar;
        } else {
          Serial.println("No Success - Registration");
          return false;
        }    
      } else {
          Serial.println("Failed to extract Username value.");
          return false;
      }
      detectedPhone = keyph;
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Sending OTP .. ");
      if (!http->connected())
      {
        Serial.println();
        http->stop();// Shutdown
        Serial.println("HTTP POST disconnected");
      }
      return true;    
    }
  }
  else{
    detectedRfid = "";
    detectedReg = "";
    detectedName = "";
    detectedPhone = "";
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("RFID : ");
    lcd.setCursor(2, 1);
    lcd.print(rf);
    Serial.println(rf); 
    delay(7000);
    if (!http->connected())
    {
      Serial.println();
      http->stop();// Shutdown
      Serial.println("HTTP POST disconnected");
    }
    return false; 
  } 
}
  
String set_fb_log()
{   

  //PUT   Write or replace data to a defined path, like messages/users/user1/<data>
  //PATCH   Update some of the keys for a defined path without replacing all of the data.
  //POST  Add to a list of data in our Firebase database. Every time we send a POST request, the Firebase client generates a unique key, like messages/users/<unique-id>/<data>
  //https://firebase.google.com/docs/database/rest/save-data
  
  String rf = detectedRfid;
  String username = detectedName;
  String userreg = detectedReg;
 
  val = (millis()/1000) - (previousMillis/1000) + val; 
  char temp[20];
  ltoa(val, temp, 10);  
  epochTime = String(temp);
  previousMillis = millis(); 
  long t_v = val;
  val = val - 1695000000; 
  ltoa(val, temp, 10);
  eTime = String(temp);
  val =t_v;
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("eTime:");
  // lcd.setCursor(0, 1);
  // lcd.print(eTime);
  if(rf !="" && userreg!="" && username!="" && eTime!=""){

    String fb_log = "{\""+(rf)+"\":{"; 
    fb_log += "\"rf\":\"" + rf + "\",";
    fb_log += "\"timestamp\":\"" + eTime + "\",";
    fb_log += "\"username\":\"" + username + "\",";
    fb_log += "\"userreg\":\"" + userreg + "\"";
    fb_log += "}}";
    
    if(PostToFirebase("PATCH", FIREBASE_PATH_LOGS, fb_log, &http_client)){
      return "logged";
    }
    else{
      return "error";
    }
  }
  else{
    return "error";
  }
} 

bool PostToFirebase(const char* method, const String & path , const String & data, HttpClient* http) {
  String response;
  int statusCode = 0;
  http->connectionKeepAlive(); // Currently, this is needed for HTTPS
  
  //--------
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  
  String sub_path = labNum+"/"+eTime+"/";
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
  if(statusCode == 200){
    return true;
  }
  else{
    return false;
  }
} 


void getLabNum(){
  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      labNum = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lab Number : ");
    }
    else if (key == 'D') {
      // If 'D' key is pressed, display the lab number and Move
      labMode = false;  
      lcd.setCursor(2, 1);
      lcd.print(labNum);
      Serial.print("Lab Number : ");
      Serial.println(labNum); 

      return ;
    } else if (labMode) {
      // If in Lab mode, add the key to the labNum
      labNum += key;
      lcd.setCursor(2, 1); 
      lcd.print(labNum); 
    }
  }

  // Serial.println(labNum); 
  return ;
}

void loop() { 
  if(epochTime!=""){
    if (scan_flag == true){
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Scan ID");
    }  
 
    
    //RFID READING
    if (!rfid.PICC_IsNewCardPresent()){
      // Serial.println("No card");
      delay(100);
      scan_flag = false;
      return;
    }
    if (rfid.PICC_ReadCardSerial()) {
      scan_flag = true;
      Serial.print("card found");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card Found");
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }
      // Serial.println(tag); 
      String phone = "";
      
      if(!modem.isGprsConnected()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Connecting APN");
        Serial.print(F("Connecting to "));
        Serial.print(apn);
        if (!modem.gprsConnect(apn, user, pass))
        {
          Serial.println(" fail");
          lcd.setCursor(0, 1);
          lcd.print("Failed");
          delay(1000);
          return;
        }
        Serial.println(" OK"); 
      } 
      http_client.connect(FIREBASE_HOST, SSL_PORT); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Getting Details .. ");
      String enteredOTP = "";
      int randomNumber = random(1000, 9999);
      otp = String(randomNumber);

      if (!http_client.connected())
      {
        Serial.println();
        http_client.stop();// Shutdown
        Serial.println("HTTP  not connect");
        tag = "";
        detectedName = "";
        detectedPhone = "";
        detectedRfid = "";
        detectedReg = "";
        return;
      }
      else{  
        if(GetFirebase("PATCH", FIREBASE_PATH, &http_client, tag)){
          phone = detectedPhone;
        } 
      }
      // phone = get_fb_data(tag); 

      if(phone != ""){
        Serial.println("RFID EXISTS");
        Serial.print("Phone Number: ");
        // Serial.println(phone); 

        //SEND MESSAGE TO THE RFID HOLDER
        String number = phone;
        // Serial.println(number); 
        String m = "Robocon Attendance Pin : "+otp;
        send_msg(number, m);

        // Reset the OTP mode flag
        otpMode = false;

        //TAKING INPUT FROM THE KEYPAD
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Enter Pin:");
        // Serial.print("Enter OTP:");
        lcd.setCursor(1, 1);
        while (!otpMode) {
          enteredOTP = getinput();
          // Serial.println("Entered : "+enteredOTP);
          if (enteredOTP == otp) {
            flag = 1;
          }
          else{
            flag = 2;
          }
        }
        password = "";
        enteredOTP = "";
      } else{
        flag = 0;
      }

      if (flag == 1) {
        flag = 0;
        // String logged = set_fb_log(phone);
        if (!http_client.connected()) {
          Serial.println();
          http_client.stop();// Shutdown
          Serial.println("HTTP  not connect");
          tag = "";
          detectedName = "";
          detectedPhone = "";
          detectedRfid = "";
          detectedReg = "";
          return;
        }
        else{
          String logged = set_fb_log();
          if(logged == "logged"){  
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Welcome");
            Serial.println("Welcome");
            lcd.setCursor(0, 1); 
            Serial.print("Detected user is :");
            Serial.println(detectedName);
            lcd.print(detectedName);
            digitalWrite(26, HIGH);
            //delay(5000);
            // digitalWrite(26, LOW);
          }
          else{
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("Could Not Log : ");
            lcd.setCursor(2, 1); 
            Serial.print("Could Not Log : ");
            Serial.println(detectedName);
            lcd.print(detectedName);
          }
          delay(5000);
          digitalWrite(26, LOW);
        }
      } else if(flag == 2){
        flag = 0;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("WRONG OTP");
        Serial.print("WRONG OTP");
        //lcd.setCursor(1, 1);
        //lcd.print(tag);
        Serial.println(tag);
        delay(2000);
      }
      else{
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Not Registered");
        Serial.print("Not Registered");
        //lcd.setCursor(1, 1);
        //lcd.print(tag);
        Serial.println(tag);
        delay(2000);
      }
      tag = "";
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
    // epochTime = 1695000000000;
    
    tag = "";
    detectedName = "";
    detectedPhone = "";
    detectedRfid = "";
    detectedReg = "";
    // delay(100);
  }
  else{
    while(GSM.available()) {
      // Serial.println("Getting Time");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Getting Time");
      parseATText(GSM.read());
    }
    delay(5000);
  } 
}
 

String getinput() {
  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      password = "";
      lcd.clear(); 
      lcd.setCursor(2, 0);
      lcd.print("Enter Pin:");
      // Serial.print("Enter OTP:");
      lcd.setCursor(1, 1);
    }
    else if (key == 'D') {
      // If 'D' key is pressed, display the entered password and switch to OTP mode
      otpMode = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verifying ..");
      // lcd.setCursor(0, 2);
      // lcd.print(password);
      Serial.print("Password: ");
      Serial.println(password);
      return password;
    } else if (!otpMode) {
      // If not in OTP mode, add the key to the password
      password += key;
    }
  }
  lcd.setCursor(0, 1);
  lcd.print(password);
  // Serial.println(password); 
  return "";
}

void send_msg(String recipientPhoneNumber, String message){  
  Serial.println(recipientPhoneNumber);   
  // Serial.println(message);
  // sendWhatsAppMessage(accountSID, authToken, recipientPhoneNumber, message);
  SendMessage(recipientPhoneNumber, message); 
} 

String urlEncode(const String& value) {
  String encodedValue = "";
  char c;
  char code0;
  char code1;
  
  for (uint8_t i = 0; i < value.length(); i++) {
    c = value.charAt(i);
    if (c == ' ') {
      encodedValue += '+';
    } else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encodedValue += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedValue += '%';
      encodedValue += code0;
      encodedValue += code1;
    }
  }
  return encodedValue;
}