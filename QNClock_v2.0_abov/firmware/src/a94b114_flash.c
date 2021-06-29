/**
*   @file       a96g174_flash.c
*   @brief      A96G174 flash control function 
*   @author     Application Team, ABOV Semiconductor Co., Ltd.
*   @version    V1.00
*   @date       15. JAN. 2020
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
********************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE - PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers' products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
*/

/*******************************************************************************
* Included File
*******************************************************************************/
#include	"a94b114_flash.h"	

/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
// User ID global value
unsigned int User_ID;

unsigned char *PageBufData;
unsigned int PageBufDataSize;

volatile unsigned char data code_buf[20] _at_ 0x30;		// Lower 128bytes internal RAM. (GPR Address 0x30~0x7F)
volatile unsigned char data g_FEMR, g_FECR, g_WDTMR;
volatile unsigned char g_IE, g_IE1, g_IE2;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
/**
* @brief	Ready to set Flash controller before operating flash memory.
* @param    None
* @return	None
*/
void Flash_Enter(void)
{
	g_IE = IE;
	g_IE1 = IE1;
	g_IE2 = IE2;
	IE = 0;
	IE1 = 0;
	IE2 = 0;
	
	g_WDTMR = WDTMR;	// Backup WDTMR
	WDTMR	= 0;		// Watch Dog Timer Stop.
	g_WDTMR |= 0x04;	// Clear WDT Counter
	
	// To access Flash Control register, ENTRY_0~2 should be set 0xAA, 0x55, 0xA5.
	*(unsigned char xdata *)0x10D8 = 0xAA;
	*(unsigned char xdata *)0x10DA = 0x55;
	*(unsigned char xdata *)0x10DD = 0xA5;
	
	// Set Flash Sector Address as Dummy address
	FEARH = (FLASH_ADDR_DUMMY >> 16) & 0x0f;	//Flash Sector Address High Register
	FEARM = (FLASH_ADDR_DUMMY >> 8) & 0xff;	//Flash Sector Address Middle Register
	FEARL = (FLASH_ADDR_DUMMY >> 0) & 0xff;	//Flash Sector Address Low Register
	
}

/**
* @brief	Ready to set Flash controller after operating flash memory.
* @param    None
* @return	None
*/
void Flash_Exit(void)
{
	FECR = 0x33; //FLASH_MODE_EXIT
	// After to change Flash Control register, ENTRY_0~2 should be set 0x00.
	*(unsigned char xdata *)0x10D8 = 0x00;
	*(unsigned char xdata *)0x10DA = 0x00;
	*(unsigned char xdata *)0x10DD = 0x00;

		// Flash code buffer ram clear.
#pragma asm
	mov		r0, #code_buf
	mov		r2, #20
	clr		a
loop0:
	mov		@r0, a
	inc		r0
	djnz	r2, loop0
#pragma endasm

	WDTMR	= g_WDTMR;	// Restore WDTMR

	/* restore IE setting */
	IE = g_IE;
	IE1 = g_IE1;
	IE2 = g_IE2;
}


//==============================================================================
// Sram-jump Program Routine.
//==============================================================================
// Do not change this routine.
/**
* @brief	Ready to write Flash page buffer in ram(must execute in SRAM!, 0 ~ 0x11(code size : 18 byte))
* @param    None
* @return	None
*/
volatile void write_page_buffer_src()		
{
	FEMR = 0x89;//
			//FLASH_SEL				// FSEL (Select Flash memory)
			//| FLASH_PBUFF_SEL	// PBUFF (Select page buffer)
			//| FLASH_CON_ENABLE;	// FEEN (Enable program & erase)

#pragma asm

	mov		r0,#32
	mov		dptr,#0x10E0	; page buffer address
	write_page_buffer_src_loop:
	mov		a, @r1				;r1=addr of rxdata
	movx	@dptr,a
	inc		r1
	inc		dptr        
	djnz	r0,write_page_buffer_src_loop
#pragma endasm

	FEMR = 0;
}

/**
* @brief	Write flash page buffer
* @param    None
* @return	None
*/
void write_page_buffer(unsigned char *dt)	// dt -> r3, r2, r1	
{
	u8 *temp = dt;
	
#pragma asm
	mov		dptr,#write_page_buffer_src
	mov		r0,#0x30
	mov		r2,#0x12
write_page_buffer_loop:
	clr		a
	movc	a,@a+dptr
	mov		@r0,a
	inc		dptr
	inc		r0
	djnz	r2,write_page_buffer_loop
			ljmp	0x8030		; jump sram region
#pragma endasm
}


