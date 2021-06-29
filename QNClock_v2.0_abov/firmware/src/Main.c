//------------------------------------------------------------------------------
//	include files and System mode define
//------------------------------------------------------------------------------
#define		MyDef			// prototype
#include	"Global.h"		//

//------------------------------------------------------------------------------
//		USER DATA
//------------------------------------------------------------------------------
//#define BUFSIZE 10
#if 0
extern void	Nop_1us_16Mhz(void);
extern void I2C_Write(unsigned char* i2cdata, unsigned char length);
extern void DS3231_Write(UCHAR Data[], UCHAR Addr, UCHAR Len);
extern void I2C_Read(unsigned char length);
extern void CLK_ReadData(UCHAR Slave);

extern void I2C_TestRead(UCHAR Slave_addr, UCHAR Data[], UCHAR Len);
extern void I2c_WriteData(UCHAR Data[], UCHAR Start_Addr, UCHAR Len);
#endif
extern void DS3231_Write(UCHAR Data[], UCHAR Addr, UCHAR Len);
extern void DS3231_Read(UCHAR Data[], UCHAR Addr, UCHAR Len);
extern UCHAR I2c_ReadByte(UCHAR Slave, UCHAR Addr);
extern void I2C_TestWrite(UCHAR Slave_addr, UCHAR Data[], UCHAR Len);
extern void I2C_TestRead(UCHAR Slave_addr, UCHAR Data[], UCHAR Len);
extern void write_i2c_bytes(UCHAR Slave_Addr, UCHAR Data);

extern void Read_Flash(unsigned long sector_address, unsigned int size, unsigned char *buf);
extern void Update_Flash_Buf(unsigned char *buf, unsigned int size);
extern void Update_Flash(unsigned int key);
extern void Init_Flash();


#define DS3231 			0xD0
#define INTCH				0x04
#define A2IE				0x02
#define A1IE				0x01
#define ENCODER_A		P03
#define ENCODER_B		P04
#define SCL_HIGH()  P20 = 1
#define SCL_LOW()		P20 = 0
#define SDA_HIGH()	P21 = 1
#define SDA_LOW()		P21 = 0

#define PAGE_BUFFER_SIZE (0x20)
#define FLASH_ADDR_USER 0x001FE0
#define FLASH_KEY 0xAA55






#define LED_CNTL P07
#define LED1_ON() (P05 = 0)
#define LED2_ON() (P06 = 0)
#define LED1_OFF() (P05 = 1)
#define LED2_OFF() (P06 = 1)
#define LED1_TOGGLE() (P05 ^= 1)
#define LED2_TOGGLE() (P06 ^= 1)
#define G     0
#define R			1
#define B			2
#define day 	0
#define yeo 	1
#define deolb 2
#define night 3
#define sa		4
#define da		5
#define seos	6
#define ah		7
#define jung 	8
#define oh		9
#define il		10
#define gob		11
#define hob		12
#define ee		13
#define sib		14
#define se		15
#define yeol	16
#define du		17
#define sam		18
#define ohh		19
#define ne		20
#define	han		21
#define si 		22
#define sibb	23
#define	bun		24








unsigned char SWITCH_A = 0;
unsigned char SWITCH_B = 0;
unsigned char SWITCH_R = 0;
unsigned char RTC_INTERRUPT = 0;
UCHAR mode = 0;			// 1:clock set
static UCHAR oldA;
static UCHAR oldB;

UCHAR BB = 0;

#define R_Data read_data

void LED_RESET(void);
//void SET_LED(unsigned char GRB[][3]);
//void SET_COLOR(unsigned char GRB[][3], char Hours, char Minutes);
//void SET_PIXEL_COLOR(UCHAR brightness, UCHAR GRB[][3], UCHAR BRT[][3]);
//void SET_PIXEL_COLOR(UCHAR brightness, UCHAR GRB[][3], UCHAR BRT[][3]);
//void GET_PIXEL_COLOR(UCHAR num, UCHAR brightness, UCHAR GRB[][3], UCHAR BRT[][3]);
//void SET_PIXEL_COLOR(UCHAR brightness, UCHAR THEMA[][3], UCHAR COLOR[][3], char Hours, char Minutes);
void SET_PIXEL_COLOR(UCHAR brightness, UCHAR THEMA[][3]);
void SET_LED(UCHAR table[], unsigned char GRB[][3]);
//void SET_LED(UCHAR GRB[][3]);
void SET_CLOCK_TABLE( UCHAR table[], char Hours, char Minutes);
void SET_THEMA(unsigned char GRB[][3]);
void SET_PIX(UCHAR num, unsigned char GRB[][3], UCHAR red, UCHAR green, UCHAR blue);
void LED_CLEAR(unsigned char GRB[][3]);
// void LED_TEST(unsigned char GRB[][3]);
void LED_TEST(UCHAR Table[], unsigned char GRB[][3]);
char Encoder(void);
UCHAR Get_Brightness(UCHAR user_brightness, char Hours);


