#include "stdafx.h"
#include "NetworkLayer.h"


NetworkLayer::NetworkLayer(){
	this->ResetHeader();
}
NetworkLayer::~NetworkLayer(){}




BOOL NetworkLayer::Receive(unsigned char * ppayload){
	PACKET*	pPacket = (PACKET*)ppayload;

	BOOL isDone;
	isDone = this->GetUpperLayer()->Receive((unsigned char*)pPacket->IP_data);
	return isDone;
}

BOOL NetworkLayer::Send(unsigned char * ppayload, int segmentLength){
	memcpy(this->_packet.IP_data, ppayload, segmentLength);

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_packet, segmentLength + IP_PACKET_HEADER_SIZE);
	return isDone;
}




void NetworkLayer::SetDstAddress(unsigned char* pAddress){
	memcpy(this->_packet.IP_dst, pAddress, 4);
}
void NetworkLayer::SetSrcAddress(unsigned char* pAddress){
	memcpy(this->_packet.IP_src, pAddress, 4);
}
unsigned char* NetworkLayer::GetDstAddress(){
	return this->_packet.IP_dst;
}
unsigned char* NetworkLayer::GetSrcAddress(){
	return this->_packet.IP_src;
}

void NetworkLayer::ResetHeader(){
	this->_packet.IP_VaL = 0;
	this->_packet.IP_tos = 0;
	this->_packet.IP_length = 0;
	this->_packet.IP_id = 0;
	this->_packet.IP_FAFO = 0;
	this->_packet.IP_ttl = 0;
	this->_packet.IP_protocol = 0;
	this->_packet.IP_HC = 0;
	memset(this->_packet.IP_src, 0, 4);
	memset(this->_packet.IP_dst, 0, 4);
	memset(this->_packet.IP_data, 0, IP_PACKET_DATA_SIZE);
}
