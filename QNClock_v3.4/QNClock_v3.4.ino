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

#define BAUDRATE 9600


#define MIC_PIN (A6)

// data pin
// SCL  A5
// SDA  A4


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

    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
}


// B,  yellow button
void swt_bfunc_intrupt()
{
	if ( timeSetMode > 0 )  // A button 동작중이면 B button 무효화
        return;

	valMode = ((++valMode) % 3);

    Serial.println("---bfunc --");
    Serial.println(valMode);
  
    if ( valMode == 2 )  isNeedStoreTheme = true;
    if ( valMode == 0 )  isNeedStoreBright = true;

  //valYellow = 0xFF;

    rot_lock = true;
    rot_val = analogRead( ROT_PIN );
    
}

void setup() 
{

	Serial.println("--- setup --");


	    
    leds.begin();                       // Initialize 'leds' object
    //Make sure LED is off to demonstrate start of sketch
    initTest();             // Test Boot Animation

    theme_num = EEPROM.read( EEPROM_THEME_ADDR );
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

    pinMode ( ROT_PIN, INPUT);      //
    pinMode( MIC_PIN, INPUT);

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


 if (HM10.available()) {
    Serial.write(HM10.read());
  }
  if (Serial.available()) {
    HM10.write(Serial.read());
  }

	
  int sound = analogRead( MIC_PIN );

  Serial.print( sound );
  Serial.print(",");
  Serial.print(760);
  Serial.print(",");
  Serial.println(0);

 delay(20);





  

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

		// 오후 6시는 밤,   오전 6시 낮	
	bool isDay = (hours >= 6 && hours < 18 );
	bool isMidNight = ( hours > 0 && hours < 5 );
	if ( theme_num > 11 )
		isMidNight = false;
  
	int calcBrightVal = bright_val;
	if ( isDay == false ) {
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
            //Serial.println("--- theme mode ----");
            if ( valBlue <= 0 )
                valBlue = 256;
                
            if ( rot_lock == false )  {
                theme_num  = (int) ( val / 70 );		// 12 level 
                fillTheme( theme_num );         // Setting Theme
                Serial.println( theme_num);
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
			//Serial.println("--- Bright mode ----");
			if ( false == rot_lock )  bright_val = (val /4);
			if ( valRed <= 0 )  valRed = 256;
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
        case 1: leds.setPixelColor(_SI, (interval==1) ? cr_theme[22] : leds.Color(0,0,0)  );      // 시
                break;
        case 2: leds.setPixelColor(_BN, (interval==1) ? cr_theme[24] : leds.Color(0,0,0)  );     // 분
                break;
    } 
}

Bitblt:
    if ( valYellow > 0 )  {
        leds.setPixelColor(8, leds.Color(valYellow, valYellow, 0) );
        valYellow -= 12;
    }
    if ( valBlue > 0 )  {
        leds.setPixelColor(8, leds.Color(0, 0, valBlue) );
        valBlue -= 12;
    }
    if ( valRed > 0 )  {
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
    unsigned char i;

    //waiting for the first character with time out
    i = 0;
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
