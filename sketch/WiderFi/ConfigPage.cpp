#include "ConfigPage.h"
#include "Connection.h"
#include "Properties.hpp"
#include "WiderFiDefs.h"
#include "WifiConfig.h"

extern Properties properties;

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

   Mode newMode = Connection::getMode();

   if (Argument::hasArgument(arguments, numArguments, "mode"))
   {
      newMode = valueOf(Argument::getArgument(arguments, numArguments, "mode")->getStringValue());
   }

   WifiConfig newWifiConfig = Connection::getWifiConfig();

   if (Argument::hasArgument(arguments, numArguments, "wifi.ssid"))
   {
      newWifiConfig.ssid = Argument::getArgument(arguments, numArguments, "wifi.ssid")->getStringValue();
   }

   if (Argument::hasArgument(arguments, numArguments, "wifi.password"))
   {
      newWifiConfig.password = Argument::getArgument(arguments, numArguments, "wifi.password")->getStringValue();
   }

   bool saveProperties = false;

   if (newMode != Connection::getMode())
   {
      Connection::setMode(newMode);
      properties.set("mode", toString(newMode));
      saveProperties = true;
   }

   if (!(newWifiConfig == Connection::getWifiConfig()))
   {
      Connection::setWifiConfig(newWifiConfig);

      // Update the properties file.
      properties.set("wifi.ssid", newWifiConfig.ssid);
      properties.set("wifi.password", newWifiConfig.password);
      saveProperties = true;
   }

   if (saveProperties)
   {
      properties.save();
   }

   return (Webpage::handle(requestMethod, requestUri, arguments, numArguments, responsePath));
}

void ConfigPage::replaceContent(
   String& content)
{
   content.replace("%master_selected", Connection::getMode() == MASTER ? "selected" : "");
   content.replace("%slave_selected", Connection::getMode() == SLAVE ? "selected" : "");
   content.replace("%ssid", Connection::getWifiConfig().ssid);
   content.replace("%password", Connection::getWifiConfig().password);
   content.replace("%info", "Successfully updated.");
}
