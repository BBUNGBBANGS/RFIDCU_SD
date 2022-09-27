#ifndef __FLASHMEM_H__
#define __FLASHMEM_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/

/* Exported constants --------------------------------------------------------*/

#define GPIO_CS                  GPIOA
#define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOA
#define GPIO_Pin_CS              SPI_nCS 



/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)

/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)

/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS
*                    
******************************************************************************/


/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/


/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/

void SPI_FLASH_Init(void);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void SPI_FLASH_idRead(uint8_t* pBuffer);
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /*__FLASHMEM_H */
