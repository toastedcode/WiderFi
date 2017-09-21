#include <FS.h>
#include <Logger.h>
#include <WebServer.h>

#include "ConfigPage.h"

WebServer webServer(80);

String ssid;

String password;

void setup()
{
   Serial.begin(9600);
   Serial.println("*** Wifi Extender Test ***\n");

   SPIFFS.begin();

   Logger::setLogger(new SerialLogger());
   Logger::setLogLevel(DEBUG_FINEST);
   
   WiFi.mode(WIFI_AP);

   // Start AP.
   WiFi.softAP("WifiExtender", "");
   
   Serial.print("Started AP at ");
   Serial.println(WiFi.softAPIP().toString().c_str());
   
   webServer.setup();
   webServer.addPage(new ConfigPage());
}

void loop()
{
   webServer.loop();
}
