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
#include "time.h"

#include <avr/sleep.h>

#include <Encoder.h>
#include <Button.h>


#define BAUDRATE 	(9600)
#define MIC_PIN 	(A6)

// data pin
// SCL  A5
// SDA  A4


Encoder	Enc ( ENC_DT, ENC_CLK );
Button 	EncButton ( ENC_SW );
long oldPosition  = -999;
int calcBrightVal = 255;

#if 0
	#define RxD 1
	#define TxD 2	
	#define blueToothSerial Serial3		//the software serial port
#else
	#include <SoftwareSerial.h>   		//Software Serial Port
	#define RxD 2
	#define TxD 3
	SoftwareSerial HM10(RxD,TxD);	//the software serial port 
#endif
char recv_str[100];


// A, red button
void swt_afunc_intrupt()
{
    if ( valMode > 0 )  // B button 동작중이면 a button 무효화
        return;
    
    timeSetMode = (++timeSetMode%3);
    if ( timeSetMode == 2 )   isNeedStoreTime = true;  // hour
    if ( timeSetMode == 0 )   isNeedStoreTime = true;  // Min

}


// B,  yellow button
void swt_bfunc_intrupt()
{
	if ( timeSetMode > 0 )  // A button 동작중이면 B button 무효화
        return;

	valMode = ((++valMode) % 3);
    if ( valMode == 2 )  isNeedStoreTheme = true;
    if ( valMode == 0 )  isNeedStoreBright = true;    
}

