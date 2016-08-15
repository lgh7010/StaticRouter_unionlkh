#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(){
	this->ResetHeader();
}
ApplicationLayer::~ApplicationLayer(){}

BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	return 0;
}

BOOL ApplicationLayer::Send(unsigned char * ppayload, int appdataSize){
	return 0;
}

void ApplicationLayer::SetDstAddress(unsigned int address){
	this->_appdata._dstAddress = address;
}

void ApplicationLayer::SetSrcAddress(unsigned int address){
	this->_appdata._srcAddress = address;
}

UINT ApplicationLayer::GetDstAddress()
{
	return this->_appdata._dstAddress;
}

UINT ApplicationLayer::GetSrcAddress()
{
	return this->_appdata._srcAddress;
}


void ApplicationLayer::ResetHeader(){
	this->_appdata._srcAddress = 0;
	this->_appdata._dstAddress = 0;
	memset(this->_appdata._data, 0, APP_DATA_SIZE);
}
