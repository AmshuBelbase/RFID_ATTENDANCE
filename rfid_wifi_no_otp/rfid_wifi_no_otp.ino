// https://codetoflow.com/
// RELAY
// GPIO26
// RFID PINS
// MOSI-23
// MISO-19
// SDA-21
// SCK-18
// RST-22

// LCD PINS
//  SDA-33
//  SCL-32

// KEYPAD
//  ROWS - 15, 2, 0, 4
//  COLUMNS - 16, 25, 5, 17  // 4 and 25 swapped 4 for column and 25 for row

// GSM MODULE
//  27-rx1 of esp, 14-tx1 of esp

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

// Firebase
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
unsigned long epochTime = 0;

// WIFI
String ssid = "TP-Link_1E4A";     //"PROJECT_EVA"; //
String pass = "81095391"; //"@SRMTEAMROBOCON";

// DISPLAY
#define scl 32
#define sda 33
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int flag;
int pos = 0;

// RFID
#define RST_PIN 22
#define SS_PIN 21
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

String labNum = "604"; // Variable to store the entered lab Number
bool labMode = false;  // Flag to indicate OTP mode

// KEYPAD
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the symbols on the buttons of the keypads
// char keys[ROWS][COLS] = {
//     {'1', '4', '7', '*'},
//     {'2', '5', '8', '0'},
//     {'3', '6', '9', '#'},
//     {'A', 'B', 'C', 'D'}};

char keys[ROWS][COLS] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}};

byte rowPins[ROWS] = {5, 17, 16, 4};
byte colPins[COLS] = {0, 2, 15, 25};

// initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Global Variables
String detectedName = "";
String detectedPhone = "";
String detectedRfid = "";
String detectedReg = "";

bool scan_flag = true;
bool change_wifi = false;

unsigned long startTime;
unsigned long elapsedTime;

// force open the door using manually set password
void force_open(){
  bool can_display = false;
  String enpass = "a";
  while (true){
    char key = keypad.getKey();
    if (key)
    {
      if (key == 'C' || key == 'B')
      {
        can_display = true;
        enpass = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password:");
        if (key == 'B')
        {
          break;
        }
      }
      else if (key == 'D')
      {
        lcd.setCursor(2, 1);
        lcd.print(enpass);
        Serial.print("Password : ");
        Serial.println(enpass);
        break;
      }
      else if(can_display)
      {
        enpass += key;
        lcd.setCursor(2, 1);
        lcd.print(enpass);
      }else{}
    }
  }
  if (enpass == "20040622")
  {
    lcd.setCursor(2, 1);
    lcd.print("Force Open");
    digitalWrite(26, HIGH); // GPIO26 for relay
    delay(2000);
  }
}

