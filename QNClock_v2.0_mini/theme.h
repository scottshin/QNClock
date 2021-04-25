static const uint8_t PROGMEM _gt[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
    13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
    30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
    76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
    97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
    122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
    150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
    182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
    218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255
};



   //cr_theme[0] = Adafruit_NeoPixel.Color(0xC0, 0xB2, 0x83) ;          // day ( GOLD)

   // 
   //    cr_theme[_DAY] = Adafruit_NeoPixel.Color(255, 255, 215 );  // ㅊㅏ가운 화이트 

   // leds[0].Color(255, 225, 205 );  //whiteleds.Color(255, 225, 205 );  //보라빛이 살짝 도는 화이트 

void fillTheme( int idx)
{

    
    for ( int i = 0; i< NUM_LEDS; i++)  
        cr_theme[i] = leds[0].Color(255, 255, 115 );  //white
    

    if ( idx == 0 )     // default ( 녹 / 노란 / 아이보리 흰 / 보라 )
    {
        cr_theme[_DAY] = leds[0].Color(0, 0xff, 0);          // day
        cr_theme[_NIG] = leds[0].Color(0xff, 0xff, 0);       // night
        cr_theme[_SI] = leds[0].Color(0xff, 0, 0xff);      // hour
        cr_theme[_BN] = leds[0].Color(0xff, 0, 0xff);      // min
    }
    else
    if ( idx == 1)  // White / Light White
    {
       cr_theme[_DAY] = leds[0].Color(255, 225, 205 );  //white
       cr_theme[_NIG] =  leds[0].Color(255, 225, 205 );  //white
       cr_theme[_SI] =  leds[0].Color(255, 225, 205 );  //white
       cr_theme[_BN] =  leds[0].Color(255, 225,205 );  //white
    }
    else
    if ( idx == 2 )     // ragge
    {
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(225, 225, 50 );  // yello
    
       cr_theme[_DAY] = leds[0].Color(0xff, 0, 0);          // day
       cr_theme[_NIG] = leds[0].Color(0xff, 0, 0);       // night
       cr_theme[_SI] = leds[0].Color(0, 255, 0);      // hour
       cr_theme[_BN] = leds[0].Color(0, 255, 0);      // min
            
    }
    else
    if ( idx == 3 )     // ORANGE ( XXX )
    {
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(0xFF, 0x63, 0x47 );  //  TOMATO

        cr_theme[_DAY] = leds[0].Color(0xFF, 0xA0, 0x7A);          // LIGHT SALMON
        cr_theme[_NIG] = leds[0].Color(0xFF, 0x45, 0x00);       // ORANGE RED
		cr_theme[_SI] = leds[0].Color(0xFF, 0x8C, 0x00);      // DARK ORANGE
		cr_theme[_BN] = leds[0].Color(0xFF, 0xA5, 0x00);      // ORANGE
    }
    else
    if ( 4== idx )      // PURPLE tints and tones 
    {

/* 
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds.Color(_gt[0xC0], _gt[0xB2], _gt[0x83] ) ;        // orchild
          //  cr_theme[i] = leds.Color( _gt0xC0, _gt(0xB2, _gt(0x33);  //  orchid

      
        cr_theme[_DAY] =  leds.Color(_gt[0xC0], _gt[0xB2], _gt[0x83] ) ;          // day ( GOLD)
        cr_theme[_NIG] =  leds.Color(_gt[0xC0], _gt[0xB2], _gt[0x83] ) ;         // day ( GOLD)
        cr_theme[_SI] =  leds.Color(_gt[0xC0], _gt[0xB2], _gt[0x83] ) ;          // day ( GOLD)
        cr_theme[_BN] =  leds.Color(_gt[0xC0], _gt[0xB2], _gt[0x83] ) ;        // day ( GOLD)
*/

		for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(_gt[0xad], _gt[0xad], _gt[0xad] ) ;        // orchild
            
        cr_theme[_DAY] =  leds[0].Color(_gt[0x4e], _gt[0x11], _gt[0x14] ) ;          // day ( GOLD)
        cr_theme[_NIG] =  leds[0].Color(_gt[0x50], _gt[0x1b], _gt[0x1d] ) ;         // day ( GOLD)
        cr_theme[_SI] =  leds[0].Color(_gt[0x64], _gt[0x48], _gt[0x5c] ) ;          // day ( GOLD)
        cr_theme[_BN] =  leds[0].Color(_gt[0x83], _gt[0x67], _gt[0x7b] ) ;        // day ( GOLD)
	}
    else
    if ( 5 == idx )       // MODERN and CLEAN
    {
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(0xE3, 0x72, 0x22 );  // TANGERINE
        
        cr_theme[0] = leds[0].Color(0x07, 0x99, 0x9B);          // day  ( TEAL 
        cr_theme[3] = leds[0].Color(0x66, 0xB9, 0xBF);       // night (POWDER
        cr_theme[22] = leds[0].Color(0xee, 0xaa, 0x7b);      // hour (TAN)
        cr_theme[24] = leds[0].Color(0xee, 0xaa, 0x7b);      // min (TAN)
    }
    else
    if ( 6 == idx )         // White ( GRADATINO )
    {
        int val = 0x10;
        for ( int i = 0; i< NUM_LEDS; i++)  
        {
           if ( i  <= 12)
                val += 0x5;
            else
                val -= 0x5;
            cr_theme[i] = leds[0].Color( val , val, val);  //GOLD
        }

        
     
        /*
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds.Color(100, 100, 0xff );  //blue
   
        
        cr_theme[0] = leds.Color(0, 0xff, 0);          // day
        cr_theme[3] = leds.Color(0xff, 0xff, 0);       // night
        cr_theme[22] = leds.Color(0xff, 0, 0xff);      // hour
        cr_theme[24] = leds.Color(0xff, 0, 0xff);      // min
        */
    }
    else
    if ( 7 == idx )         // cool and fresh
    {

/*
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds.Color(0x80, 0x80, 0x80 );  //green
   
        cr_theme[0] = leds.Color(0xDC, 0xDC, 0xDC);          // day
        cr_theme[3] = leds.Color(0xD3, 0xD3, 0xD3);       // night
        cr_theme[22] = leds.Color(0x77, 0x88, 0x99);      // hour
        cr_theme[24] = leds.Color(0x70, 0x80, 0x90);      // min
        */

        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(_gt[0xAd], _gt[0xe5], _gt[0xa1] );  //green
   
        cr_theme[0] = leds[0].Color(_gt[0x8e], _gt[0xe4], _gt[0xaf]);          // day
        cr_theme[3] = leds[0].Color(_gt[0x5c], _gt[0xdb], _gt[0x95]);       // night
        cr_theme[22] = leds[0].Color(_gt[0x37], _gt[0x96], _gt[0x83]);      // hour
        cr_theme[24] = leds[0].Color(_gt[0x05], _gt[0x86], _gt[0x6b]);      // min
    }
    else
    if ( 8 == idx )         // HOT PINK  (Cherry Blasum)
    {
        for ( int i = 0; i< NUM_LEDS; i++)  
            cr_theme[i] = leds[0].Color(0xff, 0xB6, 0xC1 );  //green
            
        cr_theme[0] = leds[0].Color(0xFF, 0x69, 0xB4);          // day
        cr_theme[3] = leds[0].Color(0xff, 0x14, 0x93);       // night
        cr_theme[22] = leds[0].Color(0xC7, 0x15, 0x85);      // hour
        cr_theme[24] = leds[0].Color(0xC7, 0x15, 0x85);     // min
    }
    else
    if ( 9 == idx )         // green (Forest )
    {
        cr_theme[0] = leds[0].Color(0xAD, 0xff, 0x2f);    

        cr_theme[1] = leds[0].Color(0x7f, 0xff, 0x00);    
        cr_theme[2] = leds[0].Color(0x7c, 0xfc, 0x00);    
        cr_theme[3] = leds[0].Color(0x00, 0xff, 0x00);    
        cr_theme[4] = leds[0].Color(0x32, 0xcd, 0x32);
            
        cr_theme[5] = leds[0].Color(0x98, 0xFB, 0x98);    
        cr_theme[6] = leds[0].Color(0x90, 0xee, 0x90);    
        cr_theme[7] = leds[0].Color(0x00, 0xfa, 0x9a);    
        cr_theme[8] = leds[0].Color(0x00, 0xff, 0x7f);    
        cr_theme[9] = leds[0].Color(0x00, 0xff, 0x7f);
            
        cr_theme[10] = leds[0].Color(0x3c, 0xb3, 0x71);    
        cr_theme[11] = leds[0].Color(0x2e, 0x8b, 0x57);    
        cr_theme[12] = leds[0].Color(0x22, 0x8b, 0x22);    
        cr_theme[13] = leds[0].Color(0x00, 0x80, 0x00);    
        cr_theme[14] = leds[0].Color(0x00, 0x64, 0x00);   
         
        cr_theme[15] = leds[0].Color(0x9a, 0xcd, 0x32);
        cr_theme[16] = leds[0].Color(0x9a, 0xcd, 0x32);    
        cr_theme[17] = leds[0].Color(0x6b, 0x8e, 0x23);    
        cr_theme[18] = leds[0].Color(0x80, 0x80, 0x00);    
        cr_theme[19] = leds[0].Color(0x55, 0x6b, 0x2f);  
          
        cr_theme[20] = leds[0].Color(0x66, 0xcd, 0xaa);    
        cr_theme[21] = leds[0].Color(0x8f, 0xbc, 0x8f);    
        cr_theme[22] = leds[0].Color(0x20, 0xb2, 0xaa);    
        cr_theme[23] = leds[0].Color(0x00, 0x8b, 0x8b);    
        cr_theme[24] = leds[0].Color(0x00, 0x80, 0x80);    
    }
    else
    if ( 10 == idx )         // Blue Color (SEA)
    {
        cr_theme[0] = leds[0].Color(0, 0xff, 0xff);    
        cr_theme[1] = leds[0].Color(0, 0xff, 0xff);    
        cr_theme[2] = leds[0].Color(0xE0, 0xff, 0xff);    
        cr_theme[3] = leds[0].Color(0xAF, 0xee, 0xee);    
        cr_theme[4] = leds[0].Color(0x7f, 0xff, 0xDA);    
        cr_theme[5] = leds[0].Color(0x40, 0xE0, 0xd0);    
        cr_theme[6] = leds[0].Color(0x48, 0xd1, 0xcc);    
        cr_theme[7] = leds[0].Color(0, 0xce, 0xD1);    
        cr_theme[8] = leds[0].Color(0x5f, 0x9e, 0xa0);    
        cr_theme[9] = leds[0].Color(0x46, 0x82, 0xb4);    
        cr_theme[10] = leds[0].Color(0xb0, 0xc4, 0xde);    
        cr_theme[11] = leds[0].Color(0xb0, 0xe0, 0xe6);    
        cr_theme[12] = leds[0].Color(0xad, 0xd8, 0xe6);    
        cr_theme[13] = leds[0].Color(0x87, 0xce, 0xeb);    
        cr_theme[14] = leds[0].Color(0x87, 0xce, 0xfa);    
        cr_theme[15] = leds[0].Color(0, 0xbf, 0xff);    
        cr_theme[16] = leds[0].Color(0x1e, 0x90, 0xff);    
        cr_theme[17] = leds[0].Color(0x64, 0x95, 0xed);    
        cr_theme[18] = leds[0].Color(0x7b, 0x68, 0xee);    
        cr_theme[19] = leds[0].Color(0x41, 0x69, 0xe1);    
        cr_theme[20] = leds[0].Color(0, 0, 0xff);    
        cr_theme[21] = leds[0].Color(0, 0, 0xcd);    
        cr_theme[22] = leds[0].Color(0, 0, 0x8b);    
        cr_theme[23] = leds[0].Color(0, 0, 0x80);    
        cr_theme[24] = leds[0].Color(0x19, 0x19, 0x70);    


    }
    else
    if ( 11 == idx )         // YELLOW COLOR
    {

       for ( int i = 0; i< NUM_LEDS; i++)  
        cr_theme[i] = leds[0].Color(0xFF, 0xFF, 0x60 );    // LIGHT YELLOW
        

        cr_theme[_DAY] = leds[0].Color(0xfF, 0xD7, 0x00);     // day
        cr_theme[_NIG] = leds[0].Color(0xff, 0xD7, 0x00);       // night


           cr_theme[_SI] = leds[0].Color(0xff, 0xFF, 0x00);       // night
              cr_theme[_BN] = leds[0].Color(0xff, 0xFF, 0x00);       // night
   //    cr_theme[22] = leds[0].Color(0xf0, 0xe6, 0x8c);      // hour
   //     cr_theme[24] = leds[0].Color(0xBD, 0xB7, 0x6B);      // min
    }
	else
    if ( 12 == idx )         // ignore midnight
    {
        cr_theme[_DAY] = leds[0].Color(0x30, 0xff, 0x30);          // day
        cr_theme[_NIG] = leds[0].Color(0xff, 0xff, 0x30);       // night
        cr_theme[_SI] = leds[0].Color(0xbf, 0x30, 0xbf);      // hour
        cr_theme[_BN] = leds[0].Color(0xbf, 0x30, 0xbf);     // min
    }
    else
    if ( 13 == idx )
    {

    	for ( int i = 0; i< NUM_LEDS; i++)  
             cr_theme[i] = leds[0].Color(180, 255, 180 );  //white

        
    	cr_theme[_DAY] = leds[0].Color(0xB2, 0x22, 0x22) ;          // day ( RED )
        cr_theme[_NIG] =leds[0].Color(0xB2, 0x22, 0x22);           // day (  RED )
        cr_theme[_SI] = leds[0].Color(0x8B, 0x00, 0x00)  ;          // day ( RED)
        cr_theme[_BN] = leds[0].Color(0x8B, 0x00, 0x00) ;          // day ( RED)
    }
    else    // ETC
    {

       // default 
       cr_theme[0]  = leds[0].Color(215, 55, 215 );  //white
       cr_theme[3]  = leds[0].Color(255, 255,215 );  //cool white
       cr_theme[22] = leds[0].Color(55, 255, 215 );  // cyan
       cr_theme[24] = leds[0].Color(255, 55, 215 );  // magenta
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
