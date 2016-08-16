#include "stdafx.h"
#include "TransportLayer.h"


TransportLayer::TransportLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
}
TransportLayer::~TransportLayer(){}

BOOL TransportLayer::Receive(unsigned char * ppayload){
	AfxMessageBox(_T("트랜스포트층 Receive"));
	SEGMENT* pSegment = (SEGMENT*)ppayload;

	BOOL isDone;
	isDone = this->GetUpperLayer()->Receive((unsigned char*)pSegment->SG_data);
	return isDone;
}

BOOL TransportLayer::Send(unsigned char * ppayload, int applicationDataSize){
	AfxMessageBox(_T("트랜스포트층 Send"));
	memcpy(this->_segment.SG_data, ppayload, applicationDataSize);

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_segment, applicationDataSize + TP_SEGMENT_HEADER_SIZE);
	return isDone;
}

void TransportLayer::ResetHeader(){
	this->_segment.SG_srcPort = 0;
	this->_segment.SG_dstPort = 0;
	memset(this->_segment.SG_sqcNum, 0, 4);
	memset(this->_segment.SG_ackNum, 0, 4);
	this->_segment.SG_HLRFLG = 0;
	this->_segment.SG_winSize = 0;
	this->_segment.SG_checkSum = 0;
	this->_segment.SG_urgentPoint = 0;
	memset(this->_segment.SG_optionAndPadding, 0, TP_SEGMENT_OP_SIZE);
	memset(this->_segment.SG_data, 0, TP_SEGMENT_DATA_SIZE);
}
