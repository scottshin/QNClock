/****************************************************************************

					UART interface (UART4)

*****************************************************************************/
//#include  <stdio.h>
//#include  <stdarg.h>
//==============================================================================

#define	MyDef	extern
#include	"Global.h"	

char _tx_queue[ QUEUE_SIZE ];
char _tx_front, _tx_rear;

char _rx_queue[ QUEUE_SIZE ];
char _rx_front, _rx_rear;

char tx_queue_is_full()
{
	return ( (_tx_rear + 1 ) % QUEUE_SIZE == _tx_front );
}

char tx_queue_is_empty()
{
	return ( _tx_rear == _tx_front );
}

char rx_queue_is_full()
{
	return ( (_rx_rear + 1 ) % QUEUE_SIZE == _rx_front );
}

char rx_queue_is_empty()
{
	return ( _rx_rear == _rx_front );
}

void USART_RXInt_Handler() interrupt USART_RX_VECT
{
	unsigned char temp, temp2;
	
	temp    = USTAT;
	temp2   = UCTRL2;
	
	if ( rx_queue_is_full () )
	{
		if ((temp & DOR )||(temp & FE )||(temp & PE ))
		{
			_rx_queue [ _rx_rear ] = UDATA;
			_rx_rear = ++_rx_rear % QUEUE_SIZE;
		}
		_rx_queue [ _rx_rear ] = UDATA;
		_rx_rear = ++_rx_rear % QUEUE_SIZE;
			
		_rx_front = _rx_rear = 0;
		#if DELIVERY_DATA_TO_NULL
			_rx_queue[ _rx_front ] = NULL;
		#else
			_rx_queue[ _rx_front ] = 0;
		#endif
	}
	else
	{
		_rx_queue [ _rx_rear ] = UDATA;
		_rx_rear = ++_rx_rear % QUEUE_SIZE;
	}
}

void USART_TXInt_Handler() interrupt USART_TX_VECT
{
	if ( tx_queue_is_empty() == TRUE )
	{
		if ( (USTAT & (TXC)) )
		{
			UCTRL2	&= ~( UDRIE + TXCIE );
			USTAT	&= ~( UDRE + TXC );
		}
		else
		{
			UCTRL2	&= ~( UDRIE );
		}
	}
	else
	{
		UDATA = _tx_queue [ _tx_front ];
		
		#if DELIVERY_DATA_TO_NULL
			_tx_queue [ _tx_front ] = NULL;
		#else
			_tx_queue [ _tx_front ] = 0;
		#endif
			_tx_front = ++_tx_front % QUEUE_SIZE;
	}
}

//------------------------------------------------------------------------------
void USART_init(void)
{
	// initialize UART interface
	// ASync. 19200 bps, 8bit data, no parity, 1 stop bit
	USTAT	|= SOFTRST;
	
	_rx_front = _rx_rear = 0;
         _rx_queue[ _rx_front ] = NULL;
    
	_tx_front = _tx_rear = 0;
        _rx_queue[ _rx_front ] = NULL;
	
	UBAUD = 0x33;						
	
	UCTRL1	= (0x00)
	|( UMSEL_ASYNC << 6)				// Mode Selection 
	|( UPM_NO_PARITY << 4)				// Parity Selection
	|( USIZE_8BIT << 1)					// Length of data bit in frame.
	|( UCPOL_TXCRISE_RXCFALL << 0);		// Sync & SPI Pol selection

	UCTRL3 = 0x00;  					// 1 stop bit
	
#if INT_MODE	
	UCTRL2	|=	RXCIE + TXE + RXE + USARTEN;
	IE1 |= 0x06;   	// enable UART RX/TX interrupt	
#else 	
	UCTRL2	|=	TXE + RXE + USARTEN;
#endif	
}

//------------------------------------------------------------------------------
char putchar(char character)		// for Keil printf function
{
	if(character == '\n')
	{
		while(!(USTAT & (UDRE)));	
		UDATA = '\r';
	}
	while(!(USTAT & (UDRE)));
	
	UDATA = character;
	return character;
}
//------------------------------------------------------------------------------
void uputch(char character)
{
#if INT_MODE
	
	if( (UCTRL2 & UDRIE) || (UCTRL2 & TXCIE))
 	{
		if ( tx_queue_is_empty() == FALSE )	
		{
			while ( tx_queue_is_full() == TRUE );
		}
 	}
	
	uarttxint_dis();
	_tx_queue [ _tx_rear ] = character;
	_tx_rear = ++_tx_rear % QUEUE_SIZE;
	UCTRL2 |= ( UDRIE );
	uarttxint_en();
		
#else
	while(!( USTAT & UDRE ));	// TX buffer empty check.
	UDATA = character;	
#endif
	
}

void putstring(char *str)
{	
	while(*str != NULL)
	{
		uputch(*str++);
	}
}

char ugetch(void)
{
	char character;
#if INT_MODE
	while ( rx_queue_is_empty() == TRUE );
	
	uartrxint_dis();
	character = _rx_queue [ _rx_front ];

	#if DELIVERY_DATA_TO_NULL
		_rx_queue[ _rx_front ] = NULL;
	#endif

	_rx_front = ++_rx_front % QUEUE_SIZE;
	uartrxint_en();
#else
	while(( USTAT & RXC)!=RXC);
	character	=	UDATA;	
#endif 
	
	return character;
}

unsigned char UART_read(void)
{
	unsigned char RX_data;
	
	while(!(USTAT & RXC));
	RX_data = UDATA;
	
	return	RX_data;
}

void UART_write(unsigned char TX_data)
{
	while(!(USTAT & UDRE));
	UDATA = TX_data;
}

void uputhex(unsigned char _Hexdecimal)
{
	unsigned int temp;
	char i;

	for (i=1;i>=0;i--){
		temp = _Hexdecimal;
		temp >>= 4*i;
		temp &= 0x0f;

		if (temp>=10) {
			temp-=10;
			temp+='a';
		}
		else 
			temp+='0';
		putchar(temp);
	}
}


