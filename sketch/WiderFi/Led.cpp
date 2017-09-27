#include "CommonDefs.hpp"
#include "Led.h"
#include "Logger.hpp"
#include "StringUtils.hpp"

static const bool CONTINUE_PATTERN = false;

const int Led::MIN_BRIGHTNESS;

const int Led::MAX_BRIGHTNESS;

const int Led::MIN_PWM;

const int Led::MAX_PWM;

// *****************************************************************************

const int Led::LedPattern::PATTERN_UPDATE_PERIOD = 250;  // milliseconds

const int Led::LedPattern::MAX_PATTERN_LENGTH;

Led::LedPattern::LedPattern(
   Led* led,
   String patternString) :
      led(led),
      patternIndex(0),
      updateTime(0)

{
   setPattern(patternString);
}

Led::LedPattern::~LedPattern()
{
   // Nothing to do here.
}

void Led::LedPattern::setPattern(
   const String& patternString)
{
   memset(patternArray, 0, sizeof(patternArray));
   patternArraySize = patternString.length();

   if (patternArraySize > 0)
   {
      parse(patternString);
      updateTime = 0;
   }
}

void Led::LedPattern::update()
{
   if (patternArraySize > 0)
   {
      long systemTime = millis();

      if (systemTime >= updateTime)
      {
         led->setBrightness(getNext(), CONTINUE_PATTERN);
         updateTime = systemTime + PATTERN_UPDATE_PERIOD;
      }
   }
}

int Led::LedPattern::getNext()
{
   patternIndex++;
   if (patternIndex >= patternArraySize)
   {
      patternIndex = 0;
   }

   return (patternArray[patternIndex]);
}

void Led::LedPattern::parse(String patternString)
{
   for (int i = 0; i < patternArraySize; i++)
   {
      char token = patternString.charAt(i);

      switch (token)
      {
         case '-':
         {
            patternArray[i] = MAX_BRIGHTNESS;
            break;
         }

         case '/':
         {
            int count = 0;
            int j = i;
            do
            {
               j++;
               count++;
            } while ((j < patternString.length()) &&
                     (patternString.charAt(j) == token));

            int delta = (MAX_BRIGHTNESS / (count + 1));

            for (int j = 0; j < count; j++)
            {
               patternArray[i + j] = MIN_BRIGHTNESS + (delta * (j + 1));
            }
            break;
         }

         case '\\':
         {
            int count = 0;
            int j = i;
            do
            {
               j++;
               count++;
            } while ((j < patternString.length()) &&
                     (patternString.charAt(j) == token));

            int delta = (MAX_BRIGHTNESS / (count + 1));

            for (int j = 0; j < count; j++)
            {
               patternArray[i + j] = MAX_BRIGHTNESS - (delta * (j + 1));
            }
            break;
         }

         case '_':
         default:
         {
            patternArray[i] = MIN_BRIGHTNESS;
            break;
         }
      }
   }
}

// *****************************************************************************

Led::LedPulse::LedPulse(
   Led* led,
   const int& period) :
      led(led),
      period(period)
{
}

Led::LedPulse::~LedPulse()
{

}

void Led::LedPulse::setPeriod(
   const int& period)
{
   this->period = period;
}

void Led::LedPulse::update()
{
   if (period > 0)
   {
      long halfPeriod = (period / 2);

      long systemTime = millis();

      long periodTime = (systemTime % period);

      int brightness = 0;

      if (periodTime < halfPeriod)
      {
         // Rising
         brightness = map(periodTime, 0, halfPeriod, Led::MIN_BRIGHTNESS, Led::MAX_BRIGHTNESS);
      }
      else
      {
         // Falling
         brightness = map(periodTime, halfPeriod, period, Led::MAX_BRIGHTNESS, Led::MIN_BRIGHTNESS);
      }

      led->setBrightness(brightness, CONTINUE_PATTERN);
   }
}

// *****************************************************************************

Led::Led(
   const int& pin) :
      pin(pin),
      brightness(0),
      ledPattern(0),
      ledPulse(0)
{
   pinMode(pin, OUTPUT);
   ledPattern = new LedPattern(this, "");
   ledPulse = new LedPulse(this, 0);
}

Led::~Led()
{
   // Nothing to do here.
}

void Led::setBrightness(
   const int& newBrightness,
   const bool& stopPattern)
{
   // Stop any ongoing pattern/pulse.
   if (stopPattern)
   {
      ledPattern->setPattern("");
      ledPulse->setPeriod(0);
   }

   brightness = constrain(newBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

   int pwm = map(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS, MIN_PWM, MAX_PWM);

   analogWrite(pin, pwm);
}

void Led::blink(
   const String& patternString)
{
   int foundPos = StringUtils::findFirstNotOf(patternString, "_-/\\", 0);

   if (foundPos != -1)
   {
      Logger::logWarning(F("Led::blink: Invalid pattern specified [\"%s\"]"), patternString.c_str());
   }
   else
   {
      ledPulse->setPeriod(0);

      ledPattern->setPattern(patternString);
   }
}

void Led::pulse(
   const int& period)
{
   ledPattern->setPattern("");

   ledPulse->setPeriod(period);
}

void Led::loop()
{
   if (ledPattern)
   {
      ledPattern->update();
   }

   if (ledPulse)
   {
      ledPulse->update();
   }
}
