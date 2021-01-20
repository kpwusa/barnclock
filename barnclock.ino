#include <WiFi.h>
#include "time.h"
#include <FastLED.h>

const char* ssid       = "
const char* password   = "

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3600;
const int   daylightOffset_sec = 3600;
    
#define NUM_LEDS 58
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

        const int TOTAL_SEGMENTS = 4; // Total amount of segments
        const int LEDS_PER_SEGMENT = 14; // Amount of LEDs per segment
        const int DISPLAY_SEGMENT[] = {0, 14, 14 * 2 + 2, 14 * 3 + 2}; // LED starting position of each segment
        const int DISPLAY_NUMBER[][14] = { // True: Lit, False:  Not lit
          {true, true, false, false, true, true, true, true, true, true, true, true, true, true}, // 0
          {false, false, false, false, false, false, false, false, true, true, true, true, false, false}, // 1
          {true, true, true, true, false, false, true, true, true, true, false, false, true, true}, // 2
          {false, false, true, true, false, false, true, true, true, true, true, true, true, true}, // 3
          {false, false, true, true, true, true, false, false, true, true, true, true, false, false}, // 4
          {false, false, true, true, true, true, true, true, false, false, true, true, true, true}, // 5
          {true, true, true, true, true, true, true, true, false, false, true, true, true, true}, // 6
          {false, false, false, false, false, false, true, true, true, true, true, true, false, false}, // 7
          {true, true, true, true, true, true, true, true, true, true, true, true, true, true}, // 8
          {false, false, true, true, true, true, true, true, true, true, true, true, true, true}, // 9
        };
        
        int red = 255;
        int green = 0;
        int blue = 0;
        char currentFade = 'r';










void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);


            FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
            FastLED.setBrightness(40); // Lower brightness
}

void loop()
{
  delay(1000);
  printLocalTime();
  DisplayLocalTime();

        
}


void DisplayLocalTime()
{
  {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
      return;
  }
  
  int hourFirstDigit = (&timeinfo, "%I") / 10; // Take the first digit
  int hourSecondDigit = (&timeinfo, "%I") % 10; // Take the second digit

  int minuteFirstDigit = (&timeinfo, "%M") / 10; // Take the first digit
  int minuteSecondDigit = (&timeinfo, "%M") % 10; // Take the second digit
  
  int totalDelay = 0;
  while (totalDelay < 10000) { // 10 seconds
    if (currentFade == 'r') { // If red is currently fading
      red--;
      green++;
      currentFade = red <= 0 ? 'g' : 'r'; // Change currentFade if necessary
    } else if (currentFade == 'g') { // If green is currently fading
      green--;
      blue++;
      currentFade = green <= 0 ? 'b' : 'g'; // Change currentFade if necessary
    } else { // If blue is currently fading
      blue--;
      red++;
      currentFade = blue <= 0 ? 'r' : 'b'; // Change currentFade if necessary
    }
    FastLED.clear(); // Clear the LEDs
    displayNumber(3, hourFirstDigit);
    displayNumber(2, hourSecondDigit);
    displayNumber(1, minuteFirstDigit);
    displayNumber(0, minuteSecondDigit);
    leds[14 * 2].setRGB(red, green, blue); // Light the dots
    leds[14 * 2 + 1].setRGB(red, green, blue);
    FastLED.show(); // Show the current LEDs
    delay(10);
    totalDelay += 10;

  
}
}
