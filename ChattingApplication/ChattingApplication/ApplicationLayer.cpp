#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName) {
	this->ResetHeader();
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//���� ��ũ�� �Ǳ� ���� UpperLayer�� ã���� �ϹǷ� �ȵ� --> Refresh�Լ� ���� ����.
}
ApplicationLayer::~ApplicationLayer() {}

void ApplicationLayer::Refresh() {
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//���⼭ ������ ����µ� �ϴ�. �̷������� ��ȯ�ؼ� ������ ���°� ����.
}

BOOL ApplicationLayer::Receive(unsigned char * ppayload) {
	AfxMessageBox("���ø����̼� Receive ȣ���");
	APPDATA* appdata = (APPDATA*)ppayload;

	//��ȭ���ڿ��� '������'�ּ������� ����ֱ����� �ʿ��� �۾�
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageDstAddress(appdata->_dstAddress);
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageSrcAddress(appdata->_srcAddress);
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageSize(appdata->_dataSize);

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char* ppayload, int appdataSize) {
	//AfxMessageBox("���ø����̼� Send ȣ���");
	memcpy(this->_appdata._data, ppayload, appdataSize);

	BOOL isDone;
	isDone = this->_pUnderLayer->Send((unsigned char*)&this->_appdata, appdataSize + APP_HEADER_SIZE);
	return isDone;
}




void ApplicationLayer::SetDstAddress(unsigned char* address) {
	memcpy(this->_appdata._dstAddress, address, 4);
}
void ApplicationLayer::SetSrcAddress(unsigned char* address) {
	memcpy(this->_appdata._srcAddress, address, 4);
}
void ApplicationLayer::SetSize(int size) {
	this->_appdata._dataSize = size;
}
unsigned char* ApplicationLayer::GetDstAddress() {
	return this->_appdata._dstAddress;
}
unsigned char* ApplicationLayer::GetSrcAddress() {
	return this->_appdata._srcAddress;
}
UINT ApplicationLayer::GetSize() {
	return this->_appdata._dataSize;
}


void ApplicationLayer::ResetHeader() {
	memset(this->_appdata._srcAddress, 0, 4);
	memset(this->_appdata._dstAddress, 0, 4);
	this->_appdata._dataSize = 0;
	memset(this->_appdata._data, 0, APP_DATA_SIZE);
}
