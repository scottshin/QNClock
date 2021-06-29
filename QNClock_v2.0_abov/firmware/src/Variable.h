//==============================================================================
//    file name   : Variable.h
//==============================================================================
//------------------------------------------------------------------------------
//    constant define
//------------------------------------------------------------------------------
#define     ON          1           //
#define     OFF         0           //
//------------------------------------
//  선택 Compile을 위한 Constant
//------------------------------------
#define		OPERATION_HZ	IRC_16MHZ
#define     POWER_DOWN_MODE STOP_MODE

//------------------------------------
#define		IRC_16MHZ		0		// fHFO/2 (default) 
#define		IRC_8MHZ		1		// fHFO/4
#define		IRC_4MHZ		2		// fHFO/8
#define		IRC_1MHZ		3		// fHFO/16

#define     LFO_128KHZ      4       // fLFO/2  0 (Defalut)
#define     LFO_64KHZ       5       // fLFO/4  1
#define     LFO_32KHZ       6       // fLFO/8  2
#define     LFO_16KHZ       7       // fLFO/16 3

#define		XTAL_16MHZ		8		
#define		XTAL_12MHZ		9		
#define		XTAL_8MHZ		10		
//------------------------------------
#define     NO_STOP         0       // Normal mode
#define     IDLE_MODE       1       //
#define     STOP_MODE       2       //
//------------------------------------
// USER HIDDEN XSFR
//------------------------------------
// FUSE_CONFIG0
//#define     FUSE_CONFIG0    *(volatile unsigned char xdata *)  0x2120
#define     FUSE_CONFIG0    *(volatile unsigned char xdata *)  0x2020
//#define     FUSE_CONFIG1    *(volatile unsigned char xdata *)  0x2121
#define     FUSE_CONFIG1    *(volatile unsigned char xdata *)  0x2021


//------------------------------------
#if		OPERATION_HZ == IRC_16MHZ
	#define 	TxDR_OSC	3999		// 0.5us * (3999 +1) = 2ms
    #define     NOP_COUNT   500         // 50ms wait * 20 = 1sec @16MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCHFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_EN<<2)|(XCLKEN_DIS<<1)|(LFOS_DIS<<0)
#elif	OPERATION_HZ == IRC_8MHZ
	#define 	TxDR_OSC	1999		// 1us * (1999 +1) = 2ms
    #define     NOP_COUNT   250         // 50ms wait * 20 = 1sec @8MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCHFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_8MHZ<<4)|(HFOS_EN<<2)|(XCLKEN_DIS<<1)|(LFOS_DIS<<0)
#elif	OPERATION_HZ == IRC_4MHZ
	#define 	TxDR_OSC	999			// 2us * (999 +1) = 2ms
    #define     NOP_COUNT   125         // 50ms wait * 20 = 1sec @4MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCHFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_4MHZ<<4)|(HFOS_EN<<2)|(XCLKEN_DIS<<1)|(LFOS_DIS<<0)
#elif	OPERATION_HZ == IRC_1MHZ
	#define 	TxDR_OSC	249			// 8us * (499 +1) = 2ms
    #define     NOP_COUNT   62          // 50ms wait * 20 = 1sec @1MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCHFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_1MHZ<<4)|(HFOS_EN<<2)|(XCLKEN_DIS<<1)|(LFOS_DIS<<0)
    
#elif	OPERATION_HZ == LFO_128KHZ
	#define 	TxDR_OSC	31			// 62.5us * (31 +1) = 2ms
    #define     NOP_COUNT   8           // 50ms wait * 20 = 1sec @1MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCLFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_DIS<<1)|(LFOS_EN<<0)    
    
#elif	OPERATION_HZ == LFO_64KHZ
	#define 	TxDR_OSC	15			// 125us * (15 +1) = 2ms
    #define     NOP_COUNT   4           // 50ms wait * 20 = 1sec @1MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCLFO<<0)
    #define     OSCCR_VALUE (LFOS_64KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_DIS<<1)|(LFOS_EN<<0)    
#elif	OPERATION_HZ == LFO_32KHZ
	#define 	TxDR_OSC	7			// 250us * (7 +1) = 2ms
    #define     NOP_COUNT   2           // 50ms wait * 20 = 1sec @1MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCLFO<<0)
    #define     OSCCR_VALUE (LFOS_32KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_DIS<<1)|(LFOS_EN<<0)
#elif	OPERATION_HZ == LFO_16KHZ
	#define 	TxDR_OSC	3			// 500us * (3 +1) = 2ms
    #define     NOP_COUNT   1           // 50ms wait * 20 = 1sec @1MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCLFO<<0)
    #define     OSCCR_VALUE (LFOS_16KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_DIS<<1)|(LFOS_EN<<0)
    
