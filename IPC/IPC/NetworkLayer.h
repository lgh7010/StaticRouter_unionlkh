#pragma once
#include "LayerStructure.h"

class NetworkLayer : public LayerStructure
{
public:
	NetworkLayer();
	~NetworkLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int segmentLength);
	//트랜스포트층의 자료구조 이름은 '세그먼트'이다. 따라서, 트랜스포트층에서 네트워크 층으로 내려오는 자료구조의 길이를 segmentLength라고 하자.

	void			SetDstAddress(unsigned char* pAddress);
	void			SetSrcAddress(unsigned char* pAddress);
	unsigned char*	GetDstAddress();
	unsigned char*	GetSrcAddress();

	typedef struct PACKET {
		unsigned char IP_VaL;		//Version and Length			4 bit + 4 bit
		unsigned char IP_tos;		//type of service				8 bit				//IP_TOS는 이미 MFC에서 사용되는 용어여서 소문자로 한다.
		unsigned short IP_length;	//total length					16 bit
		unsigned short IP_id;		//identification				16 bit
		unsigned short IP_FAFO;		//Flags And Fragment Offset		3 bit + 13 bit
		unsigned char IP_ttl;		//time to live					8 bit				//IP_TTL은 이미 MFC에서 사용되는 용어여서 소문자로 한다.
		unsigned char IP_protocol;	//protocol						8 bit
		unsigned short IP_HC;		//Header Checksum				16 bit
		unsigned char IP_src[4];	//SourceAddress					32 bit
		unsigned char IP_dst[4];	//DestinationAddress			32 bit
		//unsigned char IP_option	//option(if any)				unknown
		unsigned char IP_data[IP_PACKET_DATA_SIZE];//data			1480 Byte
	};
};