UCHAR Pre_Status;
UCHAR data write_data[32];

enum ColorThema {
		Base = 0,
		Star,
		Reggae,
		March,
		April,
		May,
		June,
		Moon,
		Young, 
		Neon, 
		Cloudy,
		Spring, 
		Summer, 
		Sea, 
		Winter
} Thema;


//==============================================================================
//    task  : Input/output port selection
//==============================================================================
//------------------------------------------------------------------------------
//    Main Board I/O initialize
//------------------------------------------------------------------------------
void Port_Initial(void)
{
/*******************************************
		P02 => SWITCH_A		(INPUT, EINT0)
		P03 => ENCODER_A	(INPUT)
		P04 => ENCODER_B	(INPUT)
		P05 => LED1 	(OUT)
		P06 => LED2 	(OUT)
		P07 => CLOCK_LED	(OUT)
		P14 => RXD
		P15 => TXD
		P20 => SCL
		P21 => SDA
		P22 => SWITCH_B 	(INPUT, EINT1)
		P23 => SWITCH_R		(INPUT, EINT10)
*******************************************/
		P0IO = _1110_0000;    	// direction
		P0PU = _1000_0000;    	// pullup
		P0OD = _0000_0000;    	// open drain
		P0DB = _0000_0000;    	// debounce
		P0   = _0000_0000;    	// port initial value

		P1IO = _0000_0000;    	// direction
		P1PU = _0010_0000;    	// pullup
		P1OD = _0000_0000;    	// open drain
		P1DB = _0000_0000;    	// debounce
		P1   = _0000_0000;    	// port initial value

		P2IO = _0000_0011;    	// direction
		P2PU = _0000_0000;    	// pullup
		P2OD = _0000_0000;    	// open drain
		P2DB = _0000_0000;    	// debounce
		P2   = _0000_0000;    	// port initial value

		// Set port functions
		P0FSR	= _0000_0000;
		//P1FSRH	= _0001_0101;		// TXD, RXD (P15, P14)
		//P1FSRL	= _0000_0000;
		P1FSRH	= _0000_0000;		// TXD, RXD (P15, P14)
		P1FSRL	= _0000_0000;		
		P2FSR	= _0100_0000;
}


