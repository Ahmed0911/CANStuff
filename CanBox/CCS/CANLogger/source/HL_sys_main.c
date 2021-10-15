/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
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


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "HL_gio.h"
#include "HL_emac.h"
#include "HL_hw_reg_access.h"
#include "HL_can.h"
#include "HL_rti.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

//////////////////////////////
// ETH Configuration
//////////////////////////////
uint8_t destMACAddress[6] =   {0x00U, 0x04U, 0x4BU, 0xAFU, 0x76U, 0x25U}; // Local home
//uint8_t destMACAddress[6] =   {0x92U, 0x6bU, 0xc7U, 0x41U, 0x4bU, 0xe8U}; // Office, Xavier-A, br0
//uint8_t destMACAddress[6] =   {0x76U, 0xe7U, 0x01U, 0x0aU, 0xcfU, 0xf2U}; // Scenic-1, Xavier-A, br0
//uint8_t destMACAddress[6] =   {0xeaU, 0x45U, 0xd7U, 0x8eU, 0x91U, 0x4bU}; // Scenic-1, Xavier-B, br0

//uint8_t destinationIPAddr[4] = { 192, 168, 0, 19 }; // CHANGE
uint8_t destinationIPAddr[4] = { 192, 168, 1, 11 }; // CHANGE

//uint8_t sourceIPAddr[4] = { 192, 168, 0, 123 }; // CHANGE
uint8_t sourceIPAddr[4] = { 192, 168, 1, 110 }; // CHANGE

uint16_t destinationPort = 59478;
//////////////////////////////



extern hdkif_t hdkif_data[1];

pbuf_t CreateUDPPacket(uint8_t* frame, uint8_t* dataToSend, uint32_t dataLength );
uint8_t g_Frame[2000]; // global buffer for UDP Packet Transmit (Should be in un-cached memory!!! It is accessed by EMAC DMA)

// counters for bitrateCalc
uint32_t g_canCalcCounter = 0;
uint32_t g_canRcvCounters[4];

// RTI trigger
volatile bool g_IntHit;


// Header Struct
struct SStats
{
    uint64_t Timestamp; // set in RTI
    uint64_t CycleCounter; // set at the end of Main loop
    float CPUUsage; // CPU Usage [%]

    // CAN Stuff
    uint32_t CanRcvCounter[4];
    float CanDataRate[4]; // data rate in kbps

    uint32_t LostPackets;
    uint32_t NumberOfCANMsgsInPacket;

    // Ethernet Stuff
    uint32_t EthFramesSent;
};

#pragma pack(push, 1)
struct AurixCanHeader
{
    uint8_t direction;// 0x14: Xavier to Aurix, 0x28: Aurix to Xavier
    uint8_t source; // 0x01 - Main Aurix
    uint8_t destination; // 0x03 - Xavier-A
    uint8_t busType; // CAN
    uint16_t infoLength; // info size (without 4Byte header): messageLength+12
    uint16_t infoCode;
    uint8_t busID; // CAN1...CAN6
    uint8_t reserved;
    uint16_t sequenceNumber;
    uint32_t messageID; // CAN-ID
    uint16_t messageLength; // 0..8 bytes
    uint8_t data[8]; // 8 data bytes
};
#pragma pack(pop)

volatile struct SStats g_Stats; // Header
uint8_t g_DataToSend[2000]; // Actual data to send (header + messages)
uint32_t g_SizeToSend = 0; // size t send (also use for filling buffer)

void GetCANAndFillBuffer(canBASE_t* canReg, uint32 canDev);

uint32_t g_TimeCounterStartTicks;
float GetAndRestartTimer_us();

/* USER CODE END */