void setup() 
{

	for ( int i = 0; i < 5; i++)
 		leds[i]= Adafruit_NeoPixel(5, LED_PIN+i, NEO_GRB + NEO_KHZ800);

	LED_INIT();

	    
    //Make sure LED is off to demonstrate start of sketch
    initTest();             // Test Boot Animation

    theme_num = EEPROM.read( EEPROM_THEME_ADDR );
    theme_num = 0; //ppp
    bright_val = EEPROM.read( EEPROM_BRIGHT_ADDR );

    // teme_num   bright_val   range check. 
    fillTheme( theme_num );         // Setting Theme

    Serial.begin(BAUDRATE);
	HM10.begin(BAUDRATE);    //BT module baud rate


    Wire.begin();

//    pinMode(SWITCH_A, INPUT_PULLUP);
//    attachInterrupt(digitalPinToInterrupt(SWITCH_A), swt_afunc_intrupt, FALLING);
//    pinMode(SWITCH_B, INPUT_PULLUP);
//    attachInterrupt(digitalPinToInterrupt(SWITCH_B), swt_bfunc_intrupt, FALLING );


    pinMode( MIC_PIN, INPUT);
    Serial.println("--- theme num --");
    Serial.println(theme_num);

/*  while(1) {
        if(recvMsg(100) == 0) {
            if(strcmp((char *)recv_str, (char *)"OK+CONN") == 0) .  {
                Serial.println("connected\r\n");
                break;
            }
        }
        delay(200);
    }  */  
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


void SendReport() {
 
  //do something quick, flip a flag, and handle in loop();
   //flag =1 ;
}

void sleepNow() 
{
	Serial.println("==sleepNow==");
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	
	attachInterrupt(0,SendReport, LOW);
	sleep_mode();

	//HERE AFTER WAKING UP
	sleep_disable();
	detachInterrupt(0);
}



// Loop through changing R, G, and B colors
// Each color fades in from off (i=0) to fully on (i=255)
void loop() {

	if (HM10.available()) {
    	Serial.write(HM10.read());
	}
	if (Serial.available()) {
    	HM10.write(Serial.read());
	}

/*	
  int sound = analogRead( MIC_PIN );
  Serial.print( sound );
  Serial.print(",");
  Serial.print(760);
  Serial.print(",");
  Serial.println(0);
  delay(20);
*/

	if (EncButton.toggled()) {
		if (EncButton.read() == Button::PRESSED)
			Serial.println("pressed");
		else
			swt_afunc_intrupt();
	}
	
    setStoreTime();     // if need
    setStoreTheme();      // if need
	if ( (frames++ %150) == 0)
	{
        interval = !interval;
        if ( 0 > get3231Date() )  {
           //SetPixelColor( 8, 100, 128, 0, 0);
           //delay(100);   // 100mS
           return;
        }
	}

	// clear flag
	for ( int i = 0; i < NUM_LEDS; i++)
		LED_SET_PIXEL_COLOR(i, 0 );
	

	switch ( timeSetMode )
	{
		case 1 :  // SET TIME MODE  (hour
		{
			long newPosition = (Enc.read() / 4);
			if (newPosition != oldPosition) {
				oldPosition = newPosition;
				Serial.println(newPosition);
			}
			if ( valFade <= 0 )
				valFade = 256;
			modeJung = 0;
				
			hours = (newPosition %12);
		}
		goto display_time;

		case 2 :  // SET TIME MODE  (min)
		{
			long newPosition = (Enc.read() / 4);
			if (newPosition != oldPosition) {
				oldPosition = newPosition;
				Serial.println(newPosition);
			}
			if ( valFade <= 0 )
				valFade = 256;
			modeJung = 0;
				
			int min = (newPosition %12);
			minutes = ( min * 5 );	
		}
		goto display_time;
		default:
				break;
	}


	switch ( valMode )
    {
        case 1 :        // theme
        {
            calcBrightVal = 255;
            //Serial.println("--- theme mode ----");
            if ( valFade <= 0 )
                valFade = 256;
                modeJung = 1;
                
            {
                //theme_num  = (int) ( val / 70 );		// 12 level 
                fillTheme( theme_num );         // Setting Theme
                Serial.println( theme_num);
            }

            for ( int i = 0; i < 25; i++)
                LED_SET_PIXEL_COLOR(i, cr_theme[i]  );
        
            // color define
			LED_SET_PIXEL_COLOR(0, cr_theme[0]  );
            LED_SET_PIXEL_COLOR(3, cr_theme[3] );
            int f = 0, d = 0;
            matrix_number_to( theme_num, f, d );
			if ( f )
				LED_SET_PIXEL_COLOR(f, cr_theme[f] );
            if ( d > 0 )
				LED_SET_PIXEL_COLOR(d, cr_theme[d] );
			LED_SET_PIXEL_COLOR(22, cr_theme[22] );        // 시
			LED_SET_PIXEL_COLOR(24, cr_theme[24] );        // 분
		}
		break;
      

		case 2:         // Bright
			//Serial.println("--- Bright mode ----");
			if ( valFade <= 0 )  
				valFade = 256;
			modeJung = 1; 
			break;

		default:
		case 0 :
            //Serial.println("--- running mode----");
            break;
	}


	printDate2Serial();
	printButtonState2Serial();

  //  leds.setGamma(0.5f);

  
display_time:
	updateDisplay();
	if ( timeSetMode == 1 || timeSetMode == 2 || valMode ==1 || valMode ==2 ) 
	{
	}
	else
		delay(100);   // 100mS
}



void updateDisplay()
{


	// 오후 6시는 밤,   오전 6시 낮	
	bool isDay = (hours >= 6 && hours < 18 );
	bool isMidNight = ( hours > 0 && hours < 5 );
	if ( theme_num > 11 )
		isMidNight = false;
  
	calcBrightVal = (bright_val);
	if ( isDay == false ) {
		calcBrightVal -= (int) ( (float)bright_val * (isMidNight ? 0.8 : 0.6) );
	}
	
{
    // color define DAY 
    LED_SET_PIXEL_COLOR(0, isDay ? cr_theme[0] : leds[0].Color(0,0,0)  );
    LED_SET_PIXEL_COLOR(3, isDay ? leds[0].Color(0, 0, 0) : cr_theme[3]  );

    /**     */
  	int array[3] = { -1, -1, -1};
    matrix_hour_to( hours, array[0], array[1]  );
    for ( int i = 0; i < 3; i++)	{
		LED_SET_PIXEL_COLOR(array[i], cr_theme[ array[i] ] );
    }
    LED_SET_PIXEL_COLOR(_SI, cr_theme[22] );            				// 시
    matrix_min_to( minutes, array[0], array[1], array[2] );
    for ( int i = 0; i < 3; i++)	{
		LED_SET_PIXEL_COLOR(array[i], cr_theme[ array[i] ] );
    }
    if ( array[0] || array[1] || array[2] )
        LED_SET_PIXEL_COLOR(_BN, cr_theme[24] );            			// 분
 
    switch ( timeSetMode )
    {
        case 1: LED_SET_PIXEL_COLOR(_SI, (interval==1) ? cr_theme[22] : leds[0].Color(0,0,0)  );      // 시
                break;
        case 2: LED_SET_PIXEL_COLOR(_BN, (interval==1) ? cr_theme[24] : leds[0].Color(0,0,0)  );     // 분
                break;
    } 
}

Bitblt:
	INVERT_JUNG( modeJung );
	LED_SHOW( calcBrightVal );
}

void LED_SHOW( int calcBrightVal )
{

  //  leds.setBrightness( _gammaTable[val] );
    for ( int i = 0; i< 5; i++)
    {
    	leds[i].setBrightness( calcBrightVal);
		leds[i].show();   
    }
}

void LED_SET_PIXEL_COLOR( int led, uint32_t cr )
{
	if (led < 0 )
		return;

    leds[led/5].setPixelColor(led%5, cr );
}



void LED_INIT()
{
	for ( int i = 0; i< 5; i++)
		leds[i].begin();                       // Initialize 'leds' object
	    
}
void LED_SHOW()
{
 	for ( int i = 0; i< 5; i++)
		leds[i].show();                 // Set new value
}

//send command to Bluetooth and return if there is a response received
int sendBlueToothCommand(char command[])
{
    Serial.print("send: ");
    Serial.print(command);
    Serial.println("");
#if NLCR
    HM10.println(command);
#else
    HM10.print(command);
#endif    
    delay(300);

    if(recvMsg(200) != 0) return -1;

    Serial.print("recv: ");
    Serial.print(recv_str);
    Serial.println("");
    return 0;
}

//receive message from Bluetooth with time out
int recvMsg(unsigned int timeout)
{
    //wait for feedback
    unsigned int time = 0;
    unsigned char num;


    //waiting for the first character with time out
    unsigned char i = 0;
    while(1)
    {
        delay(50);
        if(HM10.available())
        {
            recv_str[i] = char(HM10.read());
            i++;
            break;
        }
        time++;
        if(time > (timeout / 50)) return -1;
    }

    //read other characters from uart buffer to string
    while(HM10.available() && (i < 100))
    {                                              
        recv_str[i] = char(HM10.read());
        i++;
    }
#if NLCR    
    recv_str[i-2] = '\0';       //discard two character \n\r
#else
    recv_str[i] = '\0';
#endif
    return 0;
}



void initTest()
{

int wait = 1;                         // Standard wait delay of 5 milliseconds 
    for ( int lp =0; lp < 3; lp++ )
    for ( int i = 0; i < 255; i++)
    {
        FillColor( (lp==0?i:0) , (lp==1?i:0), (lp==2?i:0) );
        delay ( 10 );
    }
    ClearScreen();

#if 1  // init test

    for ( int j= 0; j < (NUM_LEDS + 6); j++)
        for(int i=0; i<=MAX_LUX; i = i+1)            // Loop through next color change
        {
            if ( j < NUM_LEDS )
                LED_SET_PIXEL_COLOR(j, leds[0].Color(i,i,i) );
            if ( j >= 4)
            {
                int k = (j-4);
                if ( k < MAX_LUX)
                    LED_SET_PIXEL_COLOR(k, leds[0].Color(MAX_LUX-i, MAX_LUX-i,MAX_LUX-i) );
            }

			LED_SHOW();
            delay( wait);
        }
#endif 
    ClearScreen();
}



void FillColor( int r, int g, int b )
{
    for ( int i = 0; i < NUM_LEDS; i++)
        LED_SET_PIXEL_COLOR( i, leds[0].Color(r, g, b));  // Turn off LED


	LED_SHOW();
}

/*
void SetPixelColor( int inx, int wait, int r, int g, int b )
{
    LED_SET_PIXEL_COLOR(inx, leds[0].Color(r,g,b) );
    for ( int i = 0; i< 5; i++)
		leds[i].show();                  // Set new value
    delay(wait);
}
*/
void ClearScreen()
{
    for ( int i = 0; i< NUM_LEDS; i++)  
            LED_SET_PIXEL_COLOR(i, 0 );
    

	LED_SHOW();

}

void INVERT_JUNG( int mode )
{



	int valYellow = 0;
	int valBlue = 0;
	int valRed = 0;

	if ( mode == 0 )
		valYellow = valFade;
	if ( mode == 1 )
		valBlue = valFade;
	if ( mode == 2 )
		valRed = valFade;		
	
    if ( valYellow > 0 )  {
        LED_SET_PIXEL_COLOR( 8, leds[0].Color(valYellow, valYellow, 0) );
        valYellow -= 12;
    }
    if ( valBlue > 0 )  {
        LED_SET_PIXEL_COLOR( 8, leds[0].Color(0,0,valBlue) );
        valBlue -= 12;
    }
    if ( valRed > 0 )  {
        LED_SET_PIXEL_COLOR( 8, leds[0].Color(valRed,0,0) );
        valRed -= 12;
    }

}
