//==============================================================================
//	file name	: Clock.c
//==============================================================================
#define		MyDef	extern		//
#include	"Global.h"			//
//------------------------------------------------------------------------------
void Initial_Clock()
{
	BITCR	= (0x00)
			|( BITFR_OFF << 7)			// BIT Intterrupt Flag. 
			|( BITCK_DIV_4096 << 5)		// BIT Clock Source Selection
			|( BCLR_CLR << 3)			// BIT Clear Counter
			|( BITOVER_BCK_X32 << 0);	// BIT overflow Period
	
    WDTMR = 0;
    
    #if (OPERATION_HZ > LFO_16KHZ)
	SYSCON_AR	=	0x55;       // to access FUSE_CONFIG0 register, SYSCON_AR should be set 0x55
    if (OPERATION_HZ > 7)       // XTAL 이면..
    {
        FUSE_CONFIG0 = 0x30;    // XTAL in,out enable
        //wait_100uSec(100);
    }
    #endif
    SYSCON_AR	=	0x5A;	// to access SCCR,OSCCR register, SYSCON_AR should be set 0x5A
    
    // 설정 시, OSCCR 우선 설정해야함. 
    // OSCCR (현재 clk & 이후 clk enable) -> SCCR 설정  -> OSCCR (이전 clk disable)
    #if 0
	OSCCR	= (0x00)
			|( LFOS_128KHZ << 6)	// Internal Low Frequency Osillator Post-Divider
			|( HFOS_16MHZ << 4)		// Internal High Frequency Osillator Post-Divider
			|( HFOS_EN << 2)		// Internal High Frequency Osillator Enable/disable
			|( XCLKEN_DIS << 1)		// External Clock Enable/disable
			|( LFOS_DIS << 0);		// Internal Low Frequency Osillator Enable/disable

	//wait_100uSec(NOP_COUNT);	        // 50ms wait
    
	SCCR	= (0x00)
			|( FWTIME_8MS << 4)		// STOP Release Time Selection
			|( LFOSTEN_DIS << 2)	// INT RC OSC(LFO) Enable in STOP Mode
			|( SYSCLK_IRCHFO << 0);	// System clock selection bit
    
    OSCCR	= (0x00)
			|( LFOS_128KHZ << 6)	// Internal Low Frequency Osillator Post-Divider
			|( HFOS_16MHZ << 4)		// Internal High Frequency Osillator Post-Divider
			|( HFOS_EN << 2)		// Internal High Frequency Osillator Enable/disable
			|( XCLKEN_DIS << 1)		// External Clock Enable/disable
			|( LFOS_DIS << 0);		// Internal Low Frequency Osillator Enable/disable
    
	#else
    OSCCR   = OSCCR_VALUE | (HFOS_EN<<2);       // OSC Enable & OSC divide selection
    wait_100uSec(NOP_COUNT/2);	                // 25msec
    SCCR    = SCCR_VALUE;                       // OSC change, Fast Wakeup setting, LFO enable at Stop mode
    OSCCR   = OSCCR_VALUE;
    #endif
    
	SYSCON_AR	=	0x00;	// to access SCCR,OSCCR register, SYSCON_AR should be set 0x5A
	
	WDTMR	= (0x00)
			|( WDT_CLK_HFO << 6)		// WDT Clock Source Selection
			|( WDT_CLK_DIV_1024 << 3)	// WDT Clock Divider
			|( WDT_CLR_FREERUN << 2)	// WDT counter Clear/Free Run
			|( WDT_RESET_DIS << 1)		// WDT Reset Enable/Disable
			|( WDT_DIS << 0);			// WDT Enable/disable
}
//------------------------------------------------------------------------------
#if 0
void	Nop_1us_16Mhz(void)
{
	NOP; NOP; NOP; NOP;	// NOP = 62.6ns(@16Mhz) 62.5ns * 16 = 1us
	NOP; NOP; NOP; NOP;
	NOP; NOP; //NOP; NOP;	// LCALL = 2 clock
	//NOP; NOP; NOP; NOP;	// RET = 4 clock
}

void	DELAY_1US(void)
{
	NOP; NOP; NOP; NOP;	// NOP = 62.6ns(@16Mhz) 62.5ns * 16 = 1us
	NOP; NOP; NOP; NOP;
	NOP; NOP; //NOP; NOP;	// LCALL = 2 clock
	//NOP; NOP; NOP; NOP;	// RET = 4 clock
}
#endif
void	DELAY_US(unsigned int us)
{
		unsigned int i;
		for( i = 0; i < us; i++) {
				NOP; NOP; NOP; NOP;	// NOP = 62.6ns(@16Mhz) 62.5ns * 16 = 1us
				NOP; NOP; NOP; NOP;
				NOP; NOP; //NOP; NOP;	// LCALL = 2 clock
		}
}
#if 0
void	DELAY_10US(void)
{
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
	DELAY_1US();
}
#endif
void	wait_100uSec(unsigned int delay)
{

		register unsigned int i;

		for( i = 0; i < (delay*10); i++)
		{
				//Nop_1us_16Mhz();
				//Nop_1us_16Mhz();
				//Nop_1us_16Mhz();
				//Nop_1us_16Mhz();
				DELAY_US(4);
		}
}

//------------------------------------------------------------------------------


