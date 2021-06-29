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

#define BAUDRATE	(9600)
#define MIDNIGHT 	(1)
#define BLUETOOTH 	(0)


//#define AUDIO_IN_PIN   (A3)

// data pin
// SCL  A5
// SDA  A4




#if BLUETOOTH

#if 0
	#define RxD 1
	#define TxD 2	
	#define blueToothSerial Serial3		//the software serial port
#else
	#include <SoftwareSerial.h>   		//Software Serial Port
	#define RxD 2
	#define TxD 3
	SoftwareSerial blueToothSerial(RxD,TxD);	//the software serial port 
#endif

#endif


char recv_str[100];


// A, red button
void swt_afunc_intrupt()
{
    if ( valMode == 3 || valMode == 4  )  // B button 동작중이면 a button 무효화
        return;

    if ( valMode == 0 )
    	valMode =1;
    else
    if ( valMode == 1 )
    {
    	valMode =2;
    	 isNeedStoreTime = true;  // hour
    }
    else
    if ( valMode == 2 )
    {
    	valMode = 0;	
		isNeedStoreTime = true;  // hour
    }

    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
}


// B,  yellow button
void swt_bfunc_intrupt()
{
	if ( valMode == 1 || valMode == 2 )  // A button 동작중이면 B button 무효화
        return;


	if ( valMode == 0 )
		valMode = 3;
	else
	if ( valMode == 3 )
	{
		valMode = 4;
		isNeedStoreTheme = true;
	}
	else
	if ( valMode == 4 )
	{
		valMode = 0;		
		isNeedStoreBright = true;
	}

    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
    
}

void setup() 
{
    leds.begin();                       // Initialize 'leds' object
    //Make sure LED is off to demonstrate start of sketch
    initTest();             // Test Boot Animation

    theme_num = EEPROM.read( EEPROM_THEME_ADDR );
    bright_val = EEPROM.read( EEPROM_BRIGHT_ADDR );

    // teme_num   bright_val   range check. 
    fillTheme( theme_num );         // Setting Theme

    Serial.begin(BAUDRATE);
#if BLUETOOTH   
	blueToothSerial.begin(BAUDRATE);    //BT module baud rate
#endif

    Wire.begin();

    pinMode(SWITCH_A, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SWITCH_A), swt_afunc_intrupt, FALLING);
    pinMode(SWITCH_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SWITCH_B), swt_bfunc_intrupt, FALLING );

    pinMode ( ROT_PIN, INPUT);      //
    Serial.println("--- theme num --");
    Serial.println(theme_num);

/*
    while(1)
    {
        if(recvMsg(100) == 0)
        {
            if(strcmp((char *)recv_str, (char *)"OK+CONN") == 0)
            {
                Serial.println("connected\r\n");
                break;
            }
        }
        delay(200);
    }
  */  
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

#if BLUETOOTH
	if (blueToothSerial.available()) {
		Serial.write(blueToothSerial.read());
	}
	if (Serial.available()) {
		blueToothSerial.write(Serial.read());
		Serial.println("--- write bt messageasdfdsdsfsd ----");
	}
#endif


    setStoreTime();     // if need
    setStoreTheme();      // if need
   if ( (frames++ %50) == 0)
   {
        interval = !interval;    		// inverter si, bun     
        if ( 0 > get3231Date() )
        {
           SetPixelColor( 8, 100, 128, 0, 0);
           delay(100);   // 100mS
           return;
        }
	}

	int calcBrightVal = bright_val;

	// clear flag
	for ( int i = 0; i < NUM_LEDS; i++)
		leds.setPixelColor(i, 0 );
	
	int val = analogRead(ROT_PIN);
	if ( abs( val - rot_val) > 10 )           // chetting 
		rot_lock = false;


	if ( valMode == 1 ) // SET TIME MODE  (hour)
	{	
			if ( valFade <= 0 )
				valFade = 256;
			modeJung = 0;

			if (rot_lock == false)
			{
				int hour_ =  (int)( (float)(val) / (float)42.6666 );
				hours = hour_;
				minutes = 0;     
			}
	}
	else 
	if ( valMode == 2)  // SET TIME MODE  (min)
	{
			if ( valFade <= 0 )
				valFade = 256;
			modeJung = 0;	
			
			if (rot_lock == false)
			{
				int min_ =  (int)( (float)(val - 64) / 80 );
				minutes = ( min_ * 5 );
			}
	}
	else
	if ( valMode == 3 )	 // theme
	{
            calcBrightVal = 255;
			if ( valFade <= 0 )
				valFade = 256;
            modeJung = 1;
                
            if ( rot_lock == false )  {
                theme_num  = (int) ( val / 70 );		// 12 level 
                fillTheme( theme_num );         // Setting Theme
                //Serial.println( theme_num);
            }
            
            // color define
			leds.setPixelColor(0, cr_theme[0]  );
            leds.setPixelColor(3, cr_theme[3] );
            int f = 0, d = 0;
       //     matrix_number_to( theme_num, f, d );
		//	if ( f )
		//		leds.setPixelColor(f, cr_theme[f] );
         //   if ( d > 0 )
		//		leds.setPixelColor(d, cr_theme[d] );
			leds.setPixelColor(22, cr_theme[22] );        // 시
			leds.setPixelColor(24, cr_theme[24] );        // 분

	}
	else
	if ( valMode == 4 ) // Bright
	{
			if ( rot_lock == false )  
				bright_val = (val /4);
				
			if ( valFade <= 0 )
				valFade = 256;
			modeJung = 2; 	
	}
	else
	{
            //Serial.println("--- running mode----");
	}


	printDate2Serial();
	printButtonState2Serial();



	updateTime (calcBrightVal,  (valMode==1 || valMode==2 || valMode ==3 || valMode ==4), valMode==4  );
	if (valMode==1 || valMode==2 || valMode ==3 || valMode ==4)
	{
		// for setup
		delay(8);
	}
	else
		delay(100);   // 100mS
}		