//==============================================================================
// Sram-jump Program Routine.
//==============================================================================
// Do not change this routine.
/**
* @brief	Ready to write or erase flash page in ram(must execute in SRAM!, 0 ~ 0x0E(code size : 15 byte))
* @param    None
* @return	None
*/
volatile void do_flash_at_sram_src()		
{
	#pragma	asm
		mov	FEMR,g_FEMR				// 3 byte
		mov	FECR,g_FECR				// 3 byte
	_flash_busy_wait:
		mov	A,FESR					// 2 byte
		JNB	ACC.7,_flash_busy_wait	// 3 byte
		clr	A						// 1 byte
		mov	FEMR,A					// 2 byte
	#pragma endasm
}	// RET							 // 1byte

/**
* @brief write or erase flash page
* @param    None
* @return	None
*/
void do_flash_at_sram()
{
    #pragma asm
		mov		dptr,#do_flash_at_sram_src
		mov		r0,#0x30
		mov		r1,#0x0F
	do_flash_at_sram_loop:
		clr		a
		movc	a,@a+dptr
		mov		@r0,a
		inc		dptr
		inc		r0
		djnz	r1,do_flash_at_sram_loop
		ljmp	0x8030
    #pragma endasm
}

/*******************************************************************************
* Public Function
*******************************************************************************/
/**
* @brief	Safety code operation of flash memory initialization.
* @param    None
* @return	None
*/
void Init_Flash()
{
	//Init User_ID
	User_ID = 0;

	// Set Flash Sector Address as Dummy address
	FEARH = (FLASH_ADDR_DUMMY >> 16) & 0x0f;	//Flash Sector Address High Register
	FEARM = (FLASH_ADDR_DUMMY >> 8) & 0xff;	//Flash Sector Address Middle Register
	FEARL = (FLASH_ADDR_DUMMY >> 0) & 0xff;	//Flash Sector Address Low Register
}

/**
* @brief	Update Flash buffer for writing flash memory.
* @param    buf 	This parameter contains the buffer of writing Flash memory.
* @param    buf_size 	This parameter contains the size of data.
* @return	None
*/
void Update_Flash_Buf(unsigned char *buf, unsigned int buf_size)
{
	//Flash Buffer Update
	User_ID = FLASH_USER_ID;
	PageBufData = buf;
	PageBufDataSize = buf_size;
}

/**
* @brief	Update flash memory for writing flash memory.
* @param    key 	This parameter contains the address of Flash memory.
*
*					- safety code for operation of flash memory
* @return	None
*/
void Update_Flash(unsigned int key)
{
	unsigned char i = 0;
	unsigned int s = 0;
	//unsigned long addr;
	unsigned char ret = 0;

	if(key == FLASH_KEY)
	{
		//if(User_ID == FLASH_USER_ID)
		//{		
			//addr = FLASH_ADDR_USER;
			s = PageBufDataSize;
			//while(PageBufDataSize > 0)
			//{
				//if(PageBufDataSize < PAGE_BUFFER_SIZE)
				//{
					//s = PageBufDataSize;
					//PageBufDataSize = 0;
				//}
				//else
				//{
					//s = PAGE_BUFFER_SIZE;
					//PageBufDataSize -= PAGE_BUFFER_SIZE;
				//}
				
				ret = Erase_Flash(FLASH_KEY, FLASH_ADDR_USER + (PAGE_BUFFER_SIZE*i));
				//if(ret == 1 || ret == 2)
				//{
					//error;
					//while(1);
				//}
				ret = Write_Flash(FLASH_KEY, FLASH_ADDR_USER + (PAGE_BUFFER_SIZE*i), 
							s, PageBufData + (PAGE_BUFFER_SIZE*i));
				//if(ret == 1 || ret == 4)
				//{
					//error;
					//while(1);

				//}
				//i++;
			//}
		//}
		//User_ID = 0;
	}
}



