/*
  JVC
  Communicate with JVC car radio over 3.5mm Remote connector
  
  Connect Arduino PIN to Base of NPN transistor
  Emitter is tied to ground
  Collector is tied to 3.5mm TIP
  Ground 3.5mm Ring/Sleeve
  
  By Dan Guerra (deguerra98@yahoo.com)
  */

#include "config.h"
#include "swcOTA.h"
#include "anakey.h"
#include "sendcode.h"

  
int IncomingByte = 0; // Initialize Serial Buffer
int AnaValue = 0;     // Analog value from remoke keys
int DecKey = 0;       // Decoded Key number
int ModKey = 0;       // Decoded Key modifier

unsigned long CurrTime, PrevTime;
int OtaActive;


//
// JVC type remote
//
// Keys
// 5: Vol-  
// 4: Vol+  
// 3: Seek+ 
// 2: Seek- 
// 1: Mode  
//
// NEC device code: 0x8F
// NEC commands:
// 4: volume up
// 5: volume down
// 8: source
// 13: sound (equalizer)
// 14: mute
// 18: radio seek up / pick up call / CD/USB next track
// 19: radio seek down / pick up call / CD/USB prev track
// 20: radio next / pick up call / USB next folder
// 21: radio prev / pick up call / USB prev folder
// 25: drop call
//

//
// Kenwood type remote
//
// Device code: 0xb9
// Command codes:
// 0           0x00  
// 1           0x01  
// 2           0x02  
// 3           0x03  
// 4           0x04  
// 5           0x05  
// 6           0x06  
// 7           0x07  
// 8           0x08  
// 9           0x09  
// Track-      0x0a    
// Track+      0x0b    
// Rew         0x0c    
// Ff          0x0d    
// Play/Pause  0x0e
//
// Source      0x13
// Volume+     0x14
// Volume-     0x15
// Mute        0x16
//
// Tuner       0x1c  
// Tape        0x1d  
// CD          0x1e  
// CD-MD-CH    0x1f  

// DNPP        0x5e    


#if REMOTE_TYPE == JVC

  #define CODE_NORM {0,  8, 19, 18, 4,  5}
  #define CODE_LONG {0, 25, 21, 20, 4, 14}
  #define CODE_REPT {0,  0,  0,  0, 4,  0}

  #define FULL_NORM {0, 1, 1, 1, 1, 1}
  #define FULL_LONG {0, 1, 1, 1, 1, 1}
  #define FULL_REPT {0, 0, 0, 0, 0, 0}

  #define NUMS_NORM {0, 2, 2, 2, 2, 2}
  #define NUMS_LONG {0, 2, 2, 2, 2, 2}
  #define NUMS_REPT {0, 0, 0, 0, 2, 0}

#elif REMOTE_TYPE == KENWOOD

  #define CODE_NORM {0, 0x13, 0x0A, 0x0B, 0x14, 0x15}
  #define CODE_LONG {0, 0x16, 0x0A, 0x0B, 0x0D, 0x0C}
  #define CODE_REPT {0, 0x00, 0x0A, 0x0B, 0x00, 0x00}
  
  #define FULL_NORM {0, 1, 1, 1, 1, 1}
  #define FULL_LONG {0, 1, 1, 1, 1, 1}
  #define FULL_REPT {0, 0, 0, 0, 0, 0}
  
  #define NUMS_NORM {0, 1, 1, 1, 1, 1}
  #define NUMS_LONG {0, 1, 1, 1, 1, 1}
  #define NUMS_REPT {0, 0, 1, 1, 0, 0}

#else
  #error "Unknown REMOTE_TYPE"
#endif

int KeyCodes [3][NUM_KEYS] = { CODE_NORM, CODE_LONG, CODE_REPT };
int KeyPreamb[3][NUM_KEYS] = { FULL_NORM, FULL_LONG, FULL_REPT };
int KeySample[3][NUM_KEYS] = { NUMS_NORM, NUMS_LONG, NUMS_REPT };


void SendCode(int pre, int cmd, int num) {

  if (num == 0)
    return;
    
#if REMOTE_TYPE == JVC
  SendJVC(pre, cmd, num);
#elif REMOTE_TYPE == KENWOOD
  SendKWD(pre, cmd, num);
#else
  #error "Unknown REMOTE_TYPE"
#endif
}


void setup() {

  pinMode(PIN, OUTPUT); // Set pin to output
  digitalWrite(PIN, LOW); // Make PIN low to shut off transistor

  pinMode(ANA, INPUT); // Set analog pin to input
  pinMode(OTA_LED, OUTPUT); // Set OTA LED to output

  Serial.begin(115200);
  Serial.println("Pins ready.");

  OTAinit();
  OtaActive = 1;
  digitalWrite(OTA_LED, HIGH);

  PrevTime = millis();
}


void loop() {
  
  CurrTime = millis();

  if (CurrTime - PrevTime > TICK) {

    PrevTime = CurrTime;
    DecKey = GetKey(AnaValue = analogRead(ANA));

    if (DecKey) {
      ModKey = DecKey >> KEY_BITS;
      DecKey = DecKey & ((1 << KEY_BITS) - 1);
#if (DEBUG == DBG_BASIC) || (DEBUG == DBG_EXTENDED)
      Serial.printf("%5d - %3d:%2d\n", AnaValue, DecKey, ModKey);
#endif
      SendCode(
        KeyPreamb[ModKey][DecKey],
        KeyCodes[ModKey][DecKey],
        KeySample[ModKey][DecKey]
      );
    }
    
  }
  
  if (OtaActive) {

    OTAhandle();

    if ((millis() > OTA_TIME) && (OTAclients() == 0)) {
      OtaActive = 0;
      OTAend();
      digitalWrite(OTA_LED, LOW);
      Serial.println("Closing OTA.");
    }
  }

  if (Serial.available()) {
    //IncomingByte = Serial.read();
    //Serial.print(IncomingByte);
    
    IncomingByte = Serial.parseInt();
    while(Serial.available()) Serial.read();
    Serial.print("=");
    Serial.println(IncomingByte);
    SendCode(1, IncomingByte, NUM_REP);  
  }
}