void updateTime(int bright, bool bsetup, bool control_bright )
{

	bool isLogo = true;
		// 오후 6시는 밤,   오전 6시 낮	
	bool isDay = (hours >= 6 && hours < 18 );
	bool isMidNight = false;
#if MIDNIGHT
	isMidNight = ( hours > 0 && hours < 5 );
	if ( theme_num > 11 )
		isMidNight = false;
#endif

	int apply_bright = bright;

	if ( control_bright == true )
	{

	}
	else
	{
		if ( isDay == false ) 
			apply_bright -= (int) ( (float)bright * (isMidNight ? 0.8 : 0.6) );
	}

	if ( isLogo == true )
	{
		    // color define DAY 
    	leds.setPixelColor(0, cr_theme[0] );
    	leds.setPixelColor(3, cr_theme[3] );
    	if ( bsetup == false )
			leds.setPixelColor(8, cr_theme[3] );
	}
	else
	{
    	// color define DAY 
    	leds.setPixelColor(0, isDay ? cr_theme[0] : leds.Color(0,0,0)  );
    	leds.setPixelColor(3, isDay ? leds.Color(0, 0, 0) : cr_theme[3]  );
	}

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
 
    switch ( valMode )
    {
        case 1: leds.setPixelColor(_SI, (interval==1) ? cr_theme[22] : leds.Color(0,0,0)  );      // 시
                break;
        case 2: leds.setPixelColor(_BN, (interval==1) ? cr_theme[24] : leds.Color(0,0,0)  );     // 분
                break;

        case 0: break;        
    } 

Bitblt:
	if (valMode != 0 )
		INVERT_JUNG( modeJung );
		
    LED_SHOW( apply_bright ); 
}

void LED_SHOW( int calcB )
{

  //  leds.setBrightness( _gammaTable[val] );
	leds.setBrightness( calcB);
	leds.show();

	  //  leds.setGamma(0.5f);
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
        leds.setPixelColor( 8, leds.Color(valYellow, valYellow, 0) );
    }
    if ( valBlue > 0 )  {
       leds.setPixelColor( 8, leds.Color(0,0,valBlue) );
    }
    if ( valRed > 0 )  {
        leds.setPixelColor( 8, leds.Color(valRed,0,0) );
    }

    valFade -= 6;
}




//send command to Bluetooth and return if there is a response received
int sendBlueToothCommand(char command[])
{
#if BLUETOOTH
	
    Serial.print("send: ");
    Serial.print(command);
    Serial.println("");
#if NLCR
    blueToothSerial.println(command);
#else
    blueToothSerial.print(command);
#endif    
    delay(300);

    if(recvMsg(200) != 0) return -1;

    Serial.print("recv: ");
    Serial.print(recv_str);
    Serial.println("");
#endif
    return 0;
}

//receive message from Bluetooth with time out
int recvMsg(unsigned int timeout)
{
#if BLUETOOTH
	
    //wait for feedback
    unsigned int time = 0;
    unsigned char num;
    unsigned char i;

    //waiting for the first character with time out
    i = 0;
    while(1)
    {
        delay(50);
        if(blueToothSerial.available())
        {
            recv_str[i] = char(blueToothSerial.read());
            i++;
            break;
        }
        time++;
        if(time > (timeout / 50)) return -1;
    }

    //read other characters from uart buffer to string
    while(blueToothSerial.available() && (i < 100))
    {                                              
        recv_str[i] = char(blueToothSerial.read());
        i++;
    }

    
#if NLCR    
    recv_str[i-2] = '\0';       //discard two character \n\r
#else
    recv_str[i] = '\0';
#endif

#endif
    return 0;
}
