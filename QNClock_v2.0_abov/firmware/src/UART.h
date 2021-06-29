//==============================================================================
//	file name	: USART.h
//==============================================================================
#ifndef _USART_H_
#define _USART_H_
//-------------------------------------------------------
extern void USART_init(void);
extern void uputch(char character);
extern void uputs(char *s);
extern char ugetch(void);
extern char ugetchar(char *character);
extern void uputhex(unsigned char character);
extern char putchar(char character);
//-------------------------------------------------------
//	UCTRL1 (USART Control Register : 0xE1 )
//-------------------------------------------------------
#define		UMSEL_ASYNC	0	// UCTRL1[7:6]
#define		UMSEL_SYNC		1
#define		UMSEL_SPI		3

#define		UPM_NO_PARITY	0	// UCTRL1[5:4]
#define		UPM_EVEN_PARITY	2
#define		UPM_ODD_PARITY	3

#define		USIZE_5BIT		0	// UCTRL1[3:1]
#define		USIZE_6BIT		1
#define		USIZE_7BIT		2
#define		USIZE_8BIT		3
#define		USIZE_9BIT		7

#define		UDORD_LSB		0	// UCTRIL[2] at SPI Mode
#define		UDORD_MSB		1

#define		UCPOL_TXCRISE_RXCFALL	0	// UCTRIL[0] at SPI & ASYNC Mode 
#define		UCPOL_TXCFALL_RXCRISE	1
//-------------------------------------------------------
//	UCTRL2 (USART Control Register : 0xE2 )
//-------------------------------------------------------
#define		UDRIE			0x80	
#define		TXCIE			0x40	
#define		RXCIE			0x20	
#define		WAKEIE			0x10	
#define		TXE				0x08	
#define		RXE				0x04	
#define		USARTEN			0x02	
#define		U2X				0x01	
//-------------------------------------------------------
//	UCTRL3 (USART Control Register : 0xE3 )
//-------------------------------------------------------
#define		MASTER			0x80
#define		LOOP			0x40
#define		DISXCK			0x20
#define		SPISS			0x10
#define		USBS			0x04
#define		TX8				0x02
#define		RX8				0x01
//-------------------------------------------------------
//	USTAT (USART Control Register : 0xE4 )
//-------------------------------------------------------
#define		UDRE			0x80
#define		TXC				0x40
#define		RXC				0x20
#define		WAKE			0x10
#define		SOFTRST			0x08
#define		DOR				0x04
#define		FE				0x02
#define		PE				0x01
 
//==============================================================================
#ifndef	NULL
	#define		NULL	0
#endif
	
#define INT_MODE	1
#define	QUEUE_SIZE	8

#define	DELIVERY_DATA_TO_NULL	0

extern void USART_init(void);
extern void uputch(char character);
extern char ugetch(void);
extern unsigned char UART_read(void);
extern void UART_write(unsigned char TX_data);

extern void uputhex(unsigned char character);
extern char putchar(char character);

extern void putstring(char *str);

extern char tx_queue_is_full();
extern char rx_queue_is_full();
extern char tx_queue_is_empty();
extern char rx_queue_is_empty();

extern char _tx_queue[ QUEUE_SIZE ];
extern char _tx_front, _tx_rear;

extern char _rx_queue[ QUEUE_SIZE ];
extern char _rx_front, _rx_rear;

#endif

