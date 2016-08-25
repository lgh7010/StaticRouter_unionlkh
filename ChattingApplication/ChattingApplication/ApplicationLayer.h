#pragma once
#include "LayerStructure.h"
#include "ChattingApplicationDlg.h"

class ApplicationLayer : public LayerStructure
{
public:
	ApplicationLayer(char* pName);
	~ApplicationLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int appdataSize);
	void			Refresh();

	void			SetDstAddress(unsigned int address);
	void			SetSrcAddress(unsigned int address);
	void			SetSize(int size);
	UINT			GetDstAddress();
	UINT			GetSrcAddress();
	UINT			GetSize();

private:
	void			ResetHeader();
	typedef struct APPDATA {
		UINT			_srcAddress;			//4바이트
		UINT			_dstAddress;			//4바이트
		int				_dataSize;				//4바이트
		unsigned char	_data[APP_DATA_SIZE];
	};
	APPDATA				_appdata;
};