// check if wifi is connected, if not try to continuosly connect, also give option to force open (1C) and change wifi details (A)
void con_if_not(){
  scan_flag = true;
  while (WiFi.status() != WL_CONNECTED)
  {
    if(scan_flag){
      scan_flag = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Connecting Wifi ..");
      lcd.setCursor(0, 1);
      lcd.print("A | Change");
    } 
    char yes_no_key = keypad.getKey();
    if (yes_no_key)
    {
      // Force Open the DOOR
      if(yes_no_key == '1'){ 
        force_open();
      }
      // Change Wifi Details
      if (yes_no_key == 'A')
      {
        ssid = "";
        pass = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ENTER SSID:");
        while (true)
        {
          char key = keypad.getKey();
          if (key)
          {
            if (key == 'C')
            {
              ssid = "";
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Enter SSID: ");
            }
            else if (key == 'D')
            {
              lcd.setCursor(2, 1);
              lcd.print(ssid);
              Serial.print("SSID : ");
              Serial.println(ssid);
              break;
            }
            else
            {
              ssid += key;
              lcd.setCursor(2, 1);
              lcd.print(ssid);
            }
          }
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password:");
        while (true)
        {
          char key = keypad.getKey();
          if (key)
          {
            if (key == 'C')
            {
              pass = "";
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Enter Password:");
            }
            else if (key == 'D')
            {
              lcd.setCursor(2, 1);
              lcd.print(pass);
              Serial.print("Password : ");
              Serial.println(pass);
              break;
            }
            else
            {
              pass += key;
              lcd.setCursor(2, 1);
              lcd.print(pass);
            }
          }
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wifi:");
        lcd.setCursor(5, 0);
        lcd.print(ssid);
        WiFi.begin(ssid, pass);
        delay(1000); 
      }
      scan_flag = true;
    } 
    delay(100);
  }
}


// Fetch the user details by RFID tag from Firebase
bool get_fb_data(String rf_id){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Fetching User");

  if (Firebase.ready() && signupOK)
  {
    QueryFilter query;
    query.orderBy("rfid");
    query.startAt(rf_id);
    query.endAt(rf_id);

    try
    {
      if (Firebase.getJSON(fbdo, "/rfid/users/", query))
      {
        FirebaseJson *json = fbdo.jsonObjectPtr();
        size_t len = json->iteratorBegin();
        String keyph, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++)
        {
          json->iteratorGet(i, type, keyph, value);
          // Parse JSON
          FirebaseJson jsonObject;
          jsonObject.setJsonData(value);
          FirebaseJsonData rfidData;
          FirebaseJsonData usernameData;
          FirebaseJsonData userregData;

          if (jsonObject.get(rfidData, "rfid"))
          {
            if (rfidData.success)
            {
              const char *rfid = rfidData.to<const char *>();
              detectedRfid = rfid;
            }
            else
            {
              Serial.println("No Success - RFID");
              return false;
            }
          }
          else
          {
            Serial.println("Failed to extract RFID value.");
            return false;
          }
          if (jsonObject.get(usernameData, "username"))
          {
            if (usernameData.success)
            {
              const char *usernameChar = usernameData.to<const char *>();
              detectedName = usernameChar;
            }
            else
            {
              Serial.println("No Success - Username");
              return false;
            }
          }
          else
          {
            Serial.println("Failed to extract Username value.");
            return false;
          }
          if (jsonObject.get(userregData, "userreg"))
          {
            if (userregData.success)
            {
              const char *regChar = userregData.to<const char *>();
              detectedReg = regChar;
            }
            else
            {
              Serial.println("No Success - Registration");
              return false;
            }
          }
          else
          {
            Serial.println("Failed to extract Username value.");
            return false;
          }
          detectedPhone = keyph;
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
      }
      else
      {
        Serial.println("Error querying Firebase");
        Serial.println(fbdo.errorReason());
        return false;
      }
    }
    catch (const std::exception &e)
    {
      // Handle the exception here
      Serial.println("Exception caught: ");
      Serial.println(e.what());
      return false;
      // You can add additional error handling or log messages as needed
    }
  }
  else
  {
    Serial.println("FIREBASE NOT READY");
    return false;
  }
}

// Save the user attendance log details to Firebase
String set_fb_log(String phone){
  if (Firebase.ready() && signupOK){
    String rf = detectedRfid;
    String username = detectedName;
    String userreg = detectedReg;

    elapsedTime = (millis() - startTime) / 1000; // Calculate the elapsed time

    epochTime = epochTime + elapsedTime;
    startTime = millis();
    Serial.print("Epoch Time: ");
    Serial.println(epochTime);

    // epochTime = epochTime*1000;
    // Serial.print("Epoch Time x 1000 : ");
    // Serial.println(epochTime);

    if (epochTime > 0 && rf != "" && userreg != "" && username != "")
    {
      if (Firebase.setInt(fbdo, "rfid/logs/" + (labNum) + "/" + (epochTime) + "/" + (rf) + "/timestamp", epochTime))
      {
        Serial.println();
        Serial.print(epochTime);
        Serial.print(" - saved to : " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
      else
      {
        Serial.println("FAILED: " + fbdo.errorReason());
        return "error";
      }
      if (Firebase.setString(fbdo, "rfid/logs/" + (labNum) + "/" + (epochTime) + "/" + (rf) + "/rf", rf))
      {
        Serial.println();
        Serial.print(rf);
        Serial.print(" - saved to : " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
      else
      {
        Serial.println("FAILED: " + fbdo.errorReason());
        return "error";
      }
      if (Firebase.setString(fbdo, "rfid/logs/" + (labNum) + "/" + (epochTime) + "/" + (rf) + "/userreg", userreg))
      {
        Serial.println();
        Serial.print(userreg);
        Serial.print(" - saved to : " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
      else
      {
        Serial.println("FAILED: " + fbdo.errorReason());
        return "error";
      }
      if (Firebase.setString(fbdo, "rfid/logs/" + (labNum) + "/" + (epochTime) + "/" + (rf) + "/username", username))
      {
        Serial.println();
        Serial.print(username);
        Serial.print(" - saved to : " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
      else
      {
        Serial.println("FAILED: " + fbdo.errorReason());
        return "error";
      }
      return "logged";
    }
    else
    {
      Serial.println("FAILED: Some value is missing !");
      return "error";
    }
  }
  else
  {
    Serial.println("FAILED: Firebase Not ready !");
    return "error";
  }
}

// Function to get Lab Number from keypad
void getLabNum(){
  char key = keypad.getKey();
  if (key)
  {
    if (key == 'C')
    {
      labNum = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lab Number : ");
    }
    else if (key == 'D')
    {
      // If 'D' key is pressed, display the lab number and Move
      labMode = false;
      lcd.setCursor(2, 1);
      lcd.print(labNum);
      Serial.print("Lab Number : ");
      Serial.println(labNum);

      return;
    }
    else if (labMode)
    {
      // If in Lab mode, add the key to the labNum
      labNum += key;
      lcd.setCursor(2, 1);
      lcd.print(labNum);
    }
  }
  return;
}

// Setup
void setup(){
  // initialize serial, wire and lcd
  Wire.begin(sda, scl);
  Serial.begin(115200);
  pinMode(26, OUTPUT); // Relay
  lcd.init();
  lcd.clear();
  lcd.backlight(); 

  // Connect to wifi
  Serial.println("Current Internet to Connect : " + ssid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wifi:");
  lcd.setCursor(5, 0);
  lcd.print(ssid);
  WiFi.begin(ssid, pass); 

  // check if wifi is connected, if not try to continuosly connect, also give option to force open (1C) and change wifi details (A)
  con_if_not(); 

  // move on once conencted to wifi
  Serial.println("");
  Serial.println("Connected to WiFi : " + String(ssid) + ". IP : " + String(WiFi.localIP()));

  // setup and connect firebase
  config.api_key = API_KEY;
  config.database_url = FIREBASE_HOST;
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Sign up Done");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  Serial.print("signupOK = ");
  Serial.print(signupOK);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  timeClient.begin();
  delay(1000);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Connected");
  Serial.println("Connected");

  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  delay(100);

  // Can allow to enter lab number manually if needed
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lab Number : ");
  while (labMode)
  {
    getLabNum();
  }
  if (labNum == "")
  {
    labNum = "0";
  }

  // Start from count 1 and try to get current time, max try count = 10, restart if not able to fetch time
  int count = 1;
  while ((epochTime < 1000000000 || epochTime > 9999999999) && count <= 10)
  {
    timeClient.update();
    epochTime = timeClient.getEpochTime();
    startTime = millis();
    // epochTime = getTime();
    Serial.print("Count : ");
    Serial.print(count);
    Serial.print(" Epoch Time : ");
    Serial.println(epochTime);
    lcd.setCursor(0, 0);
    lcd.print("Fetching Date!");
    lcd.setCursor(0, 1);
    lcd.print("Count : ");
    lcd.setCursor(8, 1);
    lcd.print(count);
    count++;
    if (count == 10)
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Timed Out: Restarting");
      Serial.print(" Restarting ..");
      delay(2000);
      ESP.restart(); // Restart the ESP32
    }
  }
  epochTime = epochTime - 1695000000; // 1695900000000
  Serial.print("Epoch Time Subtracted: ");
  Serial.println(epochTime);

  scan_flag = true; // allow lcd display while going to loop
}

// Loop
void loop(){
  // Allow Force Open the DOOR (Even If Connected) - Enter 1C 
  char key = keypad.getKey();
  if (key)
  {
    if(key == '1'){ 
      force_open();
    }
  }

  // Relay state 
  digitalWrite(26, LOW);

  // check if wifi is connected, if not try to continuosly connect, also give option to force open (1C) and change wifi details (A)
  con_if_not();

  // clear and display  only when lcd state changes
  if (scan_flag == true)
  {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Scan ID");
  }

  // Check if ID card is tapped, if tapped move to next else return to start of loop
  if (!rfid.PICC_IsNewCardPresent())
  {
    // Serial.println("No card");
    delay(100);
    scan_flag = false;
    return;
  }

  // If ID card is found
  if (rfid.PICC_ReadCardSerial())
  {
    scan_flag = true;
    Serial.print("card found");

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Tag Found");

    for (byte i = 0; i < 4; i++)
    {
      tag += rfid.uid.uidByte[i];
    }
    // Serial.println(tag);
    String phone = "";

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("ID Found");

    if (get_fb_data(tag))
    {
      phone = detectedPhone;
    }
    // phone = get_fb_data(tag);

    // Check if phone number found from RFID detail
    if (phone != "")
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("RFID EXISTS");
      Serial.println("RFID EXISTS");
      Serial.print("Phone Number: ");
      // Serial.println(phone);
      flag = 1;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("NO SUCH RFID");
      flag = 0;
    }

    // If phone number is found from RFID detail
    if (flag == 1)
    {
      flag = 0;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Welcome");
      Serial.println("Welcome");

      // Save Log details in Firebase
      String logged = set_fb_log(phone);

      // Check if log is saved successfully
      if (logged == "logged")
      {
        lcd.setCursor(0, 1);
        Serial.print("Detected user is :");
        Serial.println(detectedName);
        lcd.println(detectedName);
        digitalWrite(26, HIGH); // GPIO26 for relay 
      }
      else
      {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Could Not Log : ");
        lcd.setCursor(2, 1);
        Serial.print("Could Not Log : ");
        Serial.println(detectedName);
        lcd.println(detectedName);
      }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Not Registered");
      Serial.print("Not Registered");
      lcd.setCursor(1, 1);
      lcd.print(tag);
      Serial.println(tag); 
    }
    delay(2000);
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  } 
  tag = "";
  detectedName = "";
  detectedPhone = "";
  detectedRfid = "";
  detectedReg = ""; 
}