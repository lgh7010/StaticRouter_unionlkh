#include "stdafx.h"
#include "PhysicsLayer.h"


PhysicsLayer::PhysicsLayer(char *pName, LPADAPTER *pAdapterObject, int iNumAdapter) : LayerStructure(pName) {
	_adapterObject = NULL;
	_iNumAdapter = iNumAdapter;
	_threadSwitch = TRUE;
	SetAdapterList(NULL);
	
	//테스트용. 삭제요망. 왜 이게 필요하냐면, 현재 Hypatia에는 랜 디바이스가 3개있는데 그중 2번째(가운데)디바이스가 실제 랜카드다
	_iNumAdapter = 1;
}
PhysicsLayer::~PhysicsLayer() {}
void PhysicsLayer::Refresh() {}

void PhysicsLayer::PacketStartDriver(){
	char errbuf[PCAP_ERRBUF_SIZE];

	if (_iNumAdapter == -1) {
		AfxMessageBox("Not exist NICard");
		return;
	}

	_adapterObject = pcap_open_live(_pAdapterList[_iNumAdapter]->name, 1500, PCAP_OPENFLAG_PROMISCUOUS, 2000, errbuf);
	if (!_adapterObject) {
		AfxMessageBox(errbuf);
		return;
	}
	AfxBeginThread(ReadingThread, this);
}

BOOL PhysicsLayer::Send(unsigned char *ppayload, int nlength) {
	if (pcap_sendpacket(_adapterObject, ppayload, nlength)) {//이 라이브러리 함수는 전송에 성공하면 0을, 실패하면 -1을 반환한다던지 그런듯 하다.
		AfxMessageBox("패킷 전송 실패");
		return FALSE;
	}

	return TRUE;
}

BOOL PhysicsLayer::Receive(unsigned char* ppayload) {
	BOOL bSuccess = FALSE;

	bSuccess = this->_pUpperLayer->Receive(ppayload);
	return bSuccess;
}

UINT PhysicsLayer::ReadingThread(LPVOID pParam){
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int result;

	PhysicsLayer *pPhysicsLayer = (PhysicsLayer *)pParam;

	while (pPhysicsLayer->_threadSwitch) {
		result = pcap_next_ex(pPhysicsLayer->_adapterObject, &header, &pkt_data);

		if (result == 0) {
			/*CString temp;
			temp.Format("아무런 패킷도 오고있지 않아!! [현재 랜카드 : %s] 랜카드 받아오는게 이상한 랜카드를 받아온거 같아. 확인해봐", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
			AfxMessageBox(temp);*/
		} else if (result == 1) {
			/*CString temp;
			temp.Format("어떤 패킷을 받았어!! [현재 랜카드 : %s]", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
			AfxMessageBox(temp);*/
			pPhysicsLayer->Receive((u_char *)pkt_data);
		} else if (result < 0) {
			/*CString temp;
			temp.Format("아무런 패킷도 오고있지 않아!! [현재 랜카드 : %s] 랜카드 받아오는게 이상한 랜카드를 받아온거 같아. 확인해봐", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
			AfxMessageBox(temp);*/
		}
	}
	return 0;
}

void PhysicsLayer::setThreadSwitch(BOOL b) {
	this->_threadSwitch = b;
}




void PhysicsLayer::SetAdapterList(LPADAPTER *plist) {
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int i = 0;

	char errbuf[PCAP_ERRBUF_SIZE];

	for (int j = 0; j<10; j++) {
		_pAdapterList[j] = NULL;
	}

	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		AfxMessageBox("Not exist NICard");
		return;
	}
	if (!alldevs) {
		AfxMessageBox("Not exist NICard");
		return;
	}

	for (d = alldevs; d; d = d->next) {
		_pAdapterList[i++] = d;
	}
}










pcap_if_t* PhysicsLayer::GetAdapterObject(int iIndex) {
	return _pAdapterList[iIndex];
}

void PhysicsLayer::SetAdapterNumber(int iNum) {
	_iNumAdapter = iNum;
}
