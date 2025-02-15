
#include <HardwareSerial.h> 
HardwareSerial GSM(1); //TXD1 and RXD1 is used 

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
String concatenatedString = "";

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

void setup()
{
  Serial.begin(9600);
  Serial.println("Wait for 15 seconds");
  delay(15000);
  // GSM.begin(9600);
  GSM.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  delay(2000);  
  sendGSM("AT+SAPBR=3,1,\"APN\",\"portalnmms\"");  
  // sendGSM("AT+SAPBR=0,1",3000);
  sendGSM("AT+SAPBR=1,1",3000);
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
  sendGSM("AT+HTTPPARA=\"URL\",\"http://smartamshu.000webhostapp.com/get_unixtime.php\"");
  sendGSM("AT+HTTPACTION=0"); 
  while(concatenatedString==""){
    Serial.println("Getting Time in setup");
    while(GSM.available()) {
      parseATText(GSM.read());
    }
  }
}

void loop()
{   
  if(concatenatedString!=""){
    Serial.println("Final : "+concatenatedString);
  }
  else{
    Serial.println("Getting Time in loop");
    while(GSM.available()) {
      parseATText(GSM.read());
    }
  }
  delay(5000);
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
      concatenatedString += char(b);
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



// // #include <SoftwareSerial.h>
// // SoftwareSerial GSM(7, 8); // RX, TX
// #include <HardwareSerial.h> 
// HardwareSerial GSM(1); //TXD1 and RXD1 is used 

// enum _parseState {
//   PS_DETECT_MSG_TYPE,

//   PS_IGNORING_COMMAND_ECHO,

//   PS_HTTPACTION_TYPE,
//   PS_HTTPACTION_RESULT,
//   PS_HTTPACTION_LENGTH,

//   PS_HTTPREAD_LENGTH,
//   PS_HTTPREAD_CONTENT
// };

// enum _actionState {
//   AS_IDLE,
//   AS_WAITING_FOR_RESPONSE
// };

// byte actionState = AS_IDLE;
// unsigned long lastActionTime = 0;

// byte parseState = PS_DETECT_MSG_TYPE;
// char buffer[80];
// byte pos = 0;

// int contentLength = 0;
// String concatenatedString = "";


// void resetBuffer() {
//   memset(buffer, 0, sizeof(buffer));
//   pos = 0;
// }

// void sendGSM(const char* msg, int waitMs = 500) {
//   GSM.println(msg);
//   while(GSM.available()) {
//     parseATText(GSM.read());
//   }
//   delay(waitMs);
// }

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("Wait for 15 seconds");
//   delay(15000);
//   // GSM.begin(9600);
//   GSM.begin(115200, SERIAL_8N1, 27, 14); //27-rx of esp, 14-tx of esp
  
//   delay(2000);
//   sendGSM("AT+SAPBR=3,1,\"APN\",\"portalnmms\"");  
//   sendGSM("AT+SAPBR=1,1",3000);
//   sendGSM("AT+HTTPINIT");  
//   sendGSM("AT+HTTPPARA=\"CID\",1");
//   sendGSM("AT+HTTPPARA=\"URL\",\"http://smartamshu.000webhostapp.com/get_unixtime.php\"");
// }

// void loop()
// { 
//   unsigned long now = millis();
//   concatenatedString = "";
//   if ( actionState == AS_IDLE ) {
//     if ( now > lastActionTime + 5000 ) {
//       sendGSM("AT+HTTPACTION=0");
//       lastActionTime = now;
//       actionState = AS_WAITING_FOR_RESPONSE;
//       Serial.println("Final :"+concatenatedString);
//     }
//   }
  
//   while(GSM.available()) {
//     lastActionTime = now;
//     parseATText(GSM.read());
//   }
// }

// void parseATText(byte b) {

//   buffer[pos++] = b;

//   if ( pos >= sizeof(buffer) )
//     resetBuffer(); // just to be safe

//   /*
//    // Detailed debugging
//    Serial.println();
//    Serial.print("state = ");
//    Serial.println(state);
//    Serial.print("b = ");
//    Serial.println(b);
//    Serial.print("pos = ");
//    Serial.println(pos);
//    Serial.print("buffer = ");
//    Serial.println(buffer);*/

//   switch (parseState) {
//   case PS_DETECT_MSG_TYPE: 
//     {
//       if ( b == '\n' )
//         resetBuffer();
//       else {        
//         if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
//           parseState = PS_IGNORING_COMMAND_ECHO;
//         }
//         else if ( b == ':' ) {
//           //Serial.print("Checking message type: ");
//           //Serial.println(buffer);

//           if ( strcmp(buffer, "+HTTPACTION:") == 0 ) {
//             // Serial.println("Received HTTPACTION");
//             parseState = PS_HTTPACTION_TYPE;
//           }
//           else if ( strcmp(buffer, "+HTTPREAD:") == 0 ) {
//             // Serial.println("Received HTTPREAD");            
//             parseState = PS_HTTPREAD_LENGTH;
//           }
//           resetBuffer();
//         }
//       }
//     }
//     break;

//   case PS_IGNORING_COMMAND_ECHO:
//     {
//       if ( b == '\n' ) {
//         Serial.print("Ignoring echo: ");
//         Serial.println(buffer);
//         parseState = PS_DETECT_MSG_TYPE;
//         resetBuffer();
//       }
//     }
//     break;

//   case PS_HTTPACTION_TYPE:
//     {
//       if ( b == ',' ) {
//         // Serial.print("HTTPACTION type is ");
//         // Serial.println(buffer);
//         parseState = PS_HTTPACTION_RESULT;
//         resetBuffer();
//       }
//     }
//     break;

//   case PS_HTTPACTION_RESULT:
//     {
//       if ( b == ',' ) {
//         // Serial.print("HTTPACTION result is ");
//         // Serial.println(buffer);
//         parseState = PS_HTTPACTION_LENGTH;
//         resetBuffer();
//       }
//     }
//     break;

//   case PS_HTTPACTION_LENGTH:
//     {
//       if ( b == '\n' ) {
//         // Serial.print("HTTPACTION length is ");
//         // Serial.println(buffer);
        
//         // now request content
//         GSM.print("AT+HTTPREAD=0,");
//         GSM.println(buffer);
        
//         parseState = PS_DETECT_MSG_TYPE;
//         resetBuffer();
//       }
//     }
//     break;

//   case PS_HTTPREAD_LENGTH:
//     {
//       if ( b == '\n' ) {
//         contentLength = atoi(buffer);
//         // Serial.print("HTTPREAD length is ");
//         // Serial.println(contentLength);
        
//         Serial.print("HTTPREAD content: ");
        
//         parseState = PS_HTTPREAD_CONTENT;
//         resetBuffer();
//       }
//     }
//     break;

//   case PS_HTTPREAD_CONTENT:
//     {
//       // for this demo I'm just showing the content bytes in the serial monitor 
//       Serial.write(b);
//       concatenatedString += char(b);

//       contentLength--;
      
//       if ( contentLength <= 0 ) {

//         // all content bytes have now been read

//         parseState = PS_DETECT_MSG_TYPE;
//         resetBuffer();
        
//         Serial.print("\n\n\n");
        
//         actionState = AS_IDLE;
//       }
//     }
//     break;
//   }
// }