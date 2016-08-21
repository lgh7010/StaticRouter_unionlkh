#include "stdafx.h"
#include "DatalinkLayer.h"


DatalinkLayer::DatalinkLayer(char* pName) : LayerStructure(pName){
	this->ResetFrame();
}
DatalinkLayer::~DatalinkLayer(){}

/*							<Receive 설명>
1. 하위레이어(Physics)에서 '프레임'이 올라오면, 그 프레임에서 '이더넷 헤더부분'을 제거한다
2. 남은것(data부분)을 상위레이어(network)에게 올려준다. 이때 올려주는것은 당연히 '패킷'이다(이더넷 프레임의 '데이터'에 들어있는건 '패킷'이므로)

그런데 여기서는 그냥, 올라온 프레임을 pFrame이라는 변수에 일단 저장하고 거기서 EF_data부분만을 상위레이어의 Receive로 올려보내주는 것으로 끝낸다.
실제로 프레임 헤더에 포함된 정보로 하는 일은 아무것도 없는것이다.

사실, 실제 컴퓨터간 통신에서는 바로 여기서 MAC주소를 비교하는 작업이 이뤄져야 한다.
*/
BOOL DatalinkLayer::Receive(unsigned char * ppayload) {
	AfxMessageBox("데이터링크 Receive 호출됨");
	ETHERNET_FRAME* pFrame = (ETHERNET_FRAME*)ppayload;

	BOOL isDone;
	isDone = this->GetUpperLayer()->Receive((unsigned char*)pFrame->EF_data);
	return isDone;
}


/*							<Send 설명>
1. 상위 레이어(Network)에서 '패킷'이 내려오면, 그 패킷(ppayload)을 EthernetFrame의 data에 집어넣는다.
2. 이렇게 만들어진 '프레임'을 하위레이어(Physics)에 전달한다. 전달하는 방식은 프레임이 저장된 메모리의 시작주소와, 프레임의 크기를 알려주는것이다.

하지만, 사실 현재 this->ethernetFrame의 data부분을 제외한 모든 부분에는 그냥 다 0이 들어있을 뿐이다.(EthernetLayer 생성자가 ResetHeader()를 호출했다)
지금은 컴퓨터간 통신이 아니기 때문에 IP주소가 필요하지 않기 때문이다.
*/
BOOL DatalinkLayer::Send(unsigned char * ppayload, int packetLength) {
	AfxMessageBox("데이터링크 Send 호출됨");
	memcpy(this->_frame.EF_data, ppayload, packetLength);

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_frame, packetLength + ETHERNET_FRAME_HEADER_SIZE);
	return isDone;
}



void DatalinkLayer::SetDstAddress(unsigned char * pAddress) {
	//this->_ethernetFrame.EF_dstaddr = pAddress; ==>이렇게 하는게 아니고, 아래와같이 해야 한다.
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