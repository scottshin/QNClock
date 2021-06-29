/**
*   @file       A94B114_flash.h
*   @brief      A94B114 flash control function 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H_
#define __FLASH_H_
/*******************************************************************************
* Included File
*******************************************************************************/
#include "a94b114.h"
#include "intrins.h"
//#include "delay.h"
#include "typedef.h"
/*******************************************************************************
* Public Macro
*******************************************************************************/
//Device Description=====================================//
//	Device : A94B114
//	Flash : 8 Kbyte (1FFFh)
//	PageBuf : 32 byte (20h)
//=======================================================//

//Device Dependent=======================================
#define FLASH_SIZE (0x001FFF)
#define PAGE_BUFFER_SIZE (0x20)

//Modify to use==========================================
//Flash Address for used
#define FLASH_ADDR_USER 0x001FE0

//Flash Address Min/Max value for permitted area
#define FLASH_ADDR_MIN 0x001000
#define FLASH_ADDR_MAX 0x001FFF

//more than 2.0V @ IRC 16MHz
#define FLASH_LVI 0x01	//LVI 2.4V(2.05V~2.35V)

//Flash Dummy Address value, not used area
#define FLASH_ADDR_DUMMY 0x03FF00
//=====================================================

//safety code for operation of flash memory
#define FLASH_KEY 0xAA55
#define FLASH_XOR 0x9C75

#define WAITTIME 0x9D	// 2.5ms

//Flash Mode Control value
#define FLASH_PAGEBUFFER_RESET 0x01

//Flash User ID value, used password
#define FLASH_USER_ID 0xCC33



/*******************************************************************************
* Public Typedef
*******************************************************************************/

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void Init_Flash();
void Update_Flash_Buf(unsigned char *buf, unsigned int size);
void Update_Flash(unsigned int key);
unsigned char Erase_Flash(unsigned int key, unsigned long sector_address);
unsigned char Write_Flash(unsigned int key, unsigned long sector_address, unsigned int size, unsigned char *buf);
void Read_Flash(unsigned long sector_address, unsigned int size, unsigned char *buf);

#endif //_FLASH_HEADER_

/* --------------------------------- End Of File ------------------------------ */