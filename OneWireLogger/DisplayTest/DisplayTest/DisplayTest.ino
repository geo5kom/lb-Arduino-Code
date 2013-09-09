//////////////////////////////////////////////////////////////////////////////
//  DisplayTest - Test the TFT LCD Display on the One-Wire Data Logger Board.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Includes follow
//////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <EEPROM.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
#include <OneWireLogger.h>
#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_ST7735.h>   // Hardware-specific library
#include <EEPROMAnything.h>    // EEPROM anything

//////////////////////////////////////////////////////////////////////////////
//#define SERIAL_OUT
//////////////////////////////////////////////////////////////////////////////

//#define SERIAL_OUT
#undef SERIAL_OUT

enum MENUITEMS
{
  LOGGER_MENU,
  LOG2SD_MENU,
  LOADSTOR_MENU,
  SDCARD_MENU,
  MANTIME_MENU,
  BACKLITE_MENU,
  TSTKPD_MENU,
  APPENDSD_MENU,
  CREATENEW_MENU,
  NEWFILE_MENU,
  LOAD_MENU,
  STORE_MENU,
  DISPTIME_MENU,
  SETTIME_MENU,
  SDERASE_MENU,
  SDLIST_MENU,
  SDEN_MENU,
  DSDIS_MENU,
};

//////////////////////////////////////////////////////////////////////////////
// Global variables follow
//////////////////////////////////////////////////////////////////////////////

int menuState;  // Used to implement the menuing state machine

//////////////////////////////////////////////////////////////////////////////
// Board Configuration stored in EEPROM
//////////////////////////////////////////////////////////////////////////////

struct IZ_Cfgs
{
  int bll;       // Backlight level
  int enableSD;  // Enable the SD card
} 
IZConfigs;

//////////////////////////////////////////////////////////////////////////////
// class initializers
//////////////////////////////////////////////////////////////////////////////

Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DC, LCD_RST);
OneWireLogger myOneWireLogger;

//////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
//////////////////////////////////////////////////////////////////////////////

void setup() 
{
  EEPROM_readAnything(0, IZConfigs);

  menuState = LOGGER_MENU;

  analogWrite(BACKLIGHT, IZConfigs.bll);
  tft.initR(INITR_REDTAB);
  tft.setTextSize(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  textWhiteOnBlack();
  if (IZConfigs.enableSD != 0)
  {
    if (!SD.begin(SD_CS)) 
    {
      tft.print(F("SD card not present"));
      setCursorTFT(1, 0);
      tft.print(F("Disabling check"));
      setCursorTFT(2, 0);
      tft.print(F("Power cycle logger"));
      IZConfigs.enableSD = 0;
      EEPROM_writeAnything(0, IZConfigs);
      delay(2000);
    }
  }

  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning() )
  {
    tft.print("RTC is NOT running!");
    delay(1000);
  }
}

//////////////////////////////////////////////////////////////////////////////////////
// loop - runs over and over again forever:
//////////////////////////////////////////////////////////////////////////////////////

void loop() 
{

  menuRefresh();
  menuNav();
}

//////////////////////////////////////////////////////////////////////////////////////
// setCursorTFT(int row, int col)
//////////////////////////////////////////////////////////////////////////////////////

void setCursorTFT(int row, int col)
{
  tft.setCursor(col*6, row*10);
}

