#pragma once
#include "LayerStructure.h"
#include "IPCDlg.h"

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
	/*
		특급 주의사항. 여기서 가변적인 길이를 가지는 data는 무조건 맨 뒤에 선언해야 한다. 가령, 
		UINT			_srcAddress;
		UINT			_dstAddress;
		unsigned char	_data[APP_DATA_SIZE];
		int				_dataSize;이런식으로 하면, dataSize에 오류가 난다.
	*/

//	CIPCDlg*			_pIPCDlg;//이 변수 선언 자체가 무언가 문제를 일으키고 있다. MFC무언가와 충돌하는듯 하다.
};

