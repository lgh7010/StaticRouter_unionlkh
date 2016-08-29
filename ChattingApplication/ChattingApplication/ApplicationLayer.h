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

	void			SetDstAddress(unsigned char* address);
	void			SetSrcAddress(unsigned char* address);
	void			SetSize(int size);
	unsigned char*	GetDstAddress();
	unsigned char*	GetSrcAddress();
	UINT			GetSize();

private:
	void			ResetHeader();
	typedef struct APPDATA {
		unsigned char	_srcAddress[4];			//4����Ʈ
		unsigned char	_dstAddress[4];			//4����Ʈ
		int				_dataSize;				//4����Ʈ
		unsigned char	_data[APP_DATA_SIZE];
	};
	APPDATA				_appdata;
};

