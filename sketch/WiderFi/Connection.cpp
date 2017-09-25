#include <ESP8266WiFi.h>

#include "Connection.h"
#include "Logger.hpp"

extern "C" void ip_napt_enable(unsigned long addr, int enable);

const int Connection::RETRY_PERIOD = 20000;  // 20 seconds

Mode Connection::mode = SLAVE;

WifiConfig Connection::wifiConfig;

WifiConfig Connection::apConfig;

long Connection::retryTime = 0;

void Connection::setup()
{
   WiFi.mode(WIFI_AP_STA);
   WiFi.disconnect();

   startAp(apConfig.ssid, apConfig.password);
}

void Connection::loop()
{
   if (!isConnected() && (millis() > retryTime))
   {
      if (mode == MASTER)
      {
         connectWifi(wifiConfig.ssid, wifiConfig.password, 10);
      }
      else  // (mode == SLAVE)
      {
         WifiConfig nodeConfig = scanForNodes();
         connectWifi(nodeConfig.ssid, nodeConfig.password, 10);
      }

      retryTime = millis() + RETRY_PERIOD;
   }
}

void Connection::setMode(Mode mode)
{
   Connection::mode = mode;
}

Mode Connection::getMode()
{
   return (Connection::mode);
}

void Connection::setWifiConfig(const WifiConfig& wifiConfig)
{
   Connection::wifiConfig = wifiConfig;
}

const WifiConfig& Connection::getWifiConfig()
{
  return (wifiConfig);
}

void Connection::setApConfig(const WifiConfig& apConfig)
{
   Connection::apConfig = apConfig;
}

const WifiConfig& Connection::getApConfig()
{
  return (apConfig);  
}

bool Connection::connectWifi(const String& ssid, const String& password, int connectTime)
{
   if (ssid != "")
   {
      int secondsToConnect = 0;

      WiFi.begin(ssid.c_str(), password.c_str());
       
      // Wait until the connection is made, or until we timeout.
      while (!isConnected() &&
             (secondsToConnect < connectTime))
      {
         delay(1000);
         secondsToConnect++;
      }

      if (isConnected())
      {
         Logger::logDebug("Connected to wifi network %s at %s.", ssid.c_str(), WiFi.localIP().toString().c_str());
  
         // Activate NAPT (Network address and port translation)
         ip_napt_enable(WiFi.softAPIP(), 1);
  
         Logger::logDebug("Enabled NAPT.");
      } 
      else
      {
         Logger::logDebug("Failed to connect to wifi network %s.", ssid.c_str());
      }
   }

   return (isConnected());
}

bool Connection::startAp(const String& ssid, const String& password)
{
   if (ssid != "")
   {
      WiFi.softAP(ssid.c_str(), password.c_str());
   
      Logger::logDebug("Started %s AP at %s", apConfig.ssid.c_str(), WiFi.softAPIP().toString().c_str());
   }
}

bool Connection::isConnected()
{
  return (WiFi.status() == WL_CONNECTED);
}

String Connection::getUniqueId()
{
   static const String MASTER_PREFIX = "WiderFi_Master";

   static const String SLAVE_PREFIX = "WiderFi";

   String uniqueId = "";
   char buffer[32];

   bool isMaster = Connection::getMode() == MASTER;

   // Retrieve the MAC address of the board.
   unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
   WiFi.macAddress(mac);
  
   // Make an id out of the MAC address.
   sprintf(buffer,
           "%s_%02X%02X",
           isMaster ? MASTER_PREFIX.c_str() : SLAVE_PREFIX.c_str(),
           mac[4],
           mac[5]);

   return (String(buffer));          
}

WifiConfig Connection::scanForNodes()
{
   WifiConfig nodeConfig;

   Logger::logDebug("Checking for nearby Wider-Fi nodes ...");
        
   // Scan for WiderFi hotspots.
   int apCount = WiFi.scanNetworks();

   int bestAp = -1;
   int bestRssi = -1;
   bool foundMaster = false;
  
   for (int i = 0; i < apCount; i++)
   {
      if (WiFi.SSID(i).startsWith("WiderFi"))
      {
         if (WiFi.SSID(i).indexOf("Master") != -1)
         {
            if ((foundMaster == false) ||
                (WiFi.RSSI(i) > bestRssi))
            {
               bestAp = i;
               bestRssi = WiFi.RSSI(i);
               foundMaster = true;
            }
         }
         else if ((foundMaster == false) &&
                  (WiFi.RSSI(i) > bestRssi))
         {
            bestAp = i;
            bestRssi = WiFi.RSSI(i);
         }
      }
   }

   if (bestAp > -1)
   {
      Logger::logDebug(
        "Found Wider-Fi %snode: %s (signal strength = %d)",
        (foundMaster ? "MASTER " : ""),
        WiFi.SSID(bestAp).c_str(),
        bestRssi);
    
      nodeConfig.ssid = WiFi.SSID(bestAp); 
   }
   else
   {
      Logger::logDebug("No available nodes detected.");
   }

   return (nodeConfig);
}