//------------------------------------------------------------------------------
// C51: SOFTWARE RESET
// void	reset (void)
// {
// 	((void (code *) (void)) 0x0000) ();		// only support Keil C51 !!
// }
//------------------------------------------------------------------------------
void Main( void )
{

//------------------------------------------------------------------------------
//		USER DATA
//------------------------------------------------------------------------------
		UCHAR THEMA[25][3];// = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
																//{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
																//{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
																//{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
																//{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
//		UCHAR tmp_color[3];
		UCHAR xdata read_data[32];
		UCHAR LED_Table[25] = {0, };
		UCHAR user_brightness = 255;
		UCHAR var_brightness;
		// UCHAR data Rcv_Data[32];
		UINT tmp_16 = 0;
		UCHAR loop_count = 0;
		char change_en = 0;
		char Hours, Minutes;

		UCHAR tmp = 0, i = 0;
		Pre_Status = 0;
		
		Initial_Clock();
		Port_Initial(); 	// Main board port initialize 
		//USART_init();
 
//--------------------    System Stabilize Delay   -----------------------------
		//wait_100uSec(250);	        // 25ms wait
		DELAY_US(65000);
    RSFR = 0;
    
//-------------------------- interrupt -----------------------------------------
    //Timer0_Initial();
		//t0int_en();
	  ExINT_init();
		//Init_I2C();
		Init_Flash();
		EA	=1;
		//wait_100uSec(250);	
		DELAY_US(65000);
//-------------------------- Gpio -----------------------------------------		
		SCL_HIGH();
		SDA_HIGH();
		LED1_OFF();
		LED2_OFF();
		oldA = ENCODER_A;
		oldB = ENCODER_B;
		Thema = Base;

//-------------------------- TEST -----------------------------------------		
		LED_TEST( LED_Table, THEMA);
		
//-------------------------- get time info -----------------------------------------
		DS3231_Read(R_Data, 0x00, 19);
		Minutes = (R_Data[1] >> 4)*10 + (R_Data[1] & 0x0F);
		Hours = ((R_Data[2] & 0x30) >> 4) * 10 + (R_Data[2] & 0x0F);
		R_Data[0x07] = 0x00;
		R_Data[0x08] = 0x80;
		R_Data[0x09] = 0x80;
		R_Data[0x0A] = 0x80;
					
		R_Data[0x0E] = (R_Data[0x0E] | INTCH | A1IE);						// alarm 1 set
		R_Data[0x0F] = R_Data[0x0F] & 0xFC;											// alarm interrupt out pull up 
		DS3231_Write(R_Data, 0x07, 9);
		//wait_100uSec(5);
		DELAY_US(5000);

		Read_Flash(FLASH_ADDR_USER, PAGE_BUFFER_SIZE, read_data);
		if(read_data[30] > 14)
				Thema = 0;
		else
				Thema = read_data[30];
		if(read_data[31] < 5)
				user_brightness = 255;
		else
				user_brightness = read_data[31];
		//Erase_Flash( 0x1FE0 );
		
		SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
		SET_THEMA(THEMA);
		var_brightness = Get_Brightness(user_brightness, Hours);
		SET_PIXEL_COLOR(var_brightness, THEMA);
		//tmp_color[R] = THEMA[si][R];
		//tmp_color[G] = THEMA[si][G];
		//tmp_color[B] = THEMA[si][B];
		SET_LED(LED_Table, THEMA);
//-------------------------------------------------------------------------------
		while(1)
		{	
				#if 1
				if(SWITCH_R){
						SWITCH_R = 0;
						SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
						SET_THEMA(THEMA);
						var_brightness = Get_Brightness(user_brightness, Hours);
						SET_PIXEL_COLOR(var_brightness, THEMA);
						//tmp_color[R] = THEMA[si][R];
						//tmp_color[G] = THEMA[si][G];
						//tmp_color[B] = THEMA[si][B];
						SET_LED(LED_Table, THEMA);
						if(Pre_Status == 1) { 																	// Thema and Brightness setting done
								//SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
								//SET_THEMA(THEMA);
								//var_brightness = Get_Brightness(user_brightness, Hours);
								//SET_PIXEL_COLOR(var_brightness, THEMA);
								//SET_LED(LED_Table, THEMA);
// flash write **************
								Read_Flash(FLASH_ADDR_USER, PAGE_BUFFER_SIZE, read_data);
								for(i = 0; i < PAGE_BUFFER_SIZE ; i++)
										write_data[i] = read_data[i];
								write_data[30] = Thema;
								write_data[31] = user_brightness;
								Update_Flash_Buf(write_data, PAGE_BUFFER_SIZE);
								Update_Flash(FLASH_KEY);
						}
						else { 																									// Clock setting done
								for(i = 0; i < 32; i++)
										R_Data[i] = 0;
								R_Data[1] = ((Minutes / 10) << 4) + ( Minutes % 10);
								R_Data[2] = ((Hours / 10) << 4) + ( Hours % 10);
								R_Data[0x07] = 0x00;
								R_Data[0x08] = 0x80;
								R_Data[0x09] = 0x80;
								R_Data[0x0A] = 0x80;
					
								R_Data[0x0E] = (R_Data[0x0E] | INTCH | A1IE);						// alarm 1 set
								R_Data[0x0F] = R_Data[0x0F] & 0xFC;											// alarm interrupt out pull up 
								DS3231_Write(R_Data, 0x00, 19);
								//SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
								//SET_THEMA(THEMA);
								//var_brightness = Get_Brightness(user_brightness, Hours);
								//SET_PIXEL_COLOR(var_brightness, THEMA);
								//SET_LED(LED_Table, THEMA);
						}
				}
				#endif
//-------------------------------------------------------------------------------				
				#if 1
				if(SWITCH_A ){																							// SWITCH A
						if(tmp_16 == 0 && loop_count == 0){
								if(SWITCH_A == 1)
										SET_PIX(jung, THEMA, 0, 0, (0xFF * user_brightness) >> 8);
								else
										SET_PIX(jung, THEMA, (0xFF * user_brightness) >> 8, 0, 0);
								LED_Table[jung] = 1;
								SET_LED(LED_Table, THEMA);
						}
						if(tmp_16 == 0 && loop_count == 2) {
								LED_Table[jung] = 0;
								SET_LED(LED_Table, THEMA);
						}
						if(change_en){
								if(SWITCH_A == 1) {																	// setting Thema
										if( change_en  == -1) {
												if( Thema != Base ) 
														Thema--;
										} 
										else {
												if( Thema != Winter ) 
														Thema++;
										}
										//SET_THEMA(THEMA);
								}
								else {																							// setting Brightness
										if( change_en  == -1) {
												if( user_brightness >= 20 ) 
														user_brightness -= 10;
										} 
										else {
												if( user_brightness <= 240 ) 
														user_brightness += 10;
										}
								}
								SET_THEMA(THEMA);
								if(LED_Table[jung]){
										if(SWITCH_A == 1)
												SET_PIX(jung, THEMA, 0, 0, 0xFF);
										else
												SET_PIX(jung, THEMA, 0xFF, 0, 0);
								}
								SET_PIXEL_COLOR(user_brightness, THEMA);
								SET_LED(LED_Table, THEMA);
								change_en = 0;
						}
				}
				#else
//------------------------------------------------------------------------------
				if(SWITCH_A == 1){													// SWITCH A ----> Thema Setting
						if(tmp_16 == 0 && loop_count == 0){
								SET_PIX(jung, COLOR, 0, 0, (0xFF * user_brightness) >> 8);
								SET_LED(COLOR);
						}
						if(tmp_16 == 0 && loop_count == 2) {
								SET_PIX(jung, COLOR, 0, 0, 0);
								SET_LED(COLOR);
						}
						if(change_en){
								if( change_en  == -1) {
										if( Thema != Base ) 
												Thema--;
								} 
								else {
										if( Thema != Winter ) 
												Thema++;
								}
								SET_THEMA(THEMA);
								SET_PIXEL_COLOR(user_brightness, THEMA, COLOR, Hours, Minutes);
								SET_LED(COLOR);
								change_en = 0;
						}

				}
				if(SWITCH_A == 2){											// SWITCH A, A -------> Brightness Setting
						if(tmp_16 == 0 && loop_count == 0){
								SET_PIX(jung, COLOR, (0xFF * user_brightness) >> 8, 0, 0);
								SET_LED(COLOR);
						}
						if(tmp_16 == 0 && loop_count == 2) {
								SET_PIX(jung, COLOR, 0, 0, 0);
								SET_LED(COLOR);
						}
						if( change_en ) {
								if( change_en  == -1) {
										if( user_brightness >= 20 ) 
												user_brightness -= 10;
								} 
								else {
										if( user_brightness <= 240 ) 
												user_brightness += 10;
								}
								SET_PIXEL_COLOR(user_brightness, THEMA, COLOR, Hours, Minutes);
								SET_LED(COLOR);
								change_en = 0;
						}
				}
				#endif
//---------------------------------------------------------------------------
				if(mode){																										// switch B
					if(tmp_16 == 0 && loop_count == 0){
								tmp = 1;
								LED_Table[jung] = 1;
								SET_PIX(jung, THEMA, (0xFF * user_brightness) >> 8, 0, 0);
								if(mode == 1) {
										LED_Table[si] = 1;
										SET_PIX(si, THEMA, (0xFF * user_brightness) >> 8, 0, 0); }
								else {
										LED_Table[bun] = 1;
										SET_PIX(bun, THEMA, (0xFF * user_brightness) >> 8, 0, 0); }
								SET_LED(LED_Table, THEMA);
						}
						if(tmp_16 == 0 && loop_count == 2) {
								tmp = 0;
								LED_Table[jung] = 0;
								if(mode == 1)
										//SET_PIX(si, COLOR, 0, 0, 0);
										LED_Table[si] = 0;
								else
										//SET_PIX(bun, COLOR, 0, 0, 0);	
										LED_Table[bun] = 0;
								SET_LED(LED_Table, THEMA);
						}
						if(BB) {
								BB = 0;
								SET_THEMA(THEMA);
								LED_Table[si] = 1;
								SET_LED(LED_Table, THEMA);
								//SET_PIX(si, THEMA, tmp_color[R], tmp_color[G], tmp_color[B]);
						}
						if(change_en) {
								if(mode == 1) {																			// SET Hour
										Hours += change_en;
										if( Hours >= 24 )
												Hours = 0;
										else if (Hours == -1)
												Hours = 23;
								} else if (mode == 2) {															// SET Minute
										//Minutes = (Minutes / 5) * 5;
										Minutes += (change_en*5);
										if( Minutes >= 60)
												Minutes = 0;
										if( Minutes < 0 )
												Minutes = 55;
										Minutes /= 5;
										Minutes *= 5;
								} else
										;
								//SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
								SET_THEMA(THEMA);
								SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
								if(tmp){
										LED_Table[jung] = 1;
										SET_PIX(jung, THEMA, 0xFF, 0, 0);
										if(mode == 1) {
												SET_PIX(si, THEMA, 0xFF, 0, 0);
										} else {
												SET_PIX(bun, THEMA, 0xFF, 0, 0);
										}
								}else {
										LED_Table[jung] = 0;
										if(mode == 1) {
												LED_Table[si] = 0;
										} else {
												LED_Table[bun] = 0;
										}
								}
								var_brightness = Get_Brightness(user_brightness, Hours);
								SET_PIXEL_COLOR(var_brightness, THEMA);
								//SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
								SET_LED(LED_Table, THEMA);
								change_en = 0;
						}
				}
				#if 1
//-------------------------------------------------------------------------------
				if(RTC_INTERRUPT){
						RTC_INTERRUPT = 0;
						LED2_TOGGLE();
						DS3231_Read(R_Data, 0x00, 19);
						Minutes = (R_Data[1] >> 4)*10 + (R_Data[1] & 0x0F);
						Hours = ((R_Data[2] & 0x30) >> 4) * 10 + (R_Data[2] & 0x0F);
						R_Data[0x0F] = R_Data[0x0F] & 0xFC;
						DS3231_Write(R_Data, 0x0F, 1);
						SET_CLOCK_TABLE( LED_Table, Hours, Minutes);
						SET_LED(LED_Table, THEMA);
				}
				#endif
//-------------------------------------------------------------------------------
				tmp_16++;
				if(tmp_16 >= 20000){					// default 20000
						loop_count++;
						tmp_16 = 0;
				}		
				if (loop_count >= 4){
						loop_count = 0;
				}				
				change_en = Encoder();				
		}		// while end
}


