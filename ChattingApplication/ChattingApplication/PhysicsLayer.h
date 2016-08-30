#pragma once
#include "LayerStructure.h"

//pcap라이브러리 포함
#include <pcap.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Packet32.h>
#pragma comment(lib, "packet.lib")

class PhysicsLayer : public LayerStructure
{
public:
	PhysicsLayer(char* pName, LPADAPTER *pAdapterObject = NULL, int iNumAdapter = 0);
	~PhysicsLayer();

	BOOL	Receive(unsigned char* ppayload);
	BOOL	Send(unsigned char* ppayload, int frameLength);
	void	Refresh();

protected:
	pcap_t*			_adapterObject;
	int				_iNumAdapter;
	pcap_if_t*		_pAdapterList[10];

public:
	BOOL			_threadSwitch;

	void			PacketStartDriver();

	pcap_if_t* 		GetAdapterObject(int iIndex);
	void			SetAdapterNumber(int iNum);
	void			SetAdapterList(LPADAPTER *plist);

	static UINT		ReadingThread(LPVOID pParam);

	void			setThreadSwitch(BOOL b);
};

