#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "constants.h"


// debug level: DBG_NONE, DBG_BASIC, DBG_EXTENDED
#define DEBUG DBG_BASIC

// test codes from serial
#define NUM_REP 1      // code repeat number from serial


// OTA params
#define OTA_TIME 30000L
#define OTA_SSID "swcap"
#define OTA_PWD  "1234qwer"
#define OTA_LED  2

// remote type
//#define REMOTE_TYPE JVC
#define REMOTE_TYPE KENWOOD


// pins
#define ANA 36         // Analog IO pin connected to remote keys
#define PIN 16         // Digital IO pin connected to base of transistor


// JVC code parameters
#define JVC_LEN 537    // JVC Length in Microseconds
#define JVC_DEV 0x8F   // JVC device code

// Kenwood code parameters
#define KWD_LEN 562    // Kenwood Length in Microseconds
#define KWD_DEV 0xB9   // Kenwood device code


// key read parameters
#define TICK 10        // Analog read frequency in milliseconds

#define KEY_TMIN 5     // minimum key press length in TICKs
#define KEY_TLNG 50    // long key press time in TICKs
#define KEY_TRPT 52    // repeated key press interval in TICKs

#define KEY_BITS  4    // key value mask width
#define KEY_LONG 16    // key value long press bit
#define KEY_REPT 32    // key value repeated press bit

#define NUM_KEYS 6     // Number of remote keys
// #define KEY_VALS 979, 655, 338, 130, 24, 0 // key split analog values 3300R
#define KEY_VALS 2353, 1644, 1058, 571, 198, 0 // key split analog values 1000R


#endif
