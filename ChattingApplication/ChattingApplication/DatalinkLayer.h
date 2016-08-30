#pragma once
#include "LayerStructure.h"

//���� MAC�ּҸ� �޾ƿ��� ���� �߰���
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

class DatalinkLayer : public LayerStructure
{
public:
	DatalinkLayer(char* pName);
	~DatalinkLayer();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int packetLength);//�̴��� ���̾�� �����ͼ�, �̴��� ���̾ �ڽ��� �������� Data�� �־�� �ϴ°� packet�̴�. �׷��� packetSize��
																	//������, ��Ʈ��ũ���� �ڷᱸ���� �θ��� �̸��� '��Ŷ'�� �ƴ϶� '�����ͱ׷�'�̴�. ��Ŷ�� IP���������� ����ϴ� �ڷᱸ���� ���Ѵ�. �׷��� ���ó� ��Ʈ��ũ���� ���������� ���� IP�̹Ƿ�, �׳� ��Ʈ��ũ���� �ڷᱸ���� ��Ŷ�̶� �θ���.
	void			Refresh();

	void			SetDstAddress(unsigned char* pAddress);
	void			SetSrcAddress(unsigned char* pAddress);
	unsigned char*	GetDstAddress();
	unsigned char*	GetSrcAddress();

	typedef struct ETHERNET_FRAME {//�����͸�ũ���� ���������� '�̴���'�� ����ϱ�� �ϰ�, �̰��� �̴��ݿ��� ���Ǵ� �������� �ڷᱸ��. EF�� "Ethernet Frame"�� ����
		unsigned char	EF_dstaddr[6];						//size : 6 Byte
		unsigned char	EF_srcaddr[6];						//size : 6 Byte
		unsigned short	EF_type;							//size : 2 Byte
		unsigned char	EF_data[ETHERNET_FRAME_DATA_SIZE];	//size : 1500 Byte		==> total : 1514 Byte
	};
	/*
	�̺��� ������ ������ �̴��� ��������, Ư�� ��Ȳ���� ���Ǵ� �������ε� �ϴ�. �ϴ� �� ������ �����ε� ��ǻ�� 5�븦 ������ ������������
	����ƽ �����/�������� �� ����Ǿ��� ������, �⺻������ �̴��� �������� ������ �̰��̶�� ������ �� �ִ�.
	*/

private:
	void			ResetFrame();
	ETHERNET_FRAME	_frame;//���� '�̴���'�̿��� �ٸ� ���������� ����ϱ�� �Ѵٸ�, �ش� ���������� �ڷᱸ���� ����ü�� ������ ��, ETHERNET_FRAME�� �װ����� �ٲ��ָ� ��.
	CString			getLocalMACAddress();
	void			setSrcMACAddress();
};

