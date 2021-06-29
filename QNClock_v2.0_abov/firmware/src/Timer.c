//==============================================================================
//	file name	: Time.c
//==============================================================================

#define		MyDef	extern		//
#include	"Global.h"			//
//==============================================================================

volatile unsigned int  delayCnt2=0;
volatile unsigned char pre_p0=0;
#if 0
void T0_Int_Handler(void) interrupt TIMER0_VECT
{

	delayCnt2++;

	if (delayCnt2 <= 200)
	{
   P06=1;
	}
	else if(delayCnt2 > 200 && delayCnt2 < 400)
	{
   P06=0;
	}
	 else if(delayCnt2 >= 400 )
	 {
		 delayCnt2=0;
	 }

}

void T1_Int_Handler(void) interrupt TIMER1_VECT
{
		//P11	^=	1;
}

void T2_Int_Handler(void) interrupt TIMER2_VECT
{
		//P02	^=	1;
}
#endif







//==============================================================================
void EINT_Int_Handler(void) interrupt EINT_VECT
{
#if 1
		if ( EIFLAG & 0x01 ) {																		// SWITCH A - Thema and Brightness
				EIFLAG &= ~(0x01);
				if(mode == 0) {
						if( SWITCH_A == 0 || SWITCH_A == 1)								// SWITCH A - Thema and Brightness
								SWITCH_A++;
						else {
								SWITCH_A = 0;
								SWITCH_R = 1;
								Pre_Status = 1;
						}
				}
		}
		if ( EIFLAG & 0x02 ) {																		// SWITCH B - Time
				EIFLAG &= ~(0x02);
				if(SWITCH_A == 0) {
						if( mode == 0 ||  mode == 1 ) {											// SWITCH B - Time
								mode++;
								if(mode == 2)
										BB = 1;
						}
						else {
								mode = 0;
								SWITCH_R = 1;
								Pre_Status = 2;
						}
				}
						
		} 
		if (EIFLAG & 0x04) EIFLAG &= ~0x04;
#else
		// External interrupt 0,1,2
		// TODO: add your code here
		if (EIFLAG & 0x04) EIFLAG &= ~0x04;	// clear Ext. INT2
		if (EIFLAG & 0x02) EIFLAG &= ~0x02;	// clear Ext. INT1
		if (EIFLAG & 0x01) EIFLAG &= ~0x01;	// clear Ext. INT0
#endif
}








#if 0
void EINT10_Int_Handler(void) interrupt EINT10_VECT							// SWITCH R
{
		if(SWITCH_A) {
				SWITCH_A = 0;
				SWITCH_R = 1;
				Pre_Status = 1;
		}
		else if( mode == 1 ){
				mode++;
		}
		else {
				mode = 0;
				SWITCH_R = 1;
				Pre_Status = 2;
		}
}

void EINT11_Int_Handler(void) interrupt EINT11_VECT
{
}
#endif






//==============================================================================
void EINT12_Int_Handler(void) interrupt EINT12_VECT
{
		if(mode == 0)
				RTC_INTERRUPT = 1;
	
}






#if 0
void BIT_Int_Handler(void) interrupt BIT_VECT
{
}

void Timer0_Initial(void)
{
	// initialize Timer1
	
	// TIMER1 STOP
	T0CR	= (0x00)
			|( T0_DISABLE << 7)				// T0 Enable/Disable
			|( T0_PORTOUT_DISABLE << 6)		// T0 Portout control
			|( T0_MODE_COUNTER << 4)		// T0 Operation Mode Selection
			|( T0_CLKDIV_128 << 1)			// T0 CLK Selection
			|( T0_COUNT_CLR << 0);			// T0 Counter Clear
	
	T0CDR	= 0xF9;		//  1us * 250 = 250ms
	
	// TIMER1 START
	T0CR	= (0x00)
			|( T0_ENABLE << 7)				// T0 Enable/Disable
			|( T0_PORTOUT_ENABLE << 6)		// T0 Portout control
			|( T0_MODE_COUNTER << 4)		// T0 Operation Mode Selection
			|( T0_CLKDIV_128 << 1)			// T0 CLK Selection
			|( T0_COUNT_CLR << 0);			// T0 Counter Clear
    
    P2FSR   = (2 << 6)|(P2FSR & 0x3F); // T0OUT Function Select (P23)
    
}
#endif








//==============================================================================
void ExINT_init()
{
#if 0
		// initialize external interrupt
		EIPOL0 = 0x0A;  	// polarity 10 ~ 12
		EIPOL1 = 0x2A;  	// polarity 0 ~ 2
		IE |= 0x02;     	// enable Ext. INT. 10
		IE |= 0x04;     	// enable Ext. INT. 11
		EIFLAG = 0x00;  	// clear Ext. INT 0,1,2 flags
		IE |= 0x10;     	// enable Ext. INT. 0, 1, 2
		eint10_en();
		eint11_en();
		eint_en();
#else
		P0FSRL  = (1 << 4)|(P0FSRL & 0xCF); // EINT0  Function Select (P02)
    P2FSR   = (1 << 4)|(P2FSR  & 0xCF); // EINT1  Function Select (P22)
    P0FSRH  = (1 << 0)|(P0FSRH & 0xFC); // EINT2  Function Select (P04)
    
    // EINT10?? P14? P23 ?? ?? ?, ?? ???? Interrput ??
    //P1FSRH  = (3 << 0)|(P1FSRH & 0xFC); // EINT10 Function Select (P14)
    P2FSR   = (1 << 6)|(P2FSR  & 0x3F); // EINT10 Function Select (P23)
    P0FSRL  = (1 << 6)|(P0FSRL & 0x3F); // EINT11 Function Select (P03)
    P1FSRL  = (1 << 4)|(P1FSRL & 0xCF); // EINT12 Function Select (P12)
    
    EIPOL1	= (0x00)
			|( EINT2_NO_EDGE << 4)		// EINT2 Edge Selection
			|( EINT1_RISE_EDGE << 2)		// EINT1 Edge Selection
			|( EINT0_RISE_EDGE << 0);		// EINT0 Edge Selection
    //EIPOL1	= 0x02;		// EINT0 Falling Edge
    //EIPOL1	= 0x08;		// EINT1 Falling Edge
    //EIPOL1	= 0x20;		// EINT2 Falling Edge
    
    EIPOL0	= (0x00)
			|( EINT12_FALL_EDGE << 4)		// EINT12 Edge Selection
			|( EINT11_FALL_EDGE << 2)		// EINT11 Edge Selection
			|( EINT10_RISE_EDGE << 0);	// EINT10 Edge Selection
    //EIPOL0	= 0x02;		// EINT10 Falling Edge
    //EIPOL0	= 0x08;		// EINT11 Falling Edge
    //EIPOL0	= 0x20;		// EINT12 Falling Edge
    
    eint_en();
    //eint10_en();
    //eint11_en();
    eint12_en();
#endif
}

