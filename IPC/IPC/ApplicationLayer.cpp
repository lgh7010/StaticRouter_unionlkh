#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
}
ApplicationLayer::~ApplicationLayer(){}



BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	AfxMessageBox(_T("���ø����̼� Receive ȣ���"));
	APPDATA* appdata = (APPDATA*)ppayload;

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char * ppayload, int appdataSize){
	AfxMessageBox(_T("���ø����̼� Send ȣ���"));
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
