#include "stdafx.h"
#include "DatalinkLayer.h"


DatalinkLayer::DatalinkLayer(char* pName) : LayerStructure(pName){
	this->ResetFrame();
}
DatalinkLayer::~DatalinkLayer(){}

/*							<Receive ����>
1. �������̾�(Physics)���� '������'�� �ö����, �� �����ӿ��� '�̴��� ����κ�'�� �����Ѵ�
2. ������(data�κ�)�� �������̾�(network)���� �÷��ش�. �̶� �÷��ִ°��� �翬�� '��Ŷ'�̴�(�̴��� �������� '������'�� ����ִ°� '��Ŷ'�̹Ƿ�)

�׷��� ���⼭�� �׳�, �ö�� �������� pFrame�̶�� ������ �ϴ� �����ϰ� �ű⼭ EF_data�κи��� �������̾��� Receive�� �÷������ִ� ������ ������.
������ ������ ����� ���Ե� ������ �ϴ� ���� �ƹ��͵� ���°��̴�.

���, ���� ��ǻ�Ͱ� ��ſ����� �ٷ� ���⼭ MAC�ּҸ� ���ϴ� �۾��� �̷����� �Ѵ�.
*/
BOOL DatalinkLayer::Receive(unsigned char * ppayload) {
	AfxMessageBox("�����͸�ũ Receive ȣ���");
	ETHERNET_FRAME* pFrame = (ETHERNET_FRAME*)ppayload;

	BOOL isDone;
	isDone = this->GetUpperLayer()->Receive((unsigned char*)pFrame->EF_data);
	return isDone;
}


/*							<Send ����>
1. ���� ���̾�(Network)���� '��Ŷ'�� ��������, �� ��Ŷ(ppayload)�� EthernetFrame�� data�� ����ִ´�.
2. �̷��� ������� '������'�� �������̾�(Physics)�� �����Ѵ�. �����ϴ� ����� �������� ����� �޸��� �����ּҿ�, �������� ũ�⸦ �˷��ִ°��̴�.

������, ��� ���� this->ethernetFrame�� data�κ��� ������ ��� �κп��� �׳� �� 0�� ������� ���̴�.(EthernetLayer �����ڰ� ResetHeader()�� ȣ���ߴ�)
������ ��ǻ�Ͱ� ����� �ƴϱ� ������ IP�ּҰ� �ʿ����� �ʱ� �����̴�.
*/
BOOL DatalinkLayer::Send(unsigned char * ppayload, int packetLength) {
	AfxMessageBox("�����͸�ũ Send ȣ���");
	memcpy(this->_frame.EF_data, ppayload, packetLength);

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_frame, packetLength + ETHERNET_FRAME_HEADER_SIZE);
	return isDone;
}



void DatalinkLayer::SetDstAddress(unsigned char * pAddress) {
	//this->_ethernetFrame.EF_dstaddr = pAddress; ==>�̷��� �ϴ°� �ƴϰ�, �Ʒ��Ͱ��� �ؾ� �Ѵ�.
	memcpy(this->_frame.EF_dstaddr, pAddress, 6);
}
void DatalinkLayer::SetSrcAddress(unsigned char * pAddress) {
	//this->_ethernetFrame.EF_srcaddr = pAddress;
	memcpy(this->_frame.EF_srcaddr, pAddress, 6);
}
unsigned char* DatalinkLayer::GetDstAddress() {
	return this->_frame.EF_dstaddr;
}
unsigned char* DatalinkLayer::GetSrcAddress() {
	return this->_frame.EF_srcaddr;
}



void DatalinkLayer::ResetFrame() {
	memset(this->_frame.EF_dstaddr, 0, 6);
	memset(this->_frame.EF_srcaddr, 0, 6);
	this->_frame.EF_type = 0;
	memset(this->_frame.EF_data, 0, ETHERNET_FRAME_DATA_SIZE);
}