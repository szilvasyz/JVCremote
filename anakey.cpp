#include "Arduino.h"
#include "anakey.h"

static int LastKey = 0;
static int KeyCnt = 0;

int KeySplitVals[NUM_KEYS] = {
  KEY_VALS 
};

int GetKey(int AnaVal) {
  int key;
  int retval = 0;

  key = 0; 
  while (AnaVal < KeySplitVals[key]) key++;

#if DEBUG == DBG_EXTENDED
  Serial.printf("A:%5d\n", AnaVal);
#endif


  if (key == LastKey) {
    if (key) {
      KeyCnt++;
      if (KeyCnt == KEY_TLNG) 
        retval = KEY_LONG | key;
      if (KeyCnt == KEY_TRPT) {
        retval = KEY_REPT | key;
        KeyCnt = KEY_TLNG;
      }
    }
  }
  else {
    if ((KeyCnt >= KEY_TMIN) && (KeyCnt < KEY_TLNG)) {
      retval = LastKey;
    }
    KeyCnt = 0;
  }

  LastKey = key;
  return retval;
}
