#pragma once

#include "CommonDefs.hpp"

struct WifiConfig
{
   String ssid;

   String password;

   inline WifiConfig() :
      ssid(""),
      password("")
   {
   }

   inline WifiConfig(
      const String& ssid,
      const String& password) :
      ssid(ssid),
      password(password)
   {
   }

   inline WifiConfig(
      const WifiConfig& wifiConfig)
   {
      ssid = wifiConfig.ssid;
      password = wifiConfig.password;
   }

    inline bool operator==(
      const WifiConfig& rhs)
   {
      return ((ssid == rhs.ssid) &&
              (password == rhs.password));
   }

   inline WifiConfig& operator=(
      const WifiConfig& rhs)
   {
      if (this != &rhs)
      {
         ssid = rhs.ssid;
         password = rhs.password;
      }

      return (*this);
   }
};