//==============================================================================
#if 1
void LED_RESET(void) {
		//unsigned char i;
		LED_CNTL = LOW;
		DELAY_US(25);
}
#endif

#if 1
UCHAR Get_Brightness(UCHAR user_brightness, char Hours) {
		UCHAR tmp;
		if(Hours == 0 || Hours == 1 )
				tmp = (user_brightness*60) / 100;
		else if(Hours >= 2 && Hours < 5 )
				tmp = (user_brightness*20) / 100;
		else if(Hours == 5 )
				tmp = (user_brightness*60) / 100;
		else if(Hours >= 19 && Hours < 24)
				tmp = (user_brightness*60) / 100;
		else
				return user_brightness;
		if(tmp < 10)
				return 10;
		else
			return tmp;
}
#endif



#if 0
void GET_PIXEL_COLOR(UCHAR num, UCHAR brightness, UCHAR GRB[][3], UCHAR BRT[][3]) {
		//UCHAR i, j;
		UCHAR j;
		for( j = 0; j < 3; j++ )
				BRT[num][j] = (GRB[num][j] * brightness) >> 8;
}
#endif





#if 0
void SET_PIXEL_COLOR(UCHAR brightness, UCHAR THEMA[][3], UCHAR COLOR[][3], char Hours, char Minutes) {
		UCHAR i, j, a, b;
		for(i = 0; i < 25; i++)
				for( j = 0; j < 3; j++)
						COLOR[i][j] = 0;
	
		if(Hours > 6 && Hours <19){
				GET_PIXEL_COLOR(day, brightness, THEMA, COLOR);
		}
		else {
				GET_PIXEL_COLOR(night, brightness, THEMA, COLOR);
		}
		
		switch(Hours%12) {
				case 0 :
						GET_PIXEL_COLOR(yeol, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(du, brightness, THEMA, COLOR);
						break;	
				case 1 :
						GET_PIXEL_COLOR(han, brightness, THEMA, COLOR);
						break;				
				case 2 :
						GET_PIXEL_COLOR(du, brightness, THEMA, COLOR);
						break;
				case 3 :
						GET_PIXEL_COLOR(se, brightness, THEMA, COLOR);
						break;
				case 4 :
						GET_PIXEL_COLOR(ne, brightness, THEMA, COLOR);
						break;	
				case 5 :
						GET_PIXEL_COLOR(da, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(seos, brightness, THEMA, COLOR);
						break;				
				case 6 :
						GET_PIXEL_COLOR(yeo, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(seos, brightness, THEMA, COLOR);
						break;
				case 7 :
						GET_PIXEL_COLOR(il, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(gob, brightness, THEMA, COLOR);
						break;
				case 8 :
						GET_PIXEL_COLOR(yeo, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(deolb, brightness, THEMA, COLOR);
						break;
				case 9 :
						GET_PIXEL_COLOR(ah, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(hob, brightness, THEMA, COLOR);
						break;		
				case 10 :
						GET_PIXEL_COLOR(yeol, brightness, THEMA, COLOR);
						break;
				case 11 :
						GET_PIXEL_COLOR(yeol, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(han, brightness, THEMA, COLOR);
						break;
				default : 
						;
		}
		GET_PIXEL_COLOR(si, brightness, THEMA, COLOR);
		GET_PIXEL_COLOR(bun, brightness, THEMA, COLOR);
		a = Minutes / 10;
		b = Minutes % 10;
		switch(a) {
				case 0 :
						if(b > 4) {
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						} else
								SET_PIX(bun, COLOR, 0, 0, 0);
						break;	
				case 1 :
						GET_PIXEL_COLOR(sib, brightness, THEMA, COLOR);
						if(b > 4) 
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						break;				
				case 2 :
						GET_PIXEL_COLOR(ee, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(sib, brightness, THEMA, COLOR);
						if(b > 4) 
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						break;
				case 3 :
						GET_PIXEL_COLOR(sam, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(sibb, brightness, THEMA, COLOR);
						if(b > 4)
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						break;
				case 4 :
						GET_PIXEL_COLOR(sa, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(sib, brightness, THEMA, COLOR);
						if(b > 4) 
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						break;	
				case 5 :
						GET_PIXEL_COLOR(oh, brightness, THEMA, COLOR);
						GET_PIXEL_COLOR(sib, brightness, THEMA, COLOR);
						if(b > 4) 
								GET_PIXEL_COLOR(ohh, brightness, THEMA, COLOR);
						break;	
				default :
						;
			}	
}
#else
void SET_PIXEL_COLOR(UCHAR brightness, UCHAR THEMA[][3]) {
		UCHAR i, j;
		for(i = 0; i < 25; i++)
				for( j = 0; j < 3; j++)
						THEMA[i][j] = (THEMA[i][j] * brightness) >> 8;
}
#endif






#if 0
void SET_LED(unsigned char GRB[][3]) {
		unsigned char i, j, k;
		for(k = 0; k < 25; k++){
				for( i = 0; i < 3; i++){
						for(j = 7; j < 255; j--) {
								if( GRB[k][i] & (1<<j) ){
										//LED_BIT_1();
										LED_CNTL = HIGH;
										NOP; NOP; NOP; NOP; NOP; NOP;
									  NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										LED_CNTL = LOW;
										//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
								}
								else {
										//LED_BIT_0();
										LED_CNTL = HIGH;
										NOP; NOP; NOP; //NOP; //NOP; NOP;
										LED_CNTL = LOW;
										//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
								}
						}
				}
				LED_RESET();
		}
		LED_RESET();
}
#else
void SET_LED(UCHAR table[], unsigned char GRB[][3]) {
		UCHAR i, j, k;
		for(k = 0; k < 25; k++){
				if( table[k] ) {
						for( i = 0; i < 3; i++){
								for(j = 7; j < 255; j--) {
										if( GRB[k][i] & (1<<j) ){
												//LED_BIT_1();
												LED_CNTL = HIGH;
												NOP; NOP; NOP; NOP; NOP; NOP;
												NOP; NOP; NOP; NOP; NOP; NOP; NOP;
												LED_CNTL = LOW;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										}
										else {
												//LED_BIT_0();
												LED_CNTL = HIGH;
												NOP; NOP; NOP; //NOP; //NOP; NOP;
												LED_CNTL = LOW;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										}
								}
						}
						LED_RESET();
				}
				else {
						for( i = 0; i < 3; i++){
								for(j = 7; j < 255; j--) {
										//if( GRB[k][i] & (1<<j) ){
												//LED_BIT_1();
												//LED_CNTL = HIGH;
												//NOP; NOP; NOP; NOP; NOP; NOP;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
												//LED_CNTL = LOW;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										//}
										//else {
												//LED_BIT_0();
												LED_CNTL = HIGH;
												NOP; NOP; NOP; //NOP; //NOP; NOP;
												LED_CNTL = LOW;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
												//NOP; NOP; NOP; NOP; NOP; NOP; NOP;
										//}
								}
						}
						LED_RESET();
				}
		}
		LED_RESET();
}
#endif
#if 1
void SET_CLOCK_TABLE( UCHAR table[], char Hours, char Minutes){
		UCHAR i, a, b;
		for(i = 0; i < 25; i++) table[i] = 0;
	
		if(Hours > 6 && Hours <19){
				table[day] = 1;
		}
		else {
				table[night] = 1;
		}
		
		table[si] = 1;
		table[bun] = 1;
		
		switch(Hours%12) {
				case 0 :
						table[yeol] = 1;
						table[du] = 1;
						break;	
				case 1 :
						table[han] = 1;
						break;				
				case 2 :
						table[du] = 1;
						break;
				case 3 :
						table[se] = 1;
						break;
				case 4 :
						table[ne] = 1;
						break;	
				case 5 :
						table[da] = 1;
						table[seos] = 1;
						break;				
				case 6 :
						table[yeo] = 1;
						table[seos] = 1;
						break;
				case 7 :
						table[il] = 1;
						table[gob] = 1;
						break;
				case 8 :
						table[yeo] = 1;
						table[deolb] = 1;
						break;
				case 9 :
						table[ah] = 1;
						table[hob] = 1;
						break;		
				case 10 :
						table[yeol] = 1;
						break;
				case 11 :
						table[yeol] = 1;
						table[han] = 1;
						break;
				default : 
						;
		}
		//table[si] = 1;
		a = Minutes / 10;
		b = Minutes % 10;
		switch(a) {
				case 0 :
						if(b > 4)
								table[ohh] = 1;
						else
								table[bun] = 0;
						break;	
				case 1 :
						table[sib] = 1;
						if(b > 4) 
								table[ohh] = 1;
						break;				
				case 2 :
						table[ee] = 1;
						table[sib] = 1;
						if(b > 4) 
								table[ohh] = 1;
						break;
				case 3 :
						table[sam] = 1;
						table[sibb] = 1;
						if(b > 4) 
								table[ohh] = 1;
						break;
				case 4 :
						table[sa] = 1;
						table[sib] = 1;
						if(b > 4) 
								table[ohh] = 1;
						break;	
				case 5 :
						table[oh] = 1;
						table[sib] = 1;
						if(b > 4) 
								table[ohh] = 1;
						break;	
				default :
						;
			}	
}
#endif

void SET_THEMA(unsigned char GRB[][3]){
		UCHAR i;
		int val;
		switch(Thema) {
				case 1 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 255, 225, 115);
						SET_PIX(day, GRB, 255, 225, 205);
						SET_PIX(night, GRB, 255, 225, 205);
						SET_PIX(si, GRB, 255, 225, 205);
						SET_PIX(bun, GRB, 255, 225, 205);
						break;	
				case 2 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 255, 225, 50);					
						SET_PIX(day, GRB, 0xff, 0, 0);
						SET_PIX(night, GRB, 0xff, 0, 0);
						SET_PIX(si, GRB, 0, 255, 0);
						SET_PIX(bun, GRB, 0, 255, 0);
						break;				
				case 3 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xFF, 0x63, 0x47);		
						SET_PIX(day, GRB, 0xFF, 0xA0, 0x7A);
						SET_PIX(night, GRB, 0xFF, 0x45, 0x00);
						SET_PIX(si, GRB, 0xFF, 0x8C, 0x00);
						SET_PIX(bun, GRB, 0xFF, 0xA5, 0x00);
						break;
				case 4 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xad, 0xad, 0xad);		
						SET_PIX(day, GRB, 0x4e, 0x11, 0x14);
						SET_PIX(night, GRB, 0x50, 0x1b, 0x1d);
						SET_PIX(si, GRB, 0x64, 0x48, 0x5c);
						SET_PIX(bun, GRB, 0x83, 0x67, 0x7b);
						break;
				case 5 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xE3, 0x72, 0x22);		
						SET_PIX(day, GRB, 0x07, 0x99, 0x9B);
						SET_PIX(night, GRB, 0x66, 0xB9, 0xBF);
						SET_PIX(si, GRB, 0xee, 0xaa, 0x7b);
						SET_PIX(bun, GRB, 0xee, 0xaa, 0x7b);
						break;	
				case 6 :
						val = 0x10;
						for ( i = 0; i< 25; i++) {
								if ( i  <= 12)
										val += 0x5;
								else
										val -= 0x5;
								SET_PIX(i, GRB, val, val, val);
						}
						break;
				case 7:
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xAD, 0xE5, 0xA1);		
						SET_PIX(day, GRB, 0x8e, 0xe4, 0xaf);
						SET_PIX(night, GRB, 0x5c, 0xdb, 0x95);
						SET_PIX(si, GRB, 0x37, 0x96, 0x83);
						SET_PIX(bun, GRB, 0x05, 0x86, 0x6b);
						break;				
				case 8 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xFF, 0xB6, 0xC1);		
						SET_PIX(day, GRB, 0xFF, 0x69, 0xB4);
						SET_PIX(night, GRB, 0xff, 0x14, 0x93);
						SET_PIX(si, GRB, 0xC7, 0x15, 0x85);
						SET_PIX(bun, GRB, 0xC7, 0x15, 0x85);
						break;
				case 9 :
						SET_PIX(0, GRB, 0xAD, 0xff, 0x2f);    
						SET_PIX(1, GRB, 0x7f, 0xff, 0x00);    
						SET_PIX(2, GRB, 0x7c, 0xfc, 0x00);    
						SET_PIX(3, GRB, 0x00, 0xff, 0x00);    
						SET_PIX(4, GRB, 0x32, 0xcd, 0x32);
						SET_PIX(5, GRB, 0x98, 0xFB, 0x98);    
						SET_PIX(6, GRB, 0x90, 0xee, 0x90);    
						SET_PIX(7, GRB, 0x00, 0xfa, 0x9a);    
						SET_PIX(8, GRB, 0x00, 0xff, 0x7f);    
						SET_PIX(9, GRB, 0x00, 0xff, 0x7f);
						SET_PIX(10, GRB, 0x3c, 0xb3, 0x71);    
						SET_PIX(11, GRB, 0x2e, 0x8b, 0x57);    
						SET_PIX(12, GRB, 0x22, 0x8b, 0x22);    
						SET_PIX(13, GRB, 0x00, 0x80, 0x00);    
						SET_PIX(14, GRB, 0x00, 0x64, 0x00);   
						SET_PIX(15, GRB, 0x9a, 0xcd, 0x32);
						SET_PIX(16, GRB, 0x9a, 0xcd, 0x32);    
						SET_PIX(17, GRB, 0x6b, 0x8e, 0x23);    
						SET_PIX(18, GRB, 0x80, 0x80, 0x00);    
						SET_PIX(19, GRB, 0x55, 0x6b, 0x2f);  
						SET_PIX(20, GRB, 0x66, 0xcd, 0xaa);    
						SET_PIX(21, GRB, 0x8f, 0xbc, 0x8f);    
						SET_PIX(22, GRB, 0x20, 0xb2, 0xaa);    
						SET_PIX(23, GRB, 0x00, 0x8b, 0x8b);    
						SET_PIX(24, GRB, 0x00, 0x80, 0x80);  
						break;
				case 10 :
						SET_PIX(0, GRB, 0, 0xff, 0xff);       
						SET_PIX(1, GRB, 0, 0xff, 0xff);       
						SET_PIX(2, GRB, 0xE0, 0xff, 0xff);    
						SET_PIX(3, GRB, 0xAF, 0xee, 0xee);    
						SET_PIX(4, GRB, 0x7f, 0xff, 0xDA);
						SET_PIX(5, GRB, 0x40, 0xE0, 0xd0);    
						SET_PIX(6, GRB, 0x48, 0xd1, 0xcc);    
						SET_PIX(7, GRB, 0, 0xce, 0xD1);       
						SET_PIX(8, GRB, 0x5f, 0x9e, 0xa0);    
						SET_PIX(9, GRB, 0x46, 0x82, 0xb4);
						SET_PIX(10, GRB, 0xb0, 0xc4, 0xde);    
						SET_PIX(11, GRB, 0xb0, 0xe0, 0xe6);    
						SET_PIX(12, GRB, 0xad, 0xd8, 0xe6);    
						SET_PIX(13, GRB, 0x87, 0xce, 0xeb);    
						SET_PIX(14, GRB, 0x87, 0xce, 0xfa);   
						SET_PIX(15, GRB, 0, 0xbf, 0xff);   
						SET_PIX(16, GRB, 0x1e, 0x90, 0xff);    
						SET_PIX(17, GRB, 0x64, 0x95, 0xed);    
						SET_PIX(18, GRB, 0x7b, 0x68, 0xee);    
						SET_PIX(19, GRB, 0x41, 0x69, 0xe1);  
						SET_PIX(20, GRB, 0, 0, 0xff);        
						SET_PIX(21, GRB, 0, 0, 0xcd);        
						SET_PIX(22, GRB, 0, 0, 0x8b);        
						SET_PIX(23, GRB, 0, 0, 0x80);        
						SET_PIX(24, GRB, 0x19, 0x19, 0x70);
						break;				
				case 11 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 0xFF, 0xFF, 0x60);		
						SET_PIX(day, GRB, 0xfF, 0xD7, 0x00);
						SET_PIX(night, GRB, 0xff, 0xD7, 0x00);
						SET_PIX(si, GRB, 0xff, 0xFF, 0x00);
						SET_PIX(bun, GRB, 0xff, 0xFF, 0x00);
						break;
				case 12 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 255, 225, 115);					
						SET_PIX(day, GRB, 0x30, 0xff, 0x30);
						SET_PIX(night, GRB, 0xff, 0xff, 0x30);
						SET_PIX(si, GRB, 0xbf, 0x30, 0xbf);
						SET_PIX(bun, GRB, 0xbf, 0x30, 0xbf);
						break;
				case 13 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 180, 225, 180);					
						SET_PIX(day, GRB, 0xB2, 0x22, 0x22);
						SET_PIX(night, GRB, 0xB2, 0x22, 0x22);
						SET_PIX(si, GRB, 0x8B, 0x00, 0x00);
						SET_PIX(bun, GRB, 0x8B, 0x00, 0x00);
						break;				
				case 14 :
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 255, 225, 115);										
						SET_PIX(day, GRB, 215, 55, 215);
						SET_PIX(night, GRB, 255, 255, 215);
						SET_PIX(si, GRB, 55, 255, 215);
						SET_PIX(bun, GRB, 255, 55, 215);
						break;										
				default : 
						for ( i = 0; i< 25; i++)
								SET_PIX(i, GRB, 255, 225, 115);					
						SET_PIX(day, GRB, 0, 0xFF, 0);
						SET_PIX(night, GRB, 0xFF, 0xFF, 0);
						SET_PIX(si, GRB, 0xFF, 0, 0xFF);
						SET_PIX(bun, GRB, 0xFF, 0, 0xFF);
		}
}

