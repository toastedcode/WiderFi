#include "ConfigPage.h"
#include "Connection.h"
#include "WifiConfig.h"

ConfigPage::ConfigPage() :
   Webpage("/config.html",
           "/config.html")
{
   // Nothing to do here.  
}

 bool ConfigPage::handle(
    const HTTPMethod& requestMethod,
    const String& requestUri,
    Argument arguments[],
    const int& numArguments,
    String& responsePath)
{
   //
   // Update wifi config.
   //

   WifiConfig newWifiConfig = Connection::getWifiConfig();

   if (Argument::hasArgument(arguments, numArguments, "wifi.ssid"))
   {
      newWifiConfig.ssid = Argument::getArgument(arguments, numArguments, "wifi.ssid")->getStringValue();
   }

   if (Argument::hasArgument(arguments, numArguments, "wifi.password"))
   {
      newWifiConfig.password = Argument::getArgument(arguments, numArguments, "wifi.password")->getStringValue();
   }

   Connection::setWifiConfig(newWifiConfig);

   return (Webpage::handle(requestMethod, requestUri, arguments, numArguments, responsePath));    
}

void ConfigPage::replaceContent(
   String& content)
{
  content.replace("%ssid", Connection::getWifiConfig().ssid);
  content.replace("%password", Connection::getWifiConfig().password);
  content.replace("%info", "Successfully updated.");
}
