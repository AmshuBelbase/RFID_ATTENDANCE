// RELAY
// GPIO26
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
// ROWS - 15, 2, 0, 4
// COLUMNS - 16, 25, 5, 17  // 4 and 25 swapped 4 for column and 25 for row

//GSM MODULE
// 27-rx1 of esp, 14-tx1 of esp

#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>
#include <unordered_map>
#include <string>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Base64.h> 
#include <NTPClient.h>
#include <WiFiUdp.h>  
#include <Arduino.h>
#include <FirebaseESP32.h>
#include <HardwareSerial.h>
HardwareSerial sim(1); //TXD1 and RXD1 is used
int _timeout;
String _buffer; 

// #include <cstdint>
 
//Firebase
#define FIREBASE_HOST "https://narcs-1805c.firebaseio.com/"
#define FIREBASE_AUTH "IT8P78N9VoPqkOcOGp3PCc6fMsiKpIOmpRStiw1f"
#define API_KEY "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630" 

FirebaseData fbdo; 
bool signupOK = false;

FirebaseConfig config;
FirebaseAuth auth; 


// Time Stamp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
unsigned long epochTime; 
 
//WhatsAPP
String ssid = "Redmi 9C";
String pass = "aarush09"; 

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
int pos = 0;

//RFID
#define RST_PIN 22
#define SS_PIN 21
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

// OTP
String otp = "3928584";
String password = "";  // Variable to store the entered password
String labNum = "";  // Variable to store the entered lab Number
bool otpMode = false;  // Flag to indicate OTP mode
bool labMode = true;  // Flag to indicate OTP mode

//KEYPAD
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 15, 2, 0, 4 };
byte colPins[COLS] = { 16, 25, 5, 17 };

//initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

// Global Variables
String detectedName = "";
String detectedPhone = "";
String detectedRfid = "";
String detectedReg = "";


void setup() {
  Wire.begin(sda, scl);
  Serial.begin(115200);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.println("Current Internet to Connect : "+ssid);
  WiFi.begin(ssid, pass);
  // Serial.println("Connecting to WiFi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  } 
  Serial.println("");
  Serial.println("Connected to WiFi : "+String(ssid)+". IP : "+String(WiFi.localIP()));

  config.api_key = API_KEY;
  config.database_url = FIREBASE_HOST; 
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Sign up Done");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  } 
  Serial.print("signupOK = ");
  Serial.print(signupOK);    
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  timeClient.begin(); 
  _buffer.reserve(50);
  // sim.begin(115200, SERIAL_8N1, 16, 17);
  sim.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  delay(1000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Connected");
  Serial.println("Connected");
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  randomSeed(analogRead(0)); 
  pinMode(26, OUTPUT);
  delay(100);
}

void SendMessage(String recipientPhoneNumber, String message)
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  recipientPhoneNumber = "+"+recipientPhoneNumber;
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + recipientPhoneNumber + "\"\r"); //Mobile phone number to send message
  delay(200); 
  sim.println(message);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
  Serial.println(_buffer);
}

String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

bool get_fb_data(String rf_id) {
  if (Firebase.ready() && signupOK) {
    QueryFilter query;
    query.orderBy("rfid");
    query.startAt(rf_id);
    query.endAt(rf_id);  

    try {
      if (Firebase.getJSON(fbdo, "/rfid/users/", query)) {
        FirebaseJson* json = fbdo.jsonObjectPtr(); 
        size_t len = json->iteratorBegin();
        String keyph, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++) {
          json->iteratorGet(i, type, keyph, value);  
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
          // Serial.println("Sending OTP .. ");  
          json->iteratorEnd();
          return true; 
        }
        json->iteratorEnd();
        detectedRfid = "";
        detectedReg = "";
        detectedName = "";
        detectedPhone = "";
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("RFID : ");
        lcd.setCursor(2, 1);
        lcd.print(rf_id);
        Serial.println(rf_id); 
        delay(7000);   
      } else {
        Serial.println("Error querying Firebase");
        Serial.println(fbdo.errorReason());
        return false;
      }
    } catch (const std::exception& e) {
      // Handle the exception here
      Serial.println("Exception caught: ");
      Serial.println(e.what());
      return false;
      // You can add additional error handling or log messages as needed
    }
  }else {
      Serial.println("FIREBASE NOT READY");
      return false;
  }
}
 