#elif	OPERATION_HZ == XTAL_16MHZ
	#define 	TxDR_OSC	3999		// 0.5us * (3999 +1) = 2ms
    #define     NOP_COUNT   500         // 50ms wait * 20 = 1sec @16MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_EXOSC<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_EN<<1)|(LFOS_DIS<<0)
#elif	OPERATION_HZ == XTAL_12MHZ
	#define 	TxDR_OSC	2999		// 0.6667us * (2999 +1) = 2ms
    #define     NOP_COUNT   375         // 50ms wait * 20 = 1sec @12MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_EXOSC<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_EN<<1)|(LFOS_DIS<<0)
#elif	OPERATION_HZ == XTAL_8MHZ
	#define 	TxDR_OSC	1999		// 1us * (1999 +1) = 2ms
    #define     NOP_COUNT   250         // 50ms wait * 20 = 1sec @8MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_EXOSC<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_DIS<<2)|(XCLKEN_EN<<1)|(LFOS_DIS<<0)
#else   //	OPERATION_HZ == IRC_16MHZ
	#define 	TxDR_OSC	3999		// 0.5us * (3999 +1) = 2ms
    #define     NOP_COUNT   500         // 50ms wait * 20 = 1sec @16MHz
    #define     SCCR_VALUE  (FWTIME_8MS<<4)|(LFOSTEN_DIS<<2)|(SYSCLK_IRCHFO<<0)
    #define     OSCCR_VALUE (LFOS_128KHZ<<6)|(HFOS_16MHZ<<4)|(HFOS_EN<<2)|(XCLKEN_DIS<<1)|(LFOS_DIS<<0)
#endif	


//------------------------------------------------------------------------------
//    I/O Port define
//------------------------------------------------------------------------------
//		P00	P01	P02	P03	P04	P05	P06	P07	 
//		I	I	I	I	I	I	I	I
//--------------------------------------
#define	pPWM1O		P03
#define	pPWM1OB		P04
//--------------------------------------
//		P10	P11	P12	P13	P14	P15	P16	P17	 
//		O	O	O	O	O	O	-	-
//--------------------------------------
#define 	pMain		P13
#define 	pRAM		P14
#define		pRESET		P15
//--------------------------------------
//		P20	P21	P22	P23	P24	P25	P26	P27
//		I	I	I	I	-	-	-	-
//--------------------------------------
#define	pT1_INT		P23
//------------------------------------------------------------------------------
//    type define
//------------------------------------------------------------------------------

typedef struct
{	Byte  b0	:1;
	Byte  b1	:1;
	Byte  b2	:1;
	Byte  b3	:1;
	Byte  b4	:1;
	Byte  b5	:1;
	Byte  b6	:1;
	Byte  b7	:1;
} BYTE_FIELD;

typedef union 
{	unsigned char byte;
	BYTE_FIELD 	b;
} TYPE_BYTE;

typedef struct
{	Byte  b8	:1;
	Byte  b9	:1;
	Byte  b10	:1;
	Byte  b11	:1;
	Byte  b12	:1;
	Byte  b13	:1;
	Byte  b14	:1;
	Byte  b15  :1;

	Byte  b0	:1;
	Byte  b1	:1;
	Byte  b2	:1;
	Byte  b3	:1;
	Byte  b4	:1;
	Byte  b5	:1;
	Byte  b6	:1;
	Byte  b7	:1;
} WORD_FIELD;

typedef union 
{
	Word	      word;
	Byte	      byte[2];
	WORD_FIELD 	b;
} TYPE_WORD;

//------------------------------------------------------------------------------
//    bit (bool) define       0x20 ~ 0x2F
//------------------------------------------------------------------------------
MyDef Bool  f_Toggle;               // i2c acknowledge in
MyDef Bool  f_Fail;                 // RAM Fail 발생되면 지운다 
MyDef Bool  f_Reset;                // POR제외한 Reset 발생하면 지운다. 
MyDef Bool  f_Flicker;              // Flicker

MyDef Bool  f_AdcTrigger;
//------------------------------------------------------------------------------
//    Data define             0x30 ~ 0xFF
//------------------------------------------------------------------------------

MyDef Byte DATA   r2mS_LED;       //
MyDef Byte DATA   r2mS_LOAD;       //

MyDef unsigned char count_500;
MyDef bit stop_flag;

//------------------------------------------------------------------------------
//    XData define            0x000 ~ 0x1FF
//------------------------------------------------------------------------------
//MyDef	Word XDATA  xBuffer[32];    // xRAM test

//==============================================================================
//	End of Variable.h



