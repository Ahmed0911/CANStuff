// LoggerCApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <memory.h>
#include <byteswap.h>
#include <chrono>

#define SERVER_PORT 12345

#define _byteswap_ulong(x)  bswap_32(x)
#define _byteswap_uint64(x)  bswap_64(x)


struct SStats
{
        uint64_t Timestamp; // set in RTI
        uint64_t CycleCounter; // set at the end of Main loop
        float CPUUsage; // CPU Usage [%]

        // CAN Stuff
        uint32_t CanRcvCounter[4];
        float CanDataRate[4]; // kbps

        uint32_t LostPackets;
        uint32_t NumberOfCANMsgsInPacket;

        // Ethernet Stuff
        uint32_t EthFramesSent;
};

struct SCANMsg
{
        uint32_t CANnr;
        uint32_t ID;
        uint8_t Data[8];
};

float ReverseFloat(const float inFloat)
{
        float retVal;
        char *floatToConvert = (char*)& inFloat;
        char *returnFloat = (char*)& retVal;

        // swap the bytes into a temporary buffer
        returnFloat[0] = floatToConvert[3];
        returnFloat[1] = floatToConvert[2];
        returnFloat[2] = floatToConvert[1];
        returnFloat[3] = floatToConvert[0];

        return retVal;
}

int main(int argc, char* argv[])
{
    std::cout << "CAN Logger App, V1.1!\n";

        std::ofstream outputFile;

        if( argc > 1 )
        {
                std::cout << "Recording to file: " << argv[1] << std::endl;
                outputFile.open("argv[1]", std::ios::binary);

                if(!outputFile.is_open() )
                {
                        std::cout << "File open failed" << std::endl;
                        return -1;
                }
        }

        // create socket
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        std::cout << "Socket: " << s << std::endl;

        // set address
        struct sockaddr_in myaddr;
        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(SERVER_PORT);
        myaddr.sin_addr.s_addr = inet_addr("192.168.2.100"); // choose first interface (hopefully eth0)

        if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
        {
                std::cout << "bind failed" << std::endl;
                return -1;
        }

        std::cout << "Waiting for data..." << std::endl;

        // Process data
        int rcvCounter = 0;
        char Buff[2000];
        while (1)
        {
                int recvlen = recv(s, Buff, 2000, 0);

                if (recvlen != -1)
                {
                        SStats* str = (SStats*)Buff;
                        if( outputFile.is_open() ) // Dump all data to binary file
                        {
                                // add timestamp
                                int64_t timeUS = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                str->Timestamp = _byteswap_uint64(timeUS); // convert to BigE

                                outputFile.write(Buff, recvlen);
                        }
                        else // Dump to screen, TEXT
                        {
                                // bg32 to little
                                str->Timestamp = _byteswap_uint64(str->Timestamp);
                                str->CycleCounter = _byteswap_uint64(str->CycleCounter);
                                str->CPUUsage = ReverseFloat(str->CPUUsage);
                                for (int i = 0; i != 4; i++)
                                {
                                        str->CanRcvCounter[i] = _byteswap_ulong(str->CanRcvCounter[i]);
                                        str->CanDataRate[i] = ReverseFloat(str->CanDataRate[i]);
                                }
                                str->NumberOfCANMsgsInPacket = _byteswap_ulong(str->NumberOfCANMsgsInPacket);
                                str->EthFramesSent = _byteswap_ulong(str->EthFramesSent);

                                // dump packets
                                SCANMsg* packets = (SCANMsg*)(Buff + sizeof(SStats));
                                for (int i=0; i!= str->NumberOfCANMsgsInPacket; i++)
                                {
                                        packets[i].CANnr = _byteswap_ulong(packets[i].CANnr);
                                        packets[i].ID = _byteswap_ulong(packets[i].ID);

                                        int64_t timeUS = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                        std::cout << "TImestampUS: " << timeUS;
                                        //std::cout << "Timestamp: " << str->Timestamp;
                                        std::cout << ",CANnr: " << packets[i].CANnr;
                                        std::cout << ",ID: " << packets[i].ID;
                                        for(int c=0; c !=8; c++) std::cout << "," << (int)packets[i].Data[c];
                                        std::cout << std::endl;
                                }
                        }

#if 1
                        // report statistics
                        rcvCounter++;
                        if (rcvCounter >= 1000)
                        {
                                rcvCounter = 0;
                                std::cout << "###Timestamp: " << str->Timestamp;
                                std::cout << "  CycleCounter: " << str->CycleCounter;
                                std::cout << "  CPU Usage: " << str->CPUUsage << " %";
                                std::cout << "  EthFramesSent: " << str->EthFramesSent;
                                std::cout << "  CAN[1...4]: " << str->CanRcvCounter[0];
                                std::cout << "/" << str->CanRcvCounter[1];
                                std::cout << "/" << str->CanRcvCounter[2];
                                std::cout << "/" << str->CanRcvCounter[3];
                                std::cout << "  kBps[1...4]: " << str->CanDataRate[0];
                                std::cout << "/" << str->CanDataRate[1];
                                std::cout << "/" << str->CanDataRate[2];
                                std::cout << "/" << str->CanDataRate[3];
                                std::cout << std::endl;
                        }
#endif
                }
                else
                {
                        std::cout << "recv error failed" << std::endl;
                        return -1;
                }
        }
        close(s);
}
