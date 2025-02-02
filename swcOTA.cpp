/**
 * 
 * Take care about the esp32 network, 
 * it have to be tagged in private mode to manage OTA on AP mode
 * 
 */

//#include "Arduino.h"
#include "WiFi.h"
#include "ArduinoOTA.h"

#include "swcOTA.h"


String ssid = OTA_SSID;
String password = OTA_PWD;
String hostname = "swcota";


void OTAinit()
{

  ssid.concat(String(ESP.getEfuseMac(), 16));
  hostname.concat(String(ESP.getEfuseMac(), 16));

#if (DEBUG == DBG_BASIC) || (DEBUG == DBG_EXTENDED)
  Serial.printf("SSID = %s\n", ssid.c_str());
  Serial.printf("hostname = %s\n", hostname.c_str());
#endif

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.print("local IP address: ");
  Serial.println(WiFi.softAPIP());

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(hostname.c_str());

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });

  ArduinoOTA.setTimeout(60000);
  ArduinoOTA.begin();

  Serial.println("OTA Ready");
}


void OTAhandle() {
      ArduinoOTA.handle();
}


void OTAend() {
  ArduinoOTA.end();
  WiFi.softAPdisconnect(true);
}


int  OTAclients() {
  return (WiFi.softAPgetStationNum());
}