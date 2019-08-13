/** @file sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/** @mainpage TMS570LS20216SZWT Micro Controller Driver Documentation
*   @date 07-July-2017
*
*   @section sec1 Introduction
*   This document describes the TMS570 microcontroller peripheral drivers.
*
*   @section sec2 Drivers
*   @subsection sec2sub1 RTI Driver
*   Real Time Interface Module Driver.
*   @subsection sec2sub2 GIO Driver
*   General Purpose Input Output Module Driver.
*   @subsection sec2sub3 SCI Driver
*   Serial Communication Interface Module Driver.
*   @subsection sec2sub4 SPI Driver
*   Serial Peripheral Interface Module Driver.
*   @subsection sec2sub5 CAN Driver
*   Controller Area Network Module Driver.
*   @subsection sec2sub6 ADC Driver
*   Analog to Digital Converter Module Driver.
*   @subsection sec2sub7 LIN Driver
*   Local Interconnect Network Module Driver.
*   @subsection sec2sub8 HET Driver
*   High End Timer Module Driver.
*/

/* USER CODE BEGIN (0) */
/* USER CODE END */


/* Include Files */

#include "sys_common.h"
#include "system.h"

/* USER CODE BEGIN (1) */
#include "sci.h"
#include "can.h"
#include "rti.h"
/* USER CODE END */


/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
int rtiCounter = 0;
int TXCounter = 0;
void canUpdateID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();
    canInit(); /* can1 */
    rtiInit(); /* RTI/Timer driver */

    /* Enable RTI Compare 0 interrupt notification */
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    /* Start RTI Counter Block 0 */
    rtiStartCounter(rtiCOUNTER_BLOCK0);

    _enable_IRQ();

    while(1)
    {
        // read command
        // 1. Find SOP
        uint8_t ch = 0;
        do
        {
            ch = sciReceiveByte (sciREG1);
        } while( ch != 'X');

        // 2. Read packet
        uint8_t packet[14];
        sciReceive(sciREG1, 14, packet);

        // 3. Check end
        if( packet[13] != 'F') continue; // error

        // 4. Execute
        if( packet[0] == 'O') // One Mode
        {
            // wait for TX object to be free
            while( canIsTxMessagePending(canREG1, canMESSAGE_BOX1));

            // Update ID
            uint32_t newID;
            memcpy(&newID, &packet[1], 4);
            newID = __rev(newID); // invert ID
            newID  = (uint32)0x80000000U | (uint32)0x40000000U | (uint32)0x20000000U | (uint32)((uint32)((uint32)newID & (uint32)0x1FFFFFFFU) << (uint32)0U);
            canUpdateID(canREG1, canMESSAGE_BOX1, newID);

            // send
            canTransmit(canREG1, canMESSAGE_BOX1, &packet[5]);
            TXCounter++;

            // ack
            sciSendByte(sciREG1, 'A');
        }
        if( packet[0] == 'B') // Burst Mode
        {
            int i;
            for( i=0; i!=100000; i++ )
            {
                // wait for TX object to be free
                while( canIsTxMessagePending(canREG1, canMESSAGE_BOX1));

                // Update ID
                uint32_t newID;
                memcpy(&newID, &packet[1], 4);
                newID = __rev(newID); // invert ID
                newID  = (uint32)0x80000000U | (uint32)0x40000000U | (uint32)0x20000000U | (uint32)((uint32)((uint32)newID & (uint32)0x1FFFFFFFU) << (uint32)0U);
                canUpdateID(canREG1, canMESSAGE_BOX1, newID);


                // send new packet
                canTransmit(canREG1, canMESSAGE_BOX1, &packet[5]);
                TXCounter++;

                // TODO: increment ID/DATA
            }

            // Send burst packet
            sciSendByte(sciREG1, 'A');
        }
        else continue; // error
    }

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification)
{
    rtiCounter++;
}

/** @fn uint32 canUpdateID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal)
*   @brief Gets received a CAN message
*   @param[in] node Pointer to CAN node:
*              - canREG1: CAN1 node pointer
*              - canREG2: CAN2 node pointer
*              - canREG3: CAN3 node pointer
*              - canREG4: CAN4 node pointer
*   @param[in] messageBox Message box number of CAN node:
*              - canMESSAGE_BOX1: CAN message box 1
*              - canMESSAGE_BOXn: CAN message box n [n: 1-64]
*              - canMESSAGE_BOX64: CAN message box 64
*   @param[in] msgBoxArbitVal (32 bit value):
*               Bit 31 - Not used.
*               Bit 30 - 0 - The 11-bit ("standard") identifier is used for this message object.
*                        1 - The 29-bit ("extended") identifier is used for this message object.
*               Bit 29 - 0 - Direction = Receive
*                        1 - Direction = Transmit
*               Bit 28:0 - Message Identifier.
*   @return

*
*   This function changes the Identifier and other arbitration parameters of a CAN Message Box.
*
*/
/* SourceId : CAN_SourceId_027 */
/* DesignId : CAN_DesignId_021 */
/* Requirements : HL_CONQ_CAN_SR40 */
void canUpdateID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal)
{

    /** - Wait until IF2 is ready for use */
    while ((node->IF1STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

    /** - Configure IF2 for
    *     - Message direction - Read
    *     - Data Read
    *     - Clears NewDat bit in the message object.
    */
    node->IF1CMD = 0xA0U;
    /* Copy passed value into the arbitration register. */
    node->IF1ARB &= 0x80000000U;
    node->IF1ARB |= (msgBoxArbitVal & 0x7FFFFFFFU);

    /** - Update message box number. */
    /*SAFETYMCUSW 93 S MR: 6.1,6.2,10.1,10.2,10.3,10.4 <APPROVED> "LDRA Tool issue" */
    node->IF1NO = (uint8) messageBox;

    /** - Wait until data are copied into IF2 */
    while ((node->IF1STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

}
/* USER CODE END */
