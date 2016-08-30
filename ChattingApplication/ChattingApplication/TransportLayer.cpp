#include "stdafx.h"
#include "TransportLayer.h"


TransportLayer::TransportLayer(char* pName) : LayerStructure(pName) {
	this->ResetHeader();
	this->_segment.SG_srcPort = 567;//�׽�Ʈ�� ����
}
TransportLayer::~TransportLayer() {}

void TransportLayer::Refresh() {}

BOOL TransportLayer::Receive(unsigned char * ppayload) {
	//AfxMessageBox("Ʈ������Ʈ�� Receive");
	SEGMENT* pSegment = (SEGMENT*)ppayload;

	if (pSegment->SG_dstPort == this->_segment.SG_srcPort) {
		BOOL isDone;
		isDone = this->GetUpperLayer()->Receive((unsigned char*)pSegment->SG_data);
		return isDone;
	} else {
		//AfxMessageBox("Ʈ������Ʈ�� Receive : ��Ʈ�ּ� ����ġ");
		return FALSE;
	}
}

BOOL TransportLayer::Send(unsigned char * ppayload, int applicationDataSize) {
	//AfxMessageBox("Ʈ������Ʈ�� Send");
	memcpy(this->_segment.SG_data, ppayload, applicationDataSize);
	this->_segment.SG_dstPort = 567;//�׽�Ʈ�� ����

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_segment, applicationDataSize + TP_SEGMENT_HEADER_SIZE);
	return isDone;
}

unsigned short TransportLayer::getSrcPort(){
	return this->_segment.SG_srcPort;
}

unsigned short TransportLayer::getDstPort(){
	return this->_segment.SG_dstPort;
}

void TransportLayer::ResetHeader() {
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

