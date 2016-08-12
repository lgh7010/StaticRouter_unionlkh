#pragma once
#include "LayerStructure.h"

class NetworkLayer : public LayerStructure
{
public:
	NetworkLayer();
	~NetworkLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int segmentLength);
	//Ʈ������Ʈ���� �ڷᱸ�� �̸��� '���׸�Ʈ'�̴�. ����, Ʈ������Ʈ������ ��Ʈ��ũ ������ �������� �ڷᱸ���� ���̸� segmentLength��� ����.

	void			SetDstAddress(unsigned char* pAddress);
	void			SetSrcAddress(unsigned char* pAddress);
	unsigned char*	GetDstAddress();
	unsigned char*	GetSrcAddress();

	typedef struct PACKET {
		unsigned char IP_VaL;		//Version and Length			4 bit + 4 bit
		unsigned char IP_tos;		//type of service				8 bit				//IP_TOS�� �̹� MFC���� ���Ǵ� ���� �ҹ��ڷ� �Ѵ�.
		unsigned short IP_length;	//total length					16 bit
		unsigned short IP_id;		//identification				16 bit
		unsigned short IP_FAFO;		//Flags And Fragment Offset		3 bit + 13 bit
		unsigned char IP_ttl;		//time to live					8 bit				//IP_TTL�� �̹� MFC���� ���Ǵ� ���� �ҹ��ڷ� �Ѵ�.
		unsigned char IP_protocol;	//protocol						8 bit
		unsigned short IP_HC;		//Header Checksum				16 bit
		unsigned char IP_src[4];	//SourceAddress					32 bit
		unsigned char IP_dst[4];	//DestinationAddress			32 bit
		//unsigned char IP_option	//option(if any)				unknown
		unsigned char IP_data[IP_PACKET_DATA_SIZE];//data			1480 Byte
	};
};

