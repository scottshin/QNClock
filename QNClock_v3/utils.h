// 10진수를 2진화 10진수인 BCD 로 변환 (Binary Coded Decimal)
byte decToBcd(byte val)
{
    return ( (val/10*16) + (val%10) );
}

float get3231Temp()
{
    float temp3231 = 0.0f;
    
    byte tMSB, tLSB;
    //temp registers (11h-12h) get updated automatically every 64s
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
    
    if(Wire.available()) {
        tMSB = Wire.read(); //2's complement int portion
        tLSB = Wire.read(); //fraction portion
        
        temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
        temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
    }
    else {
        //error! no data!
    }
    return (temp3231);
}


void FillColor( int r, int g, int b )
{
    for ( int i = 0; i < NUM_LEDS; i++)
        leds.setPixelColor(i, leds.Color(r, g, b));  // Turn off LED
    leds.show();                        // Set new value
}

void SetPixelColor( int inx, int wait, int r, int g, int b )
{
    leds.setPixelColor(inx, leds.Color(r,g,b) );
    leds.show();
    delay(wait);
}

void ClearScreen()
{
    for ( int i = 0; i< NUM_LEDS; i++)  
        leds.setPixelColor(i, 0 );
    
    leds.show();
}



void initTest()
{

int wait = 1;                         // Standard wait delay of 5 milliseconds 



    for ( int lp =0; lp < 3; lp++ )
    for ( int i = 0; i < 255; i++)
    {
        if ( lp == 0 )
            FillColor(  i, 0, 0 );
        else
        if ( lp == 1 )
            FillColor( 0, i, 0 );
        else
            FillColor( 0, 0, i );

        delay ( 10 );
    }


  
    
    ClearScreen();

#if 1  // init test

    for ( int j= 0; j < (NUM_LEDS + 6); j++)
        for(int i=0; i<=MAX_LUX; i = i+1)            // Loop through next color change
        {

            if ( j < NUM_LEDS )
            {
                leds.setPixelColor(j, leds.Color(i,i,i) );
            }

            if ( j >= 4)
            {
                int k = (j-4);
                if ( k < MAX_LUX)
                {
                    leds.setPixelColor(k, leds.Color(MAX_LUX-i, MAX_LUX-i,MAX_LUX-i) );
                }
            }

            leds.show();
            delay( wait);
        }


#endif 


    ClearScreen();

}


void printDate2Serial()
{
    /*
     Serial.print(weekDay);
     Serial.print(", 20");
     Serial.print(year, DEC);
     Serial.print("/");
     Serial.print(month, DEC);
     Serial.print("/");
     Serial.print(date, DEC);
     Serial.print(" - ");
     Serial.print(hours, DEC);
     Serial.print(":");
     Serial.print(minutes, DEC);
     Serial.print(":");
     Serial.print(seconds, DEC);
     Serial.print(" - Temp: ");
     Serial.println(get3231Temp());
     */
}

void printButtonState2Serial()
{
    /*
    Serial.print("button :");
    for ( int i = 0; i < 6; i++)  {
        int buttonState = digitalRead(buttonPin + i);
        Serial.print(buttonState, DEC);
        Serial.print(", ");
    }
    Serial.println(" ");
    */
}



void IncMin()   // add 5minutes
{

    if ( minutes >= 55 )
    {
        minutes = 0;

        if ( hours >= 23 )
        {
            hours = 0;
        }
        else
        {
            hours++;
        }
         
    }
    else
    {
       minutes = ( minutes - (minutes%5) + 5 );
    }

    seconds = 0;
}

void matrix_hour_to( int h, int &a, int &b )
{
	a = 0, b = 0;
    switch ( h )
    {
        case 13:
        case 1:  a = 21, b =0; break;
        case 14:
        case 2:  a = 17, b =0; break;
        case 15:
        case 3:  a = 15, b =0; break;
        case 16:
        case 4:  a = 20, b =0; break;
        case 17:
        case 5:  a = 5, b =6; break;
        case 18:
        case 6:  a = 1, b =6; break;
        case 19:
        case 7:  a = 10, b =11; break;
        case 20:
        case 8:  a = 1, b =2; break;
        case 21:
        case 9:  a = 7, b =12; break;
        case 22:
        case 10:  a = 16, b =0; break;
        case 23:
        case 11:  a = 16, b =21; break;
        case 0:
        case 24:
        case 12:  a = 16, b =17; break;
    }
    
}


void matrix_number_to( int h, int &a, int &b )
{
    switch ( h+1 )
    {
        case 1:  a = 21, b =0; break;     
        case 2:  a = 17, b =0; break;
        case 3:  a = 15, b =0; break;
        case 4:  a = 20, b =0; break;
        case 5:  a = 5, b =6; break;        // 5
        case 6:  a = 1, b =6; break;        // 6
        case 7:  a = 10, b =11; break;
        case 8:  a = 1, b =2; break;
        case 9:  a = 7, b =12; break;
        case 10:  a = 16, b =0; break;
        case 11:  a = 16, b =21; break;
        case 12:  a = 16, b =17; break;
    }
    
}




void matrix_min_to( int h, int &a, int &b, int &c )
{
	a = 0, b = 0, c = 0;
    switch ( h )
    {
        default: a = b = c = 0; break;
            
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:  a = 0, b =19; break;
            
        case 10:
        case 11:
        case 12:
        case 13:
        case 14: a = 14, b =0; break;
            
        case 15:
        case 16:
        case 17:
        case 18:
        case 19: a = 14, b =19; break;
            
        case 20:
        case 21:
        case 22:
        case 23:
        case 24: a = 13, b =14; break;      // 이십
            
        case 25:
        case 26:
        case 27:
        case 28:
        case 29: a = 13, b =14;      // 이십
            c = 19;break;       // 오
        case 30:
        case 31:
        case 32:
        case 33:
        case 34: a = 18, b = 23;    // 삼십 분
            break;
            
        case 35:
        case 36:
        case 37:
        case 38:
        case 39: a = 18, b = 23;    // 삼십
            c= 19; break;      // 오
            
        case 40:
        case 41:
        case 42:
        case 43:
        case 44: a = 4, b = 14;    // 사십 분
            break;
            
        case 45:
        case 46:
        case 47:
        case 48:
        case 49: a = 4, b = 14;    // 사십
            c= 19; break;      // 오
        case 50:
        case 51:
        case 52:
        case 53:
        case 54: a = 9, b = 14;    // 오십 분
            break;
        case 55:
        case 56:
        case 57:
        case 58:
        case 59: a = 9, b = 14;    // 오십
            c= 19; break;      // 오
            
            
    }
    
}