uint8	emacAddress[6U] = 	{0x00U, 0x00U, 0xAAU, 0xBBU, 0xCCU, 0xDDU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    canInit(); /* can1...can4 */
    gioToggleBit(gioPORTB, 7);
    rtiInit(); /* RTI/Timer driver */
    EMACHWInit(emacAddress); /* EMAC Init */

    /* Enable RTI Compare 0 interrupt notification */
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
    /* Start RTI Counter Block 0+1 */
    rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK0);
    rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK1);

    // Enable Interrupts
    _enable_IRQ();

    while(1)
    {
        // 0. Start Profiling
        GetAndRestartTimer_us();

        // 1. Get CAN DATA
        g_Stats.NumberOfCANMsgsInPacket = 0; // reset counter and pointer
        g_SizeToSend = 0; // reset pointer, do not include header in aurix compatibility mode

        GetCANAndFillBuffer(canREG1, 0);
        GetCANAndFillBuffer(canREG2, 1);
        GetCANAndFillBuffer(canREG3, 2);
        GetCANAndFillBuffer(canREG4, 3);

        // 2. Calculate Data Rate
        if( ++g_canCalcCounter >= 1000 )
        {
            int i = 0;
            for(i=0; i!=4; i++)
            {
                //g_Stats.CanDataRate[i] = (float)g_canRcvCounters[i] / g_canCalcCounter; // packets/sec
                g_Stats.CanDataRate[i] = (131 * 1000 * (float)g_canRcvCounters[i] / g_canCalcCounter) / 1024; // kbps
                g_canRcvCounters[i] = 0; // reset counter
            }

            g_canCalcCounter = 0;

            // Blink LEDs
            gioToggleBit(gioPORTB, 6);
            gioToggleBit(gioPORTB, 7);
        }

        // 3. Send Ethernet Packet
        uint32_t len = g_SizeToSend; // total size to send

        if( len > 0 ) // do not send empty UDP packets (no messages)
        {
            pbuf_t pbuf = CreateUDPPacket(g_Frame, g_DataToSend, len);
            EMACTransmit(&hdkif_data[0], &pbuf);
            g_Stats.EthFramesSent++;
        }

        // 4. Calc PCU Usage
        uint32_t cpuTime_us = GetAndRestartTimer_us();
        g_Stats.CPUUsage = 100 * (cpuTime_us / 1000.0f); // 1000 - cycle time in [us]

        // wait new INT
        while(!g_IntHit);
        g_IntHit = false;
        g_Stats.CycleCounter++;
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
    g_IntHit = true;
    g_Stats.Timestamp++;
}

int EthRXINTCounter;
int EthOWNERLocked;
int EthNoEOP;
int TotalSize;
void emacRxNotification(hdkif_t *hdkif)
{
    EthRXINTCounter++;

    volatile emac_rx_bd_t *curr_bd = (&(hdkif->rxchptr))->active_head;

    while((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_SOP) == EMAC_BUF_DESC_SOP)
    {
        // packet transfer in progress, is it done yet?
        if((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_OWNER) == EMAC_BUF_DESC_OWNER)
        {
            // still in progress, go out?
            EthOWNERLocked++;
            break;
        }

        // is this also final packet?
        if((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_EOP) != EMAC_BUF_DESC_EOP)
        {
            EthNoEOP++;
            break;
        }

        // all ok, get size
        uint16_t tot_len = EMACSwizzleData(curr_bd->flags_pktlen) & 0xFFFF;
        TotalSize += tot_len;
        //////////////////////
        // TODO: Parse packet

        //////////////////////

        // go to next
        if(curr_bd->next == NULL ) break; // no more buffers?
        curr_bd = (emac_rx_bd_t *)EMACSwizzleData((uint32)curr_bd->next);
    };
}

////////////////////////////
// Perf Timer Functions
////////////////////////////
float GetAndRestartTimer_us()
{
    uint32_t newTimeTicks = rtiREG1->CNT[rtiCOUNTER_BLOCK1].FRCx;
    int32_t deltaTicks =  newTimeTicks - g_TimeCounterStartTicks;
    g_TimeCounterStartTicks = newTimeTicks;

    float timeus = deltaTicks / 25.0f; // 25MHz clock

    return timeus;
}


////////////////////////////
// Fill CAN
////////////////////////////
void GetCANAndFillBuffer(canBASE_t* canReg, uint32 canDev)
{
    uint32_t i;
    uint8  rx_data[9]; // 9. byte in CAN size (HL_can.c modifed!)
    for(i=0; i!=15; i++)
    {
        uint32_t boxId = canMESSAGE_BOX1 + i;
        if( canIsRxMessageArrived(canReg, boxId))
        {
            // get data
            uint32_t canID = canGetID(canReg, boxId); // get CAN ID (canGetData() clears NewDat flag)
            if( canGetData(canReg, boxId, rx_data) == 1 ) // data ok?
            {
                uint16 dataLength = rx_data[8];

                canID = canID >> 18; // for 11-bit
                g_Stats.CanRcvCounter[canDev]++;
                g_canRcvCounters[canDev]++;

                // fill packet
                struct AurixCanHeader header;
                header.direction = 0x28; // 0x14: Xavier to Aurix, 0x28: Aurix to Xavier
                header.source = 0x01; // 0x01 - main Aurix
                header.destination = 0x03; // 0x03 - Xavier-A
                header.busType = 0x00; // CAN
                header.infoLength = __rev16(12+dataLength);
                header.infoCode = 0;
                header.busID = canDev;
                header.sequenceNumber = 0;
                header.messageID = __rev(canID);
                header.messageLength = __rev16(dataLength);
                memcpy(header.data, rx_data, 8);

                // copy to buffer and advance
                memcpy(&g_DataToSend[g_SizeToSend], &header, 18+dataLength );
                g_SizeToSend+=18+dataLength;
                g_Stats.NumberOfCANMsgsInPacket++;
            }
            else g_Stats.LostPackets++;
        }
    }
}


