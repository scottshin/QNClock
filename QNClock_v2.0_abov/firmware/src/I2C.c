//==============================================================================
//#include  <stdio.h>
//#include  <stdarg.h>
#define		MyDef	extern		//
#include	"Global.h"			//
//==============================================================================

//static 	unsigned char fSubAddr = TRUE;
//static 	unsigned char I2C_SubAddr = 0;
extern void	Nop_1us_16Mhz(void);

//sbit   	P15	        =   0x8D;
//sbit   	P14	        =   0x8C;
sbit   	pSDA        =   0x91;
sbit   	pSCL        =   0x90;

//#define BUFSIZE 10

//unsigned char I2CBuffer[BUFSIZE];
//unsigned char ReadI2CBuffer[BUFSIZE] = {0, };

//unsigned char I2CLen;
//unsigned char I2CIdx;

//unsigned char debug_cnt;


#define SCL_HIGH()  P20 = 1
#define SCL_LOW()		P20 = 0
#define SDA_HIGH()	P21 = 1
#define SDA_LOW()		P21 = 0

#define SDA_SETI()	P2IO &= ~0x02
#define SDA_SETO()	P2IO |= 0x02
#define SDA_READ()	P21

/**********************************************
			START
***********************************************/
void I2c_Start()
{
		SDA_SETO();
		SDA_HIGH();
		SCL_HIGH();
		DELAY_US(2);	
	
		SDA_LOW();
		DELAY_US(5);
	
		SCL_LOW();  
		DELAY_US(5);	
}

/**********************************************
			STOP
***********************************************/
void I2c_Stop()
{
		SDA_SETO();
	
		SDA_LOW();
		DELAY_US(5);
	
		SCL_HIGH();
		DELAY_US(5);
	
		SDA_HIGH(); 
		DELAY_US(5);
}

/**********************************************
			WRITE_ACK
***********************************************/
void I2c_Write_Ack(void)
{
		volatile UCHAR wait = 10;
	
		//SDA_HIGH();
		DELAY_US(1);	
	
		SDA_SETI();
		DELAY_US(5);
	
		SCL_HIGH();
		while(wait--) {
		if(SDA_READ() == 0)
				break;
		}
		DELAY_US(5);
		SCL_LOW();
		
		SDA_SETO();
 
		DELAY_US(1);
}

/**********************************************
			READ_ACK
***********************************************/
void I2c_Read_Ack(void)
{
		SDA_SETO();
		DELAY_US(1);
	
		SDA_LOW();
		DELAY_US(5);	
		
		SCL_HIGH();
		DELAY_US(5);
		SCL_LOW();
	
		DELAY_US(1);
}

/**********************************************
			READ_NACK
***********************************************/
void I2c_Read_Nack(void)
{
		SDA_SETO();
	
		SDA_HIGH();
		DELAY_US(5);	
		
		SCL_HIGH();
		DELAY_US(5);
		SCL_LOW();
	
		SDA_LOW();
}


/**********************************************

***********************************************/
void I2c_Write(UCHAR dat)
{
		volatile UCHAR index = 8;

		SCL_LOW();
		DELAY_US(5);		

		while(index--) {
				if((dat & 0x80) == 0)
						SDA_LOW();
				else
						SDA_HIGH();
				DELAY_US(5);
				
				SCL_HIGH();
				DELAY_US(5);
				SCL_LOW();
				
				DELAY_US(1);
				dat <<= 1;
		}
}

/**********************************************

***********************************************/
UCHAR I2c_Read(void)
{
		volatile UCHAR index = 8;
		volatile UCHAR Data = 0;

		SDA_SETI();
		DELAY_US(1);

		while(index--) {
				DELAY_US(4);
				Data <<= 1;
				SCL_HIGH();
				DELAY_US(3);
		
				if(SDA_READ() == 1)
						Data |= 1;	
				DELAY_US(2);
				SCL_LOW();
				
				DELAY_US(1);
		}

		SDA_SETO();
	
		return Data;
}

/**********************************************

***********************************************/
void DS3231_Write(UCHAR Data[], UCHAR Addr, UCHAR Len)
{
		UCHAR i;
		I2c_Start();	
		I2c_Write(SLAVE_ADDRESS);
		I2c_Write_Ack();	
	
		I2c_Write(Addr);
		I2c_Write_Ack();
	
		//I2c_Start();
		//I2c_Write(SLAVE_ADDRESS);
		//I2c_Write_Ack();
	
		for( i = 0; i < Len; i++) {
				I2c_Write(Data[Addr + i]);
				I2c_Write_Ack();
		}
		
		I2c_Stop();
}

/**********************************************

***********************************************/
void DS3231_Read(UCHAR Data[], UCHAR Addr, UCHAR Len)
{
		UCHAR i;
		I2c_Start();	
		I2c_Write(SLAVE_ADDRESS);
		I2c_Write_Ack();	
	
		I2c_Write(Addr);
		I2c_Write_Ack();
	
		I2c_Start();
		I2c_Write(SLAVE_ADDRESS+1);
		I2c_Write_Ack();
	
		for( i = 0; i < Len; i++) {
				Data[Addr + i] = I2c_Read();
				if( i != ( Len - 1 ) )
						I2c_Read_Ack();
				else
						I2c_Read_Nack();
		}
		
		I2c_Stop();
}

#if 0
/**********************************************

***********************************************/
void I2C_TestWrite(UCHAR Slave_addr, UCHAR Data[], UCHAR Len)
{	
		UCHAR i;
		I2c_Start();	
		I2c_Write(Slave_addr);
		I2c_Write_Ack();

		for( i = 0; i < Len; i++) {
				I2c_Write(Data[i]);
				I2c_Write_Ack();
		}

		I2c_Stop();
}

