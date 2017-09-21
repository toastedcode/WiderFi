#pragma once

#include "Webpage.hpp"

class ConfigPage : public Webpage
{

public:
  
  ConfigPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      Argument arguments[],
      const int& numArguments,
      String& responsePath);

protected:

  virtual void replaceContent(
     String& content);
};