////////////////////////////
// Minimum dataLength = 18 bytes (64 bytes is minimum ethernet frame size: EthernetHeader(14) + IPHeader(20) + UDPHeader(8) + FCS(4) = 46 + 18 = 64 bytes)
// Maximum dataLength = 1476 bytes (1522 bytes is maximum ethernet frame size: 46 + 1476 = 1522)
////////////////////////////
pbuf_t CreateUDPPacket(uint8_t* frame, uint8_t* dataToSend, uint32_t dataLength )
{
    pbuf_t pbuf;
    pbuf.next = NULL;
    pbuf.payload = frame;
    pbuf.len = dataLength + 28 + 14;
    pbuf.tot_len = dataLength + 28 + 14;

    //////////////////////////////////////
    // ETHERNET/MAC HEADER (14 bytes)
    //////////////////////////////////////
    // Destination MAC address
    int i;
    for(i=0; i < 6; i++)
    {
        //frame[i] = 0xff; // broadcast
        frame[i] = destMACAddress[i];
    }

    // Source MAC address
    for(i=0; i < 6; i++) frame[6+i] = emacAddress[i];

    // Ethernet Type II
    frame[12] = 0x08;
    frame[13] = 0x00;



    //////////////////////////////////////
    // IP HEADER (20 bytes)
    //////////////////////////////////////
    // Version IPv4, header length = 5
    frame[14] = 0x45;
    // No diff services (TODO, check!)
    frame[15] = 0x00;

    // UDP Length (IP header + UDP header + Data Length)
    // IP Header = 20 bytes
    // UDP Header =  8 bytes
    uint16_t totalIPLength = 28 + dataLength;
    frame[16] = totalIPLength / 256;
    frame[17] = totalIPLength % 256;

    // Fragments (not used)
    frame[18] = 0x00; // Identification =  0;
    frame[19] = 0x00;
    frame[20] = 0x00; // Flags -> no fragmentation
    frame[21] = 0x00; // Frag offset = 0

    // TTL
    frame[22] = 5; // 5 hops max

    // Protocol
    frame[23] = 17; // UDP protocol

    // Checksum (will be addded later)
    frame[24] = 0;
    frame[25] = 0;

    // Source IP Address
    for(i=0; i < 4; i++)
    {
        frame[26+i] =  sourceIPAddr[i];
    }

    // Destination IP Address
    for(i=0; i < 4; i++)
    {
        frame[30+i] =  destinationIPAddr[i];
    }

    // Calculate IP Header checksum and add to pacet
    uint32_t sum = 0;
    for (i=0; i < 20; i=i+2)
    {
        uint16_t word16 =((frame[14+i]<<8)&0xFF00)+(frame[14+i+1]&0xFF);
        sum = sum + (uint32_t) word16;
    }
    // take only 16 bits out of the 32 bit sum and add up the carries
    while (sum>>16) sum = (sum & 0xFFFF)+(sum >> 16);
    sum = ~sum; // one's complement the result
    // Add Checksum
    frame[24] = sum / 256;
    frame[25] = sum % 256;



    //////////////////////////////////////
    // UDP HEADER (8 bytes)
    //////////////////////////////////////
    // Source port (not used, set to zero)
    frame[34] = 0;
    frame[35] = 0;

    // Destination port
    frame[36] = destinationPort / 256;
    frame[37] = destinationPort % 256;

    // Length
    uint16_t UDPLength = 8 + dataLength;
    frame[38] = UDPLength / 256;
    frame[39] = UDPLength % 256;

    // UDP Checksum (not used)
    frame[40] = 0;
    frame[41] = 0;


    //////////////////////////////////////
    // DATA
    //////////////////////////////////////
    memcpy(&frame[42], dataToSend, dataLength);


    return pbuf;
}
/* USER CODE END */
