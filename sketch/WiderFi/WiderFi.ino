#include <FS.h>
#include <Logger.h>
#include <Properties.h>
#include <WebServer.h>

#include "ConfigPage.h"
#include "Connection.h"

const String PROPERTIES_FILE = "/widerfi.properties";

WebServer webServer(80);

Properties properties;

void setup()
{
   Serial.begin(9600);
   SPIFFS.begin();

   Serial.println("*** Wifi Extender Test ***\n");

   Logger::setLogger(new SerialLogger());
   Logger::setLogLevel(DEBUG_FINEST);

   // Load properties.
   properties.load(PROPERTIES_FILE);
   Logger::logDebug(F("Properties:"));
   properties.log();

   // Setup our connection manager.
   Connection::setMode(Connection::MASTER);
   Connection::setApConfig(WifiConfig(Connection::getUniqueId(), properties.getString("ap.password")));
   Connection::setWifiConfig(WifiConfig(properties.getString("wifi.password"), properties.getString("wifi.password")));
   Connection::setup();
   
   webServer.setup();
   webServer.addPage(new ConfigPage());
}

void loop()
{
   Connection::loop();

   webServer.loop();
}
