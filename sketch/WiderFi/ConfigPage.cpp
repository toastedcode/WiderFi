#include "ConfigPage.h"

extern String ssid;
extern String password;

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

   if (Argument::hasArgument(arguments, numArguments, "wifi.ssid"))
   {
      ssid = Argument::getArgument(arguments, numArguments, "wifi.ssid")->getStringValue();
   }

   if (Argument::hasArgument(arguments, numArguments, "wifi.password"))
   {
      password = Argument::getArgument(arguments, numArguments, "wifi.password")->getStringValue();
   }

   return (Webpage::handle(requestMethod, requestUri, arguments, numArguments, responsePath));    
}

void ConfigPage::replaceContent(
   String& content)
{
  content.replace("%ssid", ssid);
  content.replace("%password", password);
  content.replace("%info", "Successfully updated.");
}