/**
* @brief	Erase operation of flash memory.
* @param    key 	This parameter contains the address of Flash memory.
*
*					- safety code for operation of flash memory
* @param    sector_address 	This parameter contains the address of Flash memory.
*
*					- address range : 8kbytes(0x0 ~ 0x1FFF)
* @return	status(0 : OK, 1/2 : Failed)
*/
#if 0
unsigned char Erase_Flash(unsigned int key, unsigned long sector_address)
{
	unsigned char code *rom_addr = 0x0000;
	unsigned char i = 0;
	unsigned char tmp = 0;
	unsigned char tmp2 = 0;
	unsigned char ret = 0;

	//Disable EA
	tmp = IE;
	IE &= (~0x80);

	
	//LVR enable, safety code
	tmp2 = LVIR;
	LVIR &= ~(1<<7); //LVR enable  

	if(key == FLASH_KEY)
	{
		key ^= FLASH_XOR;

		if(sector_address < (unsigned long)FLASH_SIZE)
		{
			
			// Flash Flash Control Register CLR change before initialization.
			FECR	= 0;
			FETCR	= 0;
			FEMR	= 0; 
			// Enable program mode
			Flash_Enter();
			
			FETCR = WAITTIME;				// 0x9D = 157 : (157+1) * 2 (31.25ns * 256) = 2.528ms
			FECR = 0x02; // Reset page buffer.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			
			g_FEMR = 0x91;   // Set Erase mode.
			g_FECR =	0x0B; // Start Erase.
	
			// Set Flash Sector Address as used area
			FEARH = (sector_address >> 16) & 0xFF;
			FEARM = (sector_address >> 8) & 0xFF;
			FEARL = (sector_address) & 0xFF;
			

			//Check Power stable by LVR Enable
			//if LVR disable, return error 2
			if((LVIR & 0x80) == 0x00)
			{
				// Check User_ID
				if(User_ID == FLASH_USER_ID)
				{
					// Check valid of Flash Sector Address range
					if( (unsigned long)FLASH_ADDR_MIN <= sector_address && sector_address <= (unsigned long)FLASH_ADDR_MAX)
					{
						if((key ^ FLASH_XOR) == FLASH_KEY)
						{
							// Flase Erase at ram
								do_flash_at_sram();
						}
						
						Flash_Exit();			            // Set flash to normal mode.
						//verify data
						for(i = 0; i < PAGE_BUFFER_SIZE; i++)
						{
							if(rom_addr[sector_address + i] != 0x00)
							{
								ret = 2;
							}
						}
					}
				}
				
			}
			else
			{
				ret = 1;//LVI error
				
				Flash_Exit();
			}
			// Set Flash Sector Address as Dummy address
			FEARH = (FLASH_ADDR_DUMMY >> 16) & 0x0f;	//Flash Sector Address High Register
			FEARM = (FLASH_ADDR_DUMMY >> 8) & 0xff;		//Flash Sector Address Middle Register
			FEARL = (FLASH_ADDR_DUMMY >> 0) & 0xff;		//Flash Sector Address Low Register
		}
	}
	LVIR = tmp2;
	IE = tmp;
	return ret;
}
#else
unsigned char Erase_Flash(unsigned int key, unsigned long sector_address)
{
	unsigned char code *rom_addr = 0x0000;
	//unsigned char i;
	unsigned char tmp = 0;
	unsigned char tmp2 = 0;
	unsigned char ret = 0;

	//Disable EA
	tmp = IE;
	IE &= (~0x80);

	
	//LVR enable, safety code
	tmp2 = LVIR;
	LVIR &= ~(1<<7); //LVR enable  

	if(key == FLASH_KEY)
	{
		key ^= FLASH_XOR;

		if(sector_address < (unsigned long)FLASH_SIZE)
		{
			
			// Flash Flash Control Register CLR change before initialization.
			FECR	= 0;
			FETCR	= 0;
			FEMR	= 0; 
			// Enable program mode
			Flash_Enter();
			
			FETCR = WAITTIME;				// 0x9D = 157 : (157+1) * 2 (31.25ns * 256) = 2.528ms
			FECR = 0x02; // Reset page buffer.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			
			g_FEMR = 0x91;   // Set Erase mode.
			g_FECR =	0x0B; // Start Erase.
	
			// Set Flash Sector Address as used area
			FEARH = (sector_address >> 16) & 0xFF;
			FEARM = (sector_address >> 8) & 0xFF;
			FEARL = (sector_address) & 0xFF;
			

			//Check Power stable by LVR Enable
			//if LVR disable, return error 2
			if((LVIR & 0x80) == 0x00)
			{
				// Check User_ID
				//if(User_ID == FLASH_USER_ID)
				//{
					// Check valid of Flash Sector Address range
					//if( (unsigned long)FLASH_ADDR_MIN <= sector_address && sector_address <= (unsigned long)FLASH_ADDR_MAX)
					//{
						//if((key ^ FLASH_XOR) == FLASH_KEY)
						//{
							// Flase Erase at ram
								do_flash_at_sram();
						//}
						
						Flash_Exit();			            // Set flash to normal mode.
						//verify data
						//for(i = 0; i < PAGE_BUFFER_SIZE; i++)
						//{
							//if(rom_addr[sector_address + i] != 0x00)
							//{
								//ret = 2;
							//}
						//}
					//}
				//}
				
			}
			else
			{
				ret = 1;//LVI error
				
				Flash_Exit();
			}
			// Set Flash Sector Address as Dummy address
			FEARH = (FLASH_ADDR_DUMMY >> 16) & 0x0f;	//Flash Sector Address High Register
			FEARM = (FLASH_ADDR_DUMMY >> 8) & 0xff;		//Flash Sector Address Middle Register
			FEARL = (FLASH_ADDR_DUMMY >> 0) & 0xff;		//Flash Sector Address Low Register
		}
	}
	LVIR = tmp2;
	IE = tmp;
	return ret;
}	
#endif
/**
* @brief	Wrtie operation of flash memory.
* @param    key 	This parameter contains the address of Flash memory.
*
*					- safety code for operation of flash memory
* @param    sector_address 	This parameter contains the address of Flash memory.
*
*					- address range : 8kbytes(0x0 ~ 0x1FFF)
* @param    buf_size 	This parameter contains the size of writing Flash data.
* @param    buf 	This parameter contains the buffer of writing Flash data
* @return	status(0 : OK, 1/4 : Failed)
*/
unsigned char Write_Flash(unsigned int key, unsigned long sector_address, unsigned int buf_size, unsigned char *buf)
{
	//unsigned char *pdt;
	unsigned char code *rom_addr=0x0000;
	unsigned char i = 0;
	unsigned char tmp = 0;
	unsigned char tmp2 = 0;
	unsigned char ret = 0;

	//Disable EA
	tmp = IE;
	IE &= (~0x80);
	
	//LVR enable, safety code
	tmp2 = LVIR;
	LVIR &= ~(1<<7); //LVR enable 
	
	if(key == FLASH_KEY)
	{
		key ^= FLASH_XOR;

		if(sector_address < (unsigned long)FLASH_SIZE)
		{
			// Enable program mode
			Flash_Enter();
			
			FETCR = WAITTIME;				// 0x9D = 157 : (157+1) * 2 (31.25ns * 256) = 2.528ms

			FEMR = 0x81;	// Select flash and enable control.
			FECR = 0x02 ; // (Reset page buffer)
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			_nop_( );	//Dummy instruction, This instruction must be needed.
			
			// Set Flash Sector Address as used area
			FEARH = (sector_address >> 16) & 0xFF;
			FEARM = (sector_address >> 8) & 0xFF;
			FEARL = (sector_address) & 0xFF;

			write_page_buffer(buf);
			buf_size = buf_size;//not used A94B114

			g_FEMR = 0xA1;    // Set write mode.
			g_FECR = 0x0B;		// Start program.
			
			//Check Power stable by LVR Enable
			//if LVR disable, return error 2
			if((LVIR & 0x80) == 0x00)
			{
				// Check User_ID1/2
				//if(User_ID == FLASH_USER_ID)
				//{
					// Check valid of Flash Sector Address range
					//if( (unsigned long)FLASH_ADDR_MIN <= sector_address && sector_address <= (unsigned long)FLASH_ADDR_MAX)
					//{
						//if((key ^ FLASH_XOR) == FLASH_KEY)
						//{
								// Flase Write at ram
								do_flash_at_sram();
						//}

						Flash_Exit();			            // Set flash to normal mode.
						//verify data
						//pdt = buf;
						//for(i = 0; i < PAGE_BUFFER_SIZE; i++)
						//{
							//if(rom_addr[sector_address + i] != *pdt)
							//{
								//ret = 4;
							//}
							
							//pdt++;
						//}
					//}
				//}

			}
			else
			{
				ret = 1;//LVI error
				
				Flash_Exit();
			}
			// Set Flash Sector Address as Dummy address
			FEARH = (FLASH_ADDR_DUMMY >> 16) & 0x0f;	//Flash Sector Address High Register
			FEARM = (FLASH_ADDR_DUMMY >> 8) & 0xff;		//Flash Sector Address Middle Register
			FEARL = (FLASH_ADDR_DUMMY >> 0) & 0xff;		//Flash Sector Address Low Register
		}
	}
	LVIR = tmp2;
	IE = tmp;
	return ret;
}


/**
* @brief	Read operation of flash memory.
* @param    sector_address 	This parameter contains the address of Flash memory.
*
*					- address range : 8kbytes(0x0 ~ 0x1FFF)
* @param    size 	This parameter contains the size of reading Flash data.
* @param    buf 	This parameter contains the data to read flash memory.
* @return	NONE
*/
void Read_Flash(unsigned long sector_address, unsigned int buf_size, unsigned char *buf)
{
	unsigned char code *rom_addr=0x0000;
	unsigned int i = 0;

	for(i = 0; i < buf_size; i++)
	{
		buf[i] = rom_addr[sector_address + i];
	}
}

/* --------------------------------- End Of File ------------------------------ */