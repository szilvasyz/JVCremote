#include "Arduino.h"
#include "sendcode.h"


void SendJVCByte(int b) {
  int i;

  for (i = 0; i < 8; i++) {
    digitalWrite(PIN, HIGH);        // mark
    delayMicroseconds(JVC_LEN * 1);
    digitalWrite(PIN, LOW);        // space
    delayMicroseconds(JVC_LEN * (b & 1 ? 3 : 1));
    
    b >>= 1;
  }
}


void SendJVC(int pre, int cmd, int num) {

  if (pre) {
    digitalWrite(PIN, HIGH);        // AGC
    delayMicroseconds(JVC_LEN * 16);
  }  
  digitalWrite(PIN, LOW);         // AGC
  delayMicroseconds(JVC_LEN * 8);

  cmd |= 0x80;
  for (int c = 0; c < num; c++) {
    SendJVCByte(JVC_DEV);
    SendJVCByte(cmd);
    digitalWrite(PIN, HIGH);        // 17th bit
    delayMicroseconds(JVC_LEN * 1);
    digitalWrite(PIN, LOW);         // inter cmd space
    delayMicroseconds(JVC_LEN * 16);
  }

}


void SendKWDByte(int b) {
  int i;
  int d;

  d = b;
  for (i = 0; i < 8; i++) {
    digitalWrite(PIN, HIGH);        // mark
    delayMicroseconds(KWD_LEN * 1);
    digitalWrite(PIN, LOW);        // space
    delayMicroseconds(KWD_LEN * (d & 1 ? 3 : 1));
    
    d >>= 1;
  }
  
  d = b;
  for (i = 0; i < 8; i++) {
    digitalWrite(PIN, HIGH);        // mark
    delayMicroseconds(KWD_LEN * 1);
    digitalWrite(PIN, LOW);        // space
    delayMicroseconds(KWD_LEN * (d & 1 ? 1 : 3));
    
    d >>= 1;
  }
}


void SendKWD(int pre, int cmd, int num) {

  for (int c = 0; c < num; c++) {
    if (pre) {
      digitalWrite(PIN, HIGH);        // AGC
      delayMicroseconds(KWD_LEN * 16);
 
      digitalWrite(PIN, LOW);         // AGC
      delayMicroseconds(KWD_LEN * 8);

      SendKWDByte(KWD_DEV);
      SendKWDByte(cmd);
    }
    else {
    
      digitalWrite(PIN, HIGH);        // AGC
      delayMicroseconds(KWD_LEN * 16);

      digitalWrite(PIN, LOW);         // AGC
      delayMicroseconds(KWD_LEN * 4);
    }
    
    digitalWrite(PIN, HIGH);        // last bit
    delayMicroseconds(KWD_LEN * 1);
    digitalWrite(PIN, LOW);         // inter cmd space
    delayMicroseconds(KWD_LEN * 16);
    
  }
}
