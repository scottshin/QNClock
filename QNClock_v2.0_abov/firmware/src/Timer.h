//==============================================================================
//	file name	: Timer.h
//==============================================================================
// T0CR
#define	T0_DISABLE			0	//T0CR[7]
#define	T0_ENABLE			1

#define	T0_PORTOUT_DISABLE	0	//T0CR[6]
#define	T0_PORTOUT_ENABLE	1

#define	T0_MODE_COUNTER		0	//T0CR[5:4]
#define	T0_MODE_PWM			1
#define	T0_MODE_CAPTURE		2

#define	T0_CLKDIV_2			0	//T0CR[3:1]
#define	T0_CLKDIV_4			1
#define	T0_CLKDIV_8			2
#define	T0_CLKDIV_32		3
#define	T0_CLKDIV_128		4
#define	T0_CLKDIV_512		5
#define	T0_CLKDIV_2048		6
#define	T0_CLK_EXTERNAL		7

#define	T0_COUNT_NOEFFET	0	//T0CR[0]	
#define	T0_COUNT_CLR		1
//---------------------------
#define	T0EN	7
#define	T0PN	6
#define	T0MS	4
#define	T0CK	1
#define	T0CC	0
//------------------------------------------------------------------------------
// T1CRH
#define	T1_DISABLE			0	//T1CRH[7]
#define	T1_ENABLE			1

#define	T1B_DISABLE			0	//T1CRH[6]
#define	T1B_ENABLE			1

#define	T1_MODE_COUNTER		0	//T1CRH[5:4]
#define	T1_MODE_CAPTURE		1
#define	T1_MODE_PWMONESHOT	2
#define	T1_MODE_PWMREPEAT	3

#define	T1_PORTOUT_DISABLE	0	//T1CRH[1]
#define	T1_PORTOUT_ENABLE	1

#define	T1_COUNT_NOEFFET	0	//T1CRH[0]	
#define	T1_COUNT_CLR		1
//---------------------------
#define	T1EN	7
#define	T1BEN	6
#define	T1MS	4
#define	T1PE	1
#define	T1CC	0
//---------------------------
// T1CRL
#define	T1_CLKDIV_2048		0	//T1CRL[7:5]
#define	T1_CLKDIV_64		1
#define	T1_CLKDIV_8			2
#define	T1_CLKDIV_4			3
#define	T1_CLKDIV_2			4
#define	T1_CLKDIV_1			5
#define	T1_CLK_32MHZ		6
#define	T1_CLK_EXTERNAL		7

#define	T1_REQUEST_OFF		0	//T1CRL[4]
#define	T1_REQUEST_ON		1

#define	T1B_POL_START_HIGH	0	//T1CRL[3]
#define	T1B_POL_START_LOW	1

#define	T1_POL_START_HIGH	0	//T1CRL[2]
#define	T1_POL_START_LOW	1

#define	T1_CLK_EXT_FALLEDGE	0	//T1CRL[1]
#define	T1_CLK_EXT_RISEEDGE	1

#define	T1_COUNT_LOAD_NOEFFET	0	//T1CRL[0]
#define	T1_COUNT_LOAD_B			1	
//---------------------------
#define	T1CK	5
#define	T1IFR	4
#define	T1BPOL	3
#define	T1POL	2
#define	T1ECE	1
#define	T1CNTR	0
//------------------------------------------------------------------------------
// T2CRH
#define	T2_DISABLE			0	//T2CRH[7]
#define	T2_ENABLE			1

#define	T2_MODE_COUNTER		0	//T2CRH[5:4]
#define	T2_MODE_CAPTURE		1
#define	T2_MODE_PWMONESHOT	2
#define	T2_MODE_PWMREPEAT	3

#define	T2_PORTOUT_DISABLE	0	//T2CRH[1]
#define	T2_PORTOUT_ENABLE	1

#define	T2_COUNT_NOEFFET	0	//T2CRH[0]	
#define	T2_COUNT_CLR		1
//---------------------------
#define	T2EN	7
#define	T2MS	4
#define	T2PE	1
#define	T2CC	0
//---------------------------
// T2CRL
#define	T2_CLKDIV_2048		0	//T2CRL[7:5]
#define	T2_CLKDIV_64		1
#define	T2_CLKDIV_8			2
#define	T2_CLKDIV_4			3
#define	T2_CLKDIV_2			4
#define	T2_CLKDIV_1			5
#define	T2_CLK_32MHZ		6
#define	T2_CLK_EXTERNAL		7

#define	T2_REQUEST_OFF		0	//T2CRL[4]
#define	T2_REQUEST_ON		1

#define	T2_POL_START_HIGH	0	//T2CRL[2]
#define	T2_POL_START_LOW	1

#define	T2_CLK_EXT_FALLEDGE	0	//T2CRL[1]
#define	T2_CLK_EXT_RISEEDGE	1

#define	T2_COUNT_LOAD_NOEFFET	0	//T2CRL[0]
#define	T2_COUNT_LOAD_B			1	
//---------------------------
#define	T2CK	5
#define	T2IFR	4
#define	T2POL	2
#define	T2ECE	1
#define	T2CNTR	0
//---------------------------
// EIPOL0 
#define	EINT12_NO_EDGE		0	// EIPOL0[5:4]
#define	EINT12_RISE_EDGE	1
#define	EINT12_FALL_EDGE	2
#define	EINT12_BOTH_EDGE	3

#define	EINT11_NO_EDGE		0	// EIPOL0[3:2]
#define	EINT11_RISE_EDGE	1
#define	EINT11_FALL_EDGE	2
#define	EINT11_BOTH_EDGE	3

#define	EINT10_NO_EDGE		0	// EIPOL0[1:0]
#define	EINT10_RISE_EDGE	1
#define	EINT10_FALL_EDGE	2
#define	EINT10_BOTH_EDGE	3
//---------------------------
// EIPOL1 
#define	EINT2_NO_EDGE		0	// EIPOL1[5:4]
#define	EINT2_RISE_EDGE		1
#define	EINT2_FALL_EDGE		2
#define	EINT2_BOTH_EDGE		3

#define	EINT1_NO_EDGE		0	// EIPOL1[3:2]
#define	EINT1_RISE_EDGE		1
#define	EINT1_FALL_EDGE		2
#define	EINT1_BOTH_EDGE		3

#define	EINT0_NO_EDGE		0	// EIPOL1[1:0]
#define	EINT0_RISE_EDGE		1
#define	EINT0_FALL_EDGE		2
#define	EINT0_BOTH_EDGE		3

//------------------------------------------------------------------------------
void Timer0_Initial(void);
void Timer1_Initial(void);
void Timer2_Initial(void);

void External_INT_Initial(void);

void ExINT_init();

extern volatile unsigned int delayCnt2;
extern volatile unsigned char pre_p0;

extern unsigned char SWITCH_A;
extern unsigned char SWITCH_B;
extern unsigned char SWITCH_R;
extern unsigned char RTC_INTERRUPT;
extern UCHAR mode;			// 1:clock set
extern UCHAR Pre_Status;

extern UCHAR BB;
//==============================================================================