void I2C_TestRead(UCHAR Slave_addr, UCHAR Data[], UCHAR Len)
{	
		UCHAR i = 0;
	
		I2c_Start();
	
		I2c_Write(Slave_addr + 1);
		I2c_Write_Ack();
		
		for( i = 0; i < Len-1; i++) {
				Data[i]= I2c_Read();
				I2c_Read_Ack();
		}
		
		Data[i]= I2c_Read();
		I2c_Read_Nack();
		I2c_Stop();
}

/**********************************************

***********************************************/
UCHAR I2c_ReadByte(UCHAR Slave, UCHAR Addr)
{
		UCHAR Data;
	
		I2c_Start();
		I2c_Write(Slave);
		I2c_Write_Ack();
	
		I2c_Write(Addr);
		I2c_Write_Ack();
	
		I2c_Start();
		I2c_Write(Slave | 0x01);
		I2c_Write_Ack();
	
		Data = I2c_Read();

		I2c_Read_Nack();
		I2c_Stop();
	
		return Data;
}


/**********************************************

***********************************************/
void Init_I2CC()
{
  SDA_SETO();

  SDA_HIGH();
  SCL_HIGH();
}
#endif
/**********************************************

**********************************************/
#if 0
void write_i2c_byte(unsigned char byte) { 
		unsigned char i = 0; 
		SDA_SETO(); // Set SDA to output 
		for (i = 0; i < 8 ; i++) { 
				if((byte & 0x80)==0x80) SDA_HIGH(); // Set SDA High 
				else 										SDA_LOW(); // Clear SDA Low 
				DELAY_US(5);
			
				SCL_HIGH(); // Set SCL High, Clock data 
				DELAY_US(5); 
				byte = byte << 1; // Shift data in buffer right one 
				SCL_LOW(); // Clear SCL 
			
				DELAY_US(1); 
		} 
		//SDA_LOW(); //listen for ACK 
		SDA_SETI(); 
		DELAY_US(5);
		
		SCL_HIGH(); 
		DELAY_US(5);
		SCL_LOW(); 
		
		//DELAY_US(20); //Clear SCL. 
		SDA_SETO(); // Set SDA to Output 
}

/**********************************************

**********************************************/
void write_i2c_bytes(UCHAR Slave_Addr, UCHAR Data) { 
		I2c_Start();
		write_i2c_byte(Slave_Addr);
		write_i2c_byte(Data);
		//write_i2c_byte(Data);
		//write_i2c_byte(Data);
		I2c_Stop();
}
#endif
#if 0
//=============================================================================
void I2C_Wait()
{
	Nop_1us_16Mhz();
	Nop_1us_16Mhz();
}

//=============================================================================
void Init_I2C(void)
{
	I2CMR	=	IICEN;                // I2C block enable!
	I2CMR	|=	SWRST;               // I2C software clear!

	I2CSAR	=	SELF_ADDRESS0;        
	I2CSAR1	=	SELF_ADDRESS1;        
	I2CMR	=	IICEN+INTEN+ACKEN+IMASTER;    // I2C interrupt enable!

	I2CSCLLR = 20;		// about 100KHz, 2 : 400Khz
	I2CSCLHR = 20;
	I2CSDAHR = 20;		// hold time : scl 'H' period 
	
	//I2CMR1 = 0x01;
      
  i2cint_en();
}

//=============================================================================
static void I2C_Error(void)
{
    I2CMR       |= SWRST;                // I2C software clear!
    I2CSAR      = SELF_ADDRESS0; 
	  I2CSAR1      = SELF_ADDRESS0;
    I2CMR       = IICEN+INTEN+ACKEN;    // I2C interrupt enable !

	  I2CSDAHR = 20;		// hold time : scl 'H' period 
	  //I2CMR1 = 0x01;
}

void I2C_Initial(void)
{
	I2CMR = IICEN;
	I2CMR |= SWRST;

#if 1	// fI2C = 200kHz @ sysclk 16MHz
	// Low period = 1/16M * (4 * (I2CSCLLR+1)) = 2.5us
	// High period = 1/16M * (4 * (I2CSCLHR+1)) = 2.5us
	// fI2C = 1/5us = 200kHz
	I2CSCLLR = 0x09;
	I2CSCLHR = 0x09;
#elif 0	// fI2C = 100kHz @ sysclk 16MHz	
	// Low period = 1/16M * (4 * (I2CSCLLR+1)) = 5us
	// High period = 1/16M * (4 * (I2CSCLHR+1)) = 5us
	// fI2C = 1/10us = 100kHz
	I2CSCLLR = 19;
	I2CSCLHR = 19;
#elif 0	// fI2C = 100kHz @ sysclk 10MHz	
	// Low period = 1/10M * (4 * (I2CSCLLR+1)) = 4.8us
	// High period = 1/10M * (4 * (I2CSCLHR+1)) = 5.2us
	// fI2C = 1/10us = 100kHz
	I2CSCLLR = 11;
	I2CSCLHR = 12;
#endif

	I2CMR1 |= (1<<0);
	I2CMR = IICEN+INTEN+ACKEN+IMASTER;
	
	i2cint_en();
}
#endif
#if 0
void I2C_Write(unsigned char* i2cdata, unsigned char length)
{
	unsigned char i;
	
	for(i=0; i<length; i++)
	{
		I2CBuffer[i] = i2cdata[i];
	}
	
	I2CLen = length;
	I2CIdx = 0;

	I2CMR = IICEN+INTEN;
	I2CDR = SLAVE_ADDRESS;
	
	I2CMR |= SRT;
}
#endif