void SET_PIX(UCHAR num, unsigned char GRB[][3], UCHAR red, UCHAR green, UCHAR blue) {
		GRB[num][R] = red;
		GRB[num][G] = green;
		GRB[num][B] = blue;
}

#if 0
void LED_CLEAR(unsigned char GRB[][3]) {
		unsigned char i, k;
		for(k = 0; k < 25; k++)
				for( i = 0; i < 3; i++)
						GRB[k][i] = 0;
}
#endif

void LED_TEST(UCHAR table[], unsigned char GRB[][3]) {
		unsigned char i, j, k;
		for(i = 0; i < 25; i++)
				table[i] = 1;

		for(k = 0; k < 3; k++) {
				for(i = 0; i < 127; i++) {
						for(j = 0; j < 25; j++) 
								GRB[j][k] = 2*i;
						SET_LED(table, GRB);
						wait_100uSec(3);
				}
				for(j = 0; j < 25; j++) 
						GRB[j][k] = 0;
		}
		for( i = 0; i < 25; i++) {
				for(j = 0; j < 3; j++)
						GRB[i][j] = 64;
				SET_LED(table, GRB);
				wait_100uSec(600);
		}
}


char Encoder(void){
		//static UCHAR oldA = 0;
		//static UCHAR oldB = 0;
		char result = 0;
		UCHAR newA = ENCODER_A;
		UCHAR newB = ENCODER_B;
		if (newA != oldA || newB != oldB) {
				if (oldA == LOW && newA == HIGH) {
					result = -(oldB * 2 - 1);
				}
		}
		oldA = newA;
		oldB = newB;
		
		return result;
}