String set_fb_log(String phone) {
  if (Firebase.ready() && signupOK) {
    String rf = detectedRfid;
    String username = detectedName;
    String userreg = detectedReg; 
    timeClient.update();
    epochTime = timeClient.getEpochTime();
    // epochTime = getTime();
    Serial.print("Epoch Time : ");
    Serial.println(epochTime);
    epochTime = epochTime - 1695000000; //1695900000000
    Serial.print("Epoch Time Subtracted: "); 
    Serial.println(epochTime);  
    // epochTime = epochTime*1000;
    // Serial.print("Epoch Time x 1000 : ");
    // Serial.println(epochTime); 
      

    if(epochTime > 0 && rf !="" && userreg!="" && username!=""){
      if(Firebase.setInt(fbdo, "rfid/logs/"+(labNum)+"/"+(epochTime)+"/"+(rf)+"/timestamp", epochTime)){
        Serial.println();
        Serial.print(epochTime);Serial.print(" - saved to : "+fbdo.dataPath());Serial.println(" ("+fbdo.dataType()+ ")");
      }else{
        Serial.println("FAILED: "+fbdo.errorReason());
        return "error";
      }
      if(Firebase.setString(fbdo, "rfid/logs/"+(labNum)+"/"+(epochTime)+"/"+(rf)+"/rf", rf)){
        Serial.println();
        Serial.print(rf);Serial.print(" - saved to : "+fbdo.dataPath());Serial.println(" ("+fbdo.dataType()+ ")");
      }else{
        Serial.println("FAILED: "+fbdo.errorReason());
        return "error";
      }
      if(Firebase.setString(fbdo, "rfid/logs/"+(labNum)+"/"+(epochTime)+"/"+(rf)+"/userreg", userreg)){
        Serial.println();
        Serial.print(userreg);Serial.print(" - saved to : "+fbdo.dataPath());Serial.println(" ("+fbdo.dataType()+ ")");
      }else{
        Serial.println("FAILED: "+fbdo.errorReason());
        return "error";
      } 
      if(Firebase.setString(fbdo, "rfid/logs/"+(labNum)+"/"+(epochTime)+"/"+(rf)+"/username", username)){
        Serial.println();
        Serial.print(username);Serial.print(" - saved to : "+fbdo.dataPath());Serial.println(" ("+fbdo.dataType()+ ")");
      }else{
        Serial.println("FAILED: "+fbdo.errorReason());
        return "error";
      }
      return "logged";
    }else{
      Serial.println("FAILED: Some value is missing !");
      return "error";
    }
  }else{
    Serial.println("FAILED: Firebase Not ready !");
    return "error";
  }
}

void getLabNum(){
  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      labNum = "";
      lcd.clear();
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
    }
  }
  lcd.setCursor(2, 1);
  lcd.print(labNum); 
  // Serial.println(labNum); 
  return ;
}

void loop() { 
  digitalWrite(26, HIGH);
  while(WiFi.status() != WL_CONNECTED){ 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Connecting ..");
    lcd.print("."); 
    delay(300);
  }
  while(labMode){ 
    lcd.setCursor(0, 0);
    lcd.print("Lab Number : ");
    // Serial.println("Lab Number : ");
    getLabNum();  
  }
  if(labNum == ""){
    labNum = "0";
  }
  String enteredOTP = "";
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Scan ID");
  // Serial.println("Scan ID");
  // lcd.setCursor(1, 1);
  int randomNumber = random(1000, 9999);
  otp = String(randomNumber);
  //RFID READING
  if (!rfid.PICC_IsNewCardPresent()){
    // Serial.println("No card");
    delay(100);
    return;
  }
  if (rfid.PICC_ReadCardSerial()) {
    Serial.print("card found");
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    // Serial.println(tag); 
    String phone = "";
    if(get_fb_data(tag)){
      phone = detectedPhone;
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
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Welcome");
      Serial.println("Welcome");
      String logged = set_fb_log(phone);
      if(logged == "logged"){  
        lcd.setCursor(0, 1); 
        Serial.print("Detected user is :");
        Serial.println(detectedName);
        lcd.println(detectedName);
        digitalWrite(26, LOW); // GPIO26 for relay
        delay(5000);
        digitalWrite(26, HIGH); // GPIO26 for relay

      }
      else{
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Could Not Log : ");
        lcd.setCursor(2, 1); 
        Serial.print("Could Not Log : ");
        Serial.println(detectedName);
        lcd.println(detectedName);
      }
      delay(5000);
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

String getinput() {
  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      password = "";
    }
    if (key == 'D') {
      // If 'D' key is pressed, display the entered password and switch to OTP mode
      otpMode = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password: ");
      lcd.setCursor(0, 2);
      lcd.print(password);
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