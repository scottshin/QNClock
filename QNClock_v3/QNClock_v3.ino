/*
  Demonstrates the use of the Parallax WS2812B RGB Led Module (#28085)
  ------------
  Connections:
  LED      Arduino
  GND      GND
  5V       5V
  SI       Digital Pin 6
*/

#include "variable.h"
#include "utils.h"
#include "theme.h"


// data pin
// SCL  A5
// SDA  A4




// A, red button
void swt_afunc_intrupt()
{
    if ( valMode > 0 )
    {
        valMode = 0;
        return;
    }
    
  /*
    SetPixelColor( 8,100, 128,0,0);     // wait
    theme_num = (++theme_num) % 12;
    fillTheme( theme_num );         // Setting Theme
    SetPixelColor( 8,10, 0,0,0);
    EEPROM.write( EEPROM_THEME_ADDR, theme_num );
  */
  /*
    if ( valMode == 1 )     // skip setTime
        isNeedStoreTime = true;
    if ( valMode == 2 )
        isNeedStoreTheme = true;
    if ( valMode == 3 )
        isNeedStoreBright = true;
        valMode = 0;    // exit  Mode
  */
    timeSetMode = (++timeSetMode%3);

   if ( timeSetMode == 2 )      // hour
      isNeedStoreTime = true;
   if ( timeSetMode == 0 )      // minu
      isNeedStoreTime = true;


    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
}


// B,  yellow button
void swt_bfunc_intrupt()
{
    if ( timeSetMode > 0 )
    {
        timeSetMode = 0;
        return;
    }
    

    valMode = ((++valMode) % 3);

    if ( valMode == 2 )
        isNeedStoreTheme = true;
    if ( valMode == 0 )
        isNeedStoreBright = true;

  //valYellow = 0xFF;

    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
    
}

/** 
 *  
 */
int get3231Date()
{
  // send request to receive data starting at register 0
  Wire.beginTransmission(DS3231_I2C_ADDRESS); // 104 is DS3231 device address
  Wire.write(0x00); // start at register 0
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes
  if (Wire.available())
  {
    seconds = Wire.read(); // get seconds
    minutes = Wire.read(); // get minutes
    hours   = Wire.read();   // get hours
    day     = Wire.read();
    date    = Wire.read();
    month   = Wire.read(); //temp month
    year    = Wire.read();

    seconds = (((seconds & B11110000) >> 4) * 10 + (seconds & B00001111)); // convert BCD to decimal
    minutes = (((minutes & B11110000) >> 4) * 10 + (minutes & B00001111)); // convert BCD to decimal
    hours   = (((hours & B00110000) >> 4) * 10 + (hours & B00001111)); // convert BCD to decimal (assume 24 hour mode)
    day     = (day & B00000111); // 1-7
    date    = (((date & B00110000) >> 4) * 10 + (date & B00001111)); // 1-31
    month   = (((month & B00010000) >> 4) * 10 + (month & B00001111)); //msb7 is century overflow
    year    = (((year & B11110000) >> 4) * 10 + (year & B00001111));

  } else {
    //oh noes, no data!67
    return (-1);
  }
/*
  char week_name[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  strcpy(weekDay, week_name[day] );
  Serial.print(weekDay);            Serial.print(", 20");
  Serial.print(year, DEC);          Serial.print("/");
  Serial.print(month, DEC);         Serial.print("/");
  Serial.print(date, DEC);          Serial.print(" - ");
  Serial.print(hours, DEC);         Serial.print(":");
  Serial.print(minutes, DEC);       Serial.print(":");
  Serial.print(seconds, DEC);       Serial.print(" - Temp: ");
  Serial.println(get3231Temp());
*/
  return (1);
}

