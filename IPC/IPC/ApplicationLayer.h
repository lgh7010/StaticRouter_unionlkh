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
	UINT			GetDstAddress();
	UINT			GetSrcAddress();

private:
	void			ResetHeader();
	typedef struct APPDATA {
		UINT			_srcAddress;//2����Ʈ
		UINT			_dstAddress;//2����Ʈ
		unsigned char	_data[APP_DATA_SIZE];//�ϴ��� �׳� 300����Ʈ��
	};
	APPDATA				_appdata;

	//CIPCDlg*			_pIPCDlg;//�� ���� ���� ��ü�� ���� ������ ����Ű�� �ִ�. MFC���𰡿� �浹�ϴµ� �ϴ�.
};

