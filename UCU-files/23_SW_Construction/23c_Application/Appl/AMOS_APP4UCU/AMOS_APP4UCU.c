
/** ====================================================================================================================
 *  INCLUDE FILES
 *  ==================================================================================================================*/
#include "AMOS_APP4UCU.h"

#include "fifo_spi.h"

/** ====================================================================================================================
 *  DEFINES AND MACROS
 *  ==================================================================================================================*/

/** ====================================================================================================================
 *  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 *  ==================================================================================================================*/

/** ====================================================================================================================
 *  LOCAL CONSTANTS
 *  ==================================================================================================================*/

/** ====================================================================================================================
 *  LOCAL VARIABLES
 *  ==================================================================================================================*/

const uint8_t CRC7_POLY = 0x91;
/** ====================================================================================================================
 *  GLOBAL CONSTANTS
 *  ==================================================================================================================*/

/** ====================================================================================================================
 *  GLOBAL VARIABLES
 *  ==================================================================================================================*/

/** ====================================================================================================================
 *  LOCAL FUNCTION PROTOTYPES
 *  ==================================================================================================================*/
uint32_t getCRC(uint32_t message[], uint32_t length);
/** ====================================================================================================================
 *  LOCAL FUNCTIONS
 *  ==================================================================================================================*/

 
uint32_t getCRC(uint32_t message[], uint32_t length)
{
  uint32_t i, j, crc = 0;
 
  for (i = 0; i < length; i++)
  {
    crc ^= message[i];
    for (j = 0; j < 32; j++)
    {
      if (crc & 1)
        crc ^= CRC7_POLY;
      crc >>= 1;
    }
  }
  return crc;
}
/** ====================================================================================================================
 *  GLOBAL FUNCTIONS
 *  ==================================================================================================================*/ 


/** ====================================================================================================================
 *
 *  @brief        : void AMOS_APP4UCU_AMOS_APP4UCU_Init(void);
 *
 *                  This function initializes the AMOS_APP4UCU software component.
 * ===================================================================================================================*/ 
void AMOS_APP4UCU_AMOS_APP4UCU_Init(void)
{
}

/** ====================================================================================================================
 *
 *  @brief        : void AMOS_APP4UCU_AMOS_APP4UCU_Cyclic(void);
 *
 *                  This is the step function of the AMOS_APP4UCU software component. It is called every 10ms.
 * ===================================================================================================================*/ 
void AMOS_APP4UCU_AMOS_APP4UCU_Cyclic(void)
{
    // empty all regs for clear transmitting
    void spi_FiFoClearRx(void);
    void spi_FiFoClearTx(void);

  //bytes to send to APP
   uint32_t numArrToApp[8];
   
   // bytes to receive from APP
   uint32_t numArrFromApp[8];
//CrcChecksum
uint32_t CrcChecksum=0;
//get all the numbers from app
for(char counterRec=0; counterRec<8; counterRec++){
    while(!spi_FiFoGetRx(numArrToApp[counterRec])){}
}
    //get crc
    spi_FiFoGetRx(&CrcChecksum);
    //do crc Check and ack
    if(getCRC(numArrFromApp, 8) != CrcChecksum){
       spi_FiFoAddTx(0x00);
       return;
    }
     spi_FiFoAddTx(0x01);
    
// send data to app

for(char counterSend=0; counterSend<8; counterSend++){
    while(!spi_FiFoAddTx(numArrToApp[counterSend])){}
}
//send CRC
spi_FiFoAddTx(getCRC(numArrToApp,8));

}