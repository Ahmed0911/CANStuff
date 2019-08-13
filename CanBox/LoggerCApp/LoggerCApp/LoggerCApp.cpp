// LoggerCApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <intrin.h>

#define SERVER_PORT 12345

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

int main()
{
    std::cout << "Logger Test App!\n";

	// Init winsock - WINDOWS ONLY
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return -1;
	}

	// create socket
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	// set address
	struct sockaddr_in myaddr;
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); // choose first interface (hopefully eth0)
	myaddr.sin_port = htons(SERVER_PORT); // Server port is fixed

	if (bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
	{
		std::cout << "bind failed" << std::endl;
		return -1;
	}

	// Process data
	int rcvCounter = 0;
	while (1)
	{
		char buffer_to_receive[2048];
		int recvlen = recv(sock, buffer_to_receive, 2048, 0);

		if (recvlen != SOCKET_ERROR)
		{
			// Parse Data
			SStats* str = (SStats*)buffer_to_receive;
	
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

			// report
			rcvCounter++;
			if (rcvCounter >= 1000)
			{
				rcvCounter = 0;
				std::cout << "Timestamp: " << str->Timestamp;
				std::cout << "  CycleCounter: " << str->CycleCounter;
				std::cout << "  CPU Usage: " << str->CPUUsage << " %";
				std::cout << "  EthFramesSent: " << str->EthFramesSent;				
				std::cout << "  CAN[1...4]: " << str->CanRcvCounter[0];
				std::cout << "/" << str->CanRcvCounter[1];
				std::cout << "/" << str->CanRcvCounter[2];
				std::cout << "/" << str->CanRcvCounter[3];
				std::cout << "  Bps[1...4]: " << str->CanDataRate[0];
				std::cout << "/" << str->CanDataRate[1];
				std::cout << "/" << str->CanDataRate[2];
				std::cout << "/" << str->CanDataRate[3];
				std::cout << std::endl;

				// dump
				SCANMsg* packets = (SCANMsg*)(buffer_to_receive + sizeof(SStats));				
				for (int i=0; i!= str->NumberOfCANMsgsInPacket; i++)
				{
					// dump packets
					packets[i].CANnr = _byteswap_ulong(packets[i].CANnr);
					packets[i].ID = _byteswap_ulong(packets[i].ID);

					std::cout << "CANnr: " << packets[i].CANnr;
					std::cout << "  ID: " << packets[i].ID;
					for(int c=0; c !=8; c++) std::cout << "," << (int)packets[i].Data[c];
					std::cout << std::endl;
				}
			}
		}
		else
		{
			std::cout << "recv error failed" << std::endl;
			return -1;
		}		
	}
}
