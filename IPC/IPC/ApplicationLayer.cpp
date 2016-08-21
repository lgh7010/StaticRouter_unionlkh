#include "stdafx.h"
#include "ApplicationLayer.h"
#include "IPCDlg.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
}
ApplicationLayer::~ApplicationLayer(){}



BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	AfxMessageBox("어플리케이션 Receive 호출됨");
	APPDATA* appdata = (APPDATA*)ppayload;
	
	//주소체크. 현재는 네트워크로 통신이 이뤄지는 내용이 전혀 없기 때문에, 다른 레이어에서는 아무것도 하지 않고 Application에서만 주소체크를 한다.
	if ( ( appdata->_dstAddress != this->_appdata._srcAddress && appdata->_dstAddress != 0xff ) ||
		appdata->_srcAddress == this->_appdata._srcAddress ) {
		/*
			올라온 앱데이터의 목적지주소와 자신의 소스주소가 다르면서(자기한테 온게 아님) 올라온 앱데이터의 목적지주소가 255가 아니거나,	(브로드캐스트도 아님)
			올라온 앱데이터의 출발지 주소가 자기 자신의 주소와 같은 경우.(자기가 보낸 메시지인 경우)(RegMsgBroadCast이므로 자기 자신에게도 온다)
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
	AfxMessageBox("어플리케이션 Send 호출됨");
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