void setup() 
{
    leds.begin();                       // Initialize 'leds' object
    //Make sure LED is off to demonstrate start of sketch
    initTest();             // Test Boot Animation

    theme_num = EEPROM.read( EEPROM_THEME_ADDR );
    bright_val = EEPROM.read( EEPROM_BRIGHT_ADDR );

    fillTheme( theme_num );         // Setting Theme

    Serial.begin(9600);
    Wire.begin();

    pinMode(SWITCH_A, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SWITCH_A), swt_afunc_intrupt, FALLING);
    pinMode(SWITCH_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SWITCH_B), swt_bfunc_intrupt, FALLING );

    pinMode ( ROT_PIN, INPUT);      //
    Serial.println("--- theme num --");
    Serial.println(theme_num);
}

void setStoreTime()
{
  if ( isNeedStoreTime == true )
  {
    isNeedStoreTime = false;

    year = 18;
    seconds = 0;
    date = 01;
    month = 01;

    Serial.print(" SetTime : ");
    Serial.print(hours, DEC);
    Serial.print(":");
    Serial.print(minutes, DEC);
    Serial.print(":");
    Serial.println(seconds, DEC);

    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(decToBcd(seconds));
    Wire.write(decToBcd(minutes));
    Wire.write(decToBcd(hours));
    Wire.write(decToBcd(day));
    Wire.write(decToBcd(date));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.endTransmission();
  }
}

void setStoreTheme()
{
  if ( isNeedStoreTheme == true )
  {
        isNeedStoreTheme = false;
        fillTheme( theme_num );         // Setting Theme
        EEPROM.write( EEPROM_THEME_ADDR, theme_num );
  }
}
 
//시간설정
// T(설정명령) + 년(00~99) + 월(01~12) + 일(01~31) + 시(00~23) + 분(00~59) + 초(00~59) + 요일(1~7, 일1 월2 화3 수4 목5 금6 토7)
// 예: T1810091300002 (2016년 5월 9일 13시 00분 00초 월요일)
void set3231Date()
{
  year    = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  month   = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  date    = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  hours   = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  minutes = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  seconds = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  day     = (byte) (Serial.read() - 48);

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(decToBcd(seconds));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(date));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();

}

void watchConsole()
{
/*
  if (Serial.available()) {      // Look for char in serial queue and process if found
    if (Serial.read() == 84) {   //If command = "T" Set Date
      set3231Date();
      get3231Date();
      Serial.println(" ");
    }
  }
*/
}


