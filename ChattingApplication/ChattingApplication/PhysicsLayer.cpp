#include "stdafx.h"
#include "PhysicsLayer.h"


PhysicsLayer::PhysicsLayer(char *pName, LPADAPTER *pAdapterObject, int iNumAdapter) : LayerStructure(pName) {
	_adapterObject = NULL;
	_iNumAdapter = iNumAdapter;
	_threadSwitch = TRUE;
	SetAdapterList(NULL);
	
	//�׽�Ʈ��. �������. �� �̰� �ʿ��ϳĸ�, ���� Hypatia���� �� ����̽��� 3���ִµ� ���� 2��°(���)����̽��� ���� ��ī���
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
	if (pcap_sendpacket(_adapterObject, ppayload, nlength)) {//�� ���̺귯�� �Լ��� ���ۿ� �����ϸ� 0��, �����ϸ� -1�� ��ȯ�Ѵٴ��� �׷��� �ϴ�.
		AfxMessageBox("��Ŷ ���� ����");
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
			temp.Format("�ƹ��� ��Ŷ�� �������� �ʾ�!! [���� ��ī�� : %s] ��ī�� �޾ƿ��°� �̻��� ��ī�带 �޾ƿ°� ����. Ȯ���غ�", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
			AfxMessageBox(temp);*/
		} else if (result == 1) {
			/*CString temp;
			temp.Format("� ��Ŷ�� �޾Ҿ�!! [���� ��ī�� : %s]", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
			AfxMessageBox(temp);*/
			pPhysicsLayer->Receive((u_char *)pkt_data);
		} else if (result < 0) {
			/*CString temp;
			temp.Format("�ƹ��� ��Ŷ�� �������� �ʾ�!! [���� ��ī�� : %s] ��ī�� �޾ƿ��°� �̻��� ��ī�带 �޾ƿ°� ����. Ȯ���غ�", pPhysicsLayer->_pAdapterList[pPhysicsLayer->_iNumAdapter]->name);
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
