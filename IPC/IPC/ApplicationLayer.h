#pragma once
#include "LayerStructure.h"

class ApplicationLayer : public LayerStructure
{
public:
	ApplicationLayer(char* pName);
	~ApplicationLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int appdataSize);

	void			SetDstAddress(unsigned int address);
	void			SetSrcAddress(unsigned int address);
	UINT			GetDstAddress();
	UINT			GetSrcAddress();

private:
	void			ResetHeader();
	typedef struct APPDATA {
		UINT			_srcAddress;//2바이트
		UINT			_dstAddress;//2바이트
		unsigned char	_data[APP_DATA_SIZE];//일단은 그냥 300바이트다
	};
	APPDATA				_appdata;
};

