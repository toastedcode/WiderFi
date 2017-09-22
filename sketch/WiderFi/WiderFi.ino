#include <FS.h>
#include <Logger.h>
#include <WebServer.h>

#include "ConfigPage.h"
#include "Connection.h"

WebServer webServer(80);

void setup()
{
   Serial.begin(9600);
   SPIFFS.begin();

   Serial.println("*** Wifi Extender Test ***\n");

   Logger::setLogger(new SerialLogger());
   Logger::setLogLevel(DEBUG_FINEST);

   // TODO: Load from properties file.
   Connection::setMode(Connection::MASTER);
   Connection::setApConfig(WifiConfig(Connection::getUniqueId(), ""));
   Connection::setWifiConfig(WifiConfig("TostNet", "t0stn3t5"));
   
   Connection::setup();
   
   webServer.setup();
   webServer.addPage(new ConfigPage());
}

void loop()
{
   Connection::loop();

   webServer.loop();
}
