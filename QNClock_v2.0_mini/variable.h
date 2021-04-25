#include <Wire.h>
#include <Adafruit_NeoPixel.h>         // Include Adafruit NeoPixel library

#include <EEPROM.h>


#define NUM_LEDS        25               // Use total of 1 LED
#define MAX_LUX         128
#define DS3231_I2C_ADDRESS (104)

#define LED_PIN            (5)              // LED on digital pin D4

//#define BLUETOOTH_TX
//#define BLUETOOTH_RX

#define SWITCH_A    (7)
#define SWITCH_B    (8)

#define ENC_DT	(2)
#define ENC_CLK	(3)
#define ENC_SW  (4)

#define VERSION     "3.6"

volatile uint32_t cr_theme[NUM_LEDS];


volatile byte seconds = 0, minutes = 0, hours = 12, day = 1, date, month =1, year = 18;
char weekDay[4];

volatile int isNeedStoreTime = 0;
volatile int isNeedStoreTheme = 0;
volatile int isNeedStoreBright = 0;

// Create 'leds' object to drive LEDs
Adafruit_NeoPixel leds[5];


#define EEPROM_THEME_ADDR  (0)
#define EEPROM_BRIGHT_ADDR  (1)

volatile byte theme_num = 0;
volatile byte bright_val = 255;


volatile int modeJung = 0;
volatile int valFade = 0;
/*
volatile int valYellow = 0;  // yellow button
volatile int valBlue = 0; // blue 
volatile int valRed = 0;
*/

volatile int valMode = 0;  // 0 : normal,  1 : setTime,  2: setTheme,  3: setBright
volatile int timeSetMode = 0;   // 0:normal, 1: hour, 2: min 

volatile int interval = 0;
volatile int frames = 0;

// ERROR FLAG



#define _DAY    0
#define _NIG    3
#define _SI     22
#define _BN     24
