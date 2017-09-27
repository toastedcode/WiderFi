#pragma once

#include "Led.h"
#include "WiderFiDefs.h"
#include "WifiConfig.h"

class Connection
{

public:

   static void setup();

   static void loop();

   static void setMode(Mode mode);

   static Mode getMode();

   static void setWifiConfig(const WifiConfig& wifiConfig);

   static const WifiConfig& getWifiConfig();

   static void setApConfig(const WifiConfig& apConfig);

   static const WifiConfig& getApConfig();

   static bool connectWifi(const String& ssid, const String& password, int connectTime);

   static bool startAp(const String& ssid, const String& password);

   static bool isConnected();

   static String getUniqueId();

   static void setIndicatorLed(Led* led);

  private:

   static const int RETRY_PERIOD;

   static IPAddress getUniqueGateway();

   static WifiConfig scanForNodes();

   static void updateIndicatorLed();

   static Mode mode;

   static WifiConfig wifiConfig;

   static WifiConfig apConfig;

   static Led* led;

   static long retryTime;
};

