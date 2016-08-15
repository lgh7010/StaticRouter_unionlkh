#pragma once
#include "LayerStructure.h"

class ApplicationLayer : public LayerStructure
{
public:
	ApplicationLayer();
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
		UINT			_srcAddress;
		UINT			_dstAddress;
		unsigned char	_data[APP_DATA_SIZE];//일단은 그냥 400바이트다
	};
	APPDATA				_appdata;


};

