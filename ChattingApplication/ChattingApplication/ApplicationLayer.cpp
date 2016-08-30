#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName) {
	this->ResetHeader();
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//아직 링크가 되기 전에 UpperLayer를 찾으려 하므로 안됨 --> Refresh함수 개념 도입.
}
ApplicationLayer::~ApplicationLayer() {}

void ApplicationLayer::Refresh() {
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//여기서 문제가 생기는듯 하다. 이런식으로 변환해서 쓸수는 없는것 같다.
}

BOOL ApplicationLayer::Receive(unsigned char * ppayload) {
	AfxMessageBox("어플리케이션 Receive 호출됨");
	APPDATA* appdata = (APPDATA*)ppayload;

	//대화상자에서 '보낸이'주소정보를 띄워주기위해 필요한 작업
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageDstAddress(appdata->_dstAddress);
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageSrcAddress(appdata->_srcAddress);
	((CChattingApplicationDlg*)this->GetUpperLayer())->SetCurrentMessageSize(appdata->_dataSize);

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char* ppayload, int appdataSize) {
	//AfxMessageBox("어플리케이션 Send 호출됨");
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
