//==============================================================================
//    file name   : Clock.h
//==============================================================================
//	SCCR
#define	FWTIME_128US	0	// SCCR[6:4]
#define	FWTIME_256US	1
#define	FWTIME_512US	2
#define	FWTIME_4MS		3
#define	FWTIME_8MS		4	// default
#define	FWTIME_16MS		5
#define	FWTIME_32MS		6
#define	FWTIME_64MS     7

#define	LFOSTEN_DIS		0	// SCCR[2]
#define	LFOSTEN_EN		1

#define	SYSCLK_IRCHFO	0	// SCCR[1:0] default
#define	SYSCLK_EXOSC	1
#define	SYSCLK_IRCLFO	2

//	OSCCR
#define	LFOS_128KHZ		0	// OSCCR[7:6] default
#define	LFOS_64KHZ		1
#define	LFOS_32KHZ		2
#define	LFOS_16KHZ		3

#define	HFOS_16MHZ		0	// OSCCR[5:4] default
#define	HFOS_8MHZ		1
#define	HFOS_4MHZ		2
#define	HFOS_1MHZ		3


#define	HFOS_DIS		0	// OSCCR[2]
#define	HFOS_EN			1	// default

#define	XCLKEN_DIS		0	// OSCCR[1] default
#define	XCLKEN_EN		1

#define	LFOS_DIS		0	// OSCCR[0] default
#define	LFOS_EN			1

// BITCR
#define	BITFR_OFF		0	// BITCR[7] default
#define	BITFR_ON		1

#define	BITCK_DIV_4096	0	// BITCR[6:5] default
#define	BITCK_DIV_1024	1
#define	BITCK_DIV_128	2
#define	BITCK_DIV_16	3

#define	BCLR_FREERUN	0	// BITCR[3]
#define	BCLR_CLR		1

#define	BITOVER_BCK_X2		0	// BITCR[2:0]
#define	BITOVER_BCK_X4		1
#define	BITOVER_BCK_X8		2
#define	BITOVER_BCK_X16		3
#define	BITOVER_BCK_X32		4	// default
#define	BITOVER_BCK_X64		5
#define	BITOVER_BCK_X128	6
#define	BITOVER_BCK_X256	7

// WDTMR
#define	WDT_CLK_HFO			0	// WDTMR[7:6]
#define	WDT_CLK_LFO			1
#define	WDT_CLK_BITOVER		2

#define	WDT_CLK_DIV_1		0	// WDTMR[5:3]
#define	WDT_CLK_DIV_2		1
#define	WDT_CLK_DIV_4		2
#define	WDT_CLK_DIV_8		3
#define	WDT_CLK_DIV_128		4
#define	WDT_CLK_DIV_256		5
#define	WDT_CLK_DIV_512		6
#define	WDT_CLK_DIV_1024	7

#define	WDT_CLR_FREERUN		0	// WDTMR[2]
#define	WDT_CLR_CLR			1

#define	WDT_RESET_DIS	0	// WDTMR[1]
#define	WDT_RESET_EN	1

#define	WDT_DIS			0	// WDTMR[0]
#define	WDT_EN			1
//------------------------------------------------------------------------------
extern void Initial_Clock();
extern void	wait_100uSec(unsigned int delay);

//==============================================================================