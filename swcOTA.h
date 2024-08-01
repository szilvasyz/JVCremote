#ifndef _SWCOTA_H_
#define _SWCOTA_H_
/**
 * 
 * Take care about the esp32 network, 
 * it have to be tagged in private mode to manage OTA on AP mode
 * 
 */

#include "config.h"


void OTAinit();
void OTAhandle();
void OTAend();
int  OTAclients();


#endif
