#pragma once
#include "LayerStructure.h"

class TransportLayer : public LayerStructure
{
public:
	TransportLayer(char* pName);
	~TransportLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int applicationDataSize);
	void			Refresh();

	typedef struct SEGMENT {
		unsigned short	SG_srcPort;
		unsigned short	SG_dstPort;
		unsigned char	SG_sqcNum[4];
		unsigned char	SG_ackNum[4];
		unsigned short	SG_HLRFLG;//HeaderLength, Reserved, Flags
		unsigned short	SG_winSize;
		unsigned short	SG_checkSum;
		unsigned short	SG_urgentPoint;
		unsigned char	SG_optionAndPadding[TP_SEGMENT_OP_SIZE];//일단 4바이트라고 가정한다.
		unsigned char	SG_data[TP_SEGMENT_DATA_SIZE];
	};

private:
	void		ResetHeader();
	SEGMENT		_segment;
};

