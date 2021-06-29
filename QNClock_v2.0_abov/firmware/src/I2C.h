#ifndef	_I2C_
#define	_I2C_

void  I2c_Start();
void  I2c_Stop();
void  I2c_Ack();
void  I2c_Send(UCHAR dat);
UCHAR I2c_Read(void);


void  I2c_WriteData(UCHAR Slave, UCHAR Addr, UCHAR dat);
UCHAR I2c_ReadData(UCHAR Slave, UCHAR Addr);
void  Init_I2CC();
void	DELAY_1US(void);
void	DELAY_10US(void);
void CLK_ReadData(UCHAR Slave);
extern volatile unsigned char secj;
extern volatile unsigned char minj;
extern volatile unsigned char hourj;
extern volatile unsigned char dayj;
extern void I2C_Process(void);

#define RTC_SLAVE_ADDR	0xD0

#ifdef I2C_GLOBALS
#define I2C_EXT
#else
#define I2C_EXT	extern
#endif

//=============================================================================
//=============================================================================
#define     SLAVE_ADDRESS     0xD0		// slave address	//EEPROM
#define     SELF_ADDRESS0     0x40		//0x06        // slave mode
#define     SELF_ADDRESS1     0x41		//
//=============================================================================
#define     I2C_DEVICE_ID     0xC8     // device ID  
#define     I2C_CONTROL_ID    0xAA     // dummy control ID(abnormal protection)    

#define		SW_VERSION		// D6000/D7000 TV acceptable SW ID
#define		HW_VERSION		// D6000/D7000 TV acceptable HW ID.
//----------------------------------------------------------
//     I2CMR       [0xDA]
//----------------------------------------------------------
#define     IIF   0x80        // bit7 =0:I2C interrupt request
#define     IICEN 0x40        // bit6 =1:I2C block active
#define     SWRST 0x20        // bit5 =1:I2C block software initialize
#define     INTEN 0x10        // bit4 =1:Interrupt enable
#define     ACKEN 0x08        // bit3 =1:ACK out enable
#define     IMASTER 0x04       // bit2 =1:Master mode ID (read only)
#define     STP   0x02        // bit1 =1:STOP generate
#define     SRT   0x01        // bit0 =1:START generate

//----------------------------------------------------------
//    I2CSR       [0xDB]      // when read, contents isn't changed
//----------------------------------------------------------
#define     GCALL  0x80       // bit7 =1:General Call detected
#define     TEND   0x40       // bit6 =1:1byte tx completed
#define     STPID  0x20       // bit5 =1:STOP detected
#define     SSEL   0x10       // bit4 =1:Slave Address matched
#define     MLOST  0x08       // bit3 =1:arbtration lost
#define     BUSY   0x04       // bit2 =1:BUS busy
#define     TMODE  0x02       // bit1 =1:TX mode
#define     RXACK  0x01       // bit0 =1:acknowledge received at 9th scl
//----------------------------------------------------------
//=============================================================================

#define I2CMASTER

#define	TP00	P20

extern volatile unsigned char I2C_Buffer0[64];
extern volatile unsigned char I2C_Buffer[64];
extern BYTE	g_fKeyValueAccess;
extern BYTE	I2C_KeyValue;

extern void I2CIntHandler(void);
extern void Init_I2C(void);

extern void I2C_Initial(void);
extern void Init_I2C_Master(void);
extern void I2C_MasterWrite(unsigned char* i2cdata, unsigned char length);
extern void I2C_MasterRead(unsigned char length);
extern void t_I2CM(void);
extern void	DELAY_US(unsigned int ms);

//=============================================================================


#endif