// Loop through changing R, G, and B colors
// Each color fades in from off (i=0) to fully on (i=255)
void loop() {

    setStoreTime();     // if need
    setStoreTheme();      // if need
   if ( (frames++ %50) == 0)
   {
        interval = !interval;
        
 

        if ( 0 > get3231Date() )
        {
           SetPixelColor( 8, 100, 128, 0, 0);
           delay(100);   // 100mS
           return;
        }
   }
  bool isDay = (hours > 6 && hours < 18 );
  bool isMidNight = ( hours > 0 && hours < 5 );
  int calcBrightVal = bright_val;
  if ( isDay == false )
  {
        calcBrightVal -= (int) ( (float)bright_val * (isMidNight ? 0.8 : 0.6) );
  }

  // clear flag
  for ( int i = 0; i < NUM_LEDS; i++)
    leds.setPixelColor(i, 0 );

  int val = analogRead(ROT_PIN);
  if ( abs( val - rot_val) > 10 )           // chetting 
        rot_lock = false;

    
  switch ( timeSetMode )
  {
    case 1 :  // SET TIME MODE  (hour
      {
        Serial.println("--- set time mode ----");
        if ( valYellow <= 0 )
          valYellow = 256;

        if (rot_lock == false)
        {
          int hour =  (int)( (float)(val) / (float)42.6666 );
          hours = hour;
          minutes = 0;
          /*
            int time  = (int) ( (float)val -160 ) / 3 +1;     // 0 ~ 864 using
            if ( 0 > time )
              time = 0;
            {
              Serial.print(time, DEC);
              hours =  time/(12);
              Serial.print(hours, DEC);
              Serial.print(":");
              minutes = (time % (12)) * 5;
              Serial.println(minutes);
            }
          */
        }
      }
      goto display_time;

    case 2 :  // SET TIME MODE  (min)
      Serial.println("--- set time mode ----");
      if ( valYellow <= 0 )
        valYellow = 256;
      if (rot_lock == false)
      {
        int min =  (int)( (float)(val - 64) / 80 );
        minutes = ( min * 5 );
      }
      
      goto display_time;


      case 0:
      default:
            break;
  }


    switch ( valMode )
    {
        case 1 :        // theme
        {
            calcBrightVal = 255;
            Serial.println("--- theme mode ----");
            if ( valBlue <= 0 )
                valBlue = 256;
                
            if ( rot_lock == false )  {
                theme_num  = (int) ( val / 90 );
                fillTheme( theme_num );         // Setting Theme
            }

            for ( int i = 0; i < 25; i++)
                leds.setPixelColor(i, cr_theme[i]  );
        
            
            // color define
            leds.setPixelColor(0, cr_theme[0]  );
            leds.setPixelColor(3, cr_theme[3] );
            int f = 0, d = 0;
            matrix_number_to( theme_num, f, d );
            if ( f )
                leds.setPixelColor(f, cr_theme[f] );
            if ( d > 0 )
                leds.setPixelColor(d, cr_theme[d] );
            leds.setPixelColor(22, cr_theme[22] );        // 시
            leds.setPixelColor(24, cr_theme[24] );        // 분
      }
      goto Bitblt;
      

    case 2:         // Bright
        if ( rot_lock == false )
            bright_val = (val /)4;

        if ( valRed <= 0 )
            valRed = 256;
        break;

    default:
    case 0 :
            //Serial.println("--- running mode----");
            break;
  }




/*
  RELEASE
  watchConsole();
  if ( 0 > get3231Date() )
  {
    SetPixelColor( 8, 100, 128, 0, 0);
    delay(100);   // 100mS
    return;
  }
*/


  printDate2Serial();
  printButtonState2Serial();

  //  leds.setGamma(0.5f);
display_time:

#if 1
{

  // color define DAY 
  leds.setPixelColor(0, isDay ? cr_theme[0] : leds.Color(0,0,0)  );
  leds.setPixelColor(3, isDay ? leds.Color(0, 0, 0) : cr_theme[3]  );

  /**     */

  	int array[3] = { 0, 0, 0};
    matrix_hour_to( hours, array[0], array[1]  );
	for ( int i = 0; i < 3; i++)	{
		if ( array[i] )
	    	leds.setPixelColor(array[i], cr_theme[ array[i] ] );
	}
    leds.setPixelColor(_SI, cr_theme[22] );            // 시

    matrix_min_to( minutes, array[0], array[1], array[2] );
	for ( int i = 0; i < 3; i++)	{
		if ( array[i] )
	    	leds.setPixelColor(array[i], cr_theme[ array[i] ] );
	}
    if ( array[0] || array[1] || array[2] )
        leds.setPixelColor(_BN, cr_theme[24] );            // 분


 
    switch ( timeSetMode )
    {
        case 1: leds.setPixelColor(_SI, (interval==1)?cr_theme[22] : leds.Color(0,0,0)  );            // 시
                break;
        case 2:
                leds.setPixelColor(_BN, (interval==1) ? cr_theme[24] : leds.Color(0,0,0)  );            // 분
                break;
    } 

}
 



Bitblt:




    if ( valYellow > 0 )  {
        leds.setPixelColor(8, leds.Color(valYellow, valYellow, 0) );
        valYellow -= 12;
    }
    if ( valBlue > 0 )
    {
        leds.setPixelColor(8, leds.Color(0, 0, valBlue) );
        valBlue -= 12;
    }
    if ( valRed > 0 )
    {
        leds.setPixelColor(8, leds.Color(valRed, 0, 0) );
        valRed -= 12;
    }

    
#if 1
  //  leds.setBrightness( _gammaTable[val] );
  leds.setBrightness( calcBrightVal);
#endif

    leds.show();
#endif



  delay(10);   // 100mS
}
