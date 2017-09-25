#include "CommonDefs.hpp"
#include "WiderFiDefs.h"

String toString(const Mode& mode)
{
  const String NAMES[] = 
  {
     "MASTER",
     "SLAVE"
  };

  return (NAMES[mode]);
}

Mode valueOf(const String& enumName)
{
  Mode mode = MASTER;

  static const int NUM_NAMES = 2;
  static const String NAMES[] = 
  {
     "MASTER",
     "SLAVE"
  };

  for (int i = 0; i < NUM_NAMES; i++)
  {
     if (enumName == NAMES[i])
     {
        mode = static_cast<Mode>(i);
        break; 
     }
  }

  return (mode);
}

