#include "stdafx.h"
#include "ApplicationLayer.h"
#include "IPCDlg.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
}
ApplicationLayer::~ApplicationLayer(){}



BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	AfxMessageBox("���ø����̼� Receive ȣ���");
	APPDATA* appdata = (APPDATA*)ppayload;
	
	//�ּ�üũ. ����� ��Ʈ��ũ�� ����� �̷����� ������ ���� ���� ������, �ٸ� ���̾���� �ƹ��͵� ���� �ʰ� Application������ �ּ�üũ�� �Ѵ�.
	if ( ( appdata->_dstAddress != this->_appdata._srcAddress && appdata->_dstAddress != 0xff ) ||
		appdata->_srcAddress == this->_appdata._srcAddress ) {
		/*
			�ö�� �۵������� �������ּҿ� �ڽ��� �ҽ��ּҰ� �ٸ��鼭(�ڱ����� �°� �ƴ�) �ö�� �۵������� �������ּҰ� 255�� �ƴϰų�,	(��ε�ĳ��Ʈ�� �ƴ�)
			�ö�� �۵������� ����� �ּҰ� �ڱ� �ڽ��� �ּҿ� ���� ���.(�ڱⰡ ���� �޽����� ���)(RegMsgBroadCast�̹Ƿ� �ڱ� �ڽſ��Ե� �´�)
		*/
		return FALSE;
	}

	/*
		
	*/

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char * ppayload, int appdataSize){
	AfxMessageBox("���ø����̼� Send ȣ���");
	memcpy(this->_appdata._data, ppayload, appdataSize);

	BOOL isDone;
	isDone = this->_pUnderLayer->Send((unsigned char*)&this->_appdata, appdataSize + APP_HEADER_SIZE);
	return isDone;
}




void ApplicationLayer::SetDstAddress(unsigned int address){
	this->_appdata._dstAddress = address;
}
void ApplicationLayer::SetSrcAddress(unsigned int address){
	this->_appdata._srcAddress = address;
}
UINT ApplicationLayer::GetDstAddress(){
	return this->_appdata._dstAddress;
}
UINT ApplicationLayer::GetSrcAddress(){
	return this->_appdata._srcAddress;
}


void ApplicationLayer::ResetHeader(){
	this->_appdata._srcAddress = 0;
	this->_appdata._dstAddress = 0;
	memset(this->_appdata._data, 0, APP_DATA_SIZE);
}
