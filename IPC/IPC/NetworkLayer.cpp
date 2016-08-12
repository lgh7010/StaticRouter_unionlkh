#include "stdafx.h"
#include "NetworkLayer.h"


NetworkLayer::NetworkLayer(){

}
NetworkLayer::~NetworkLayer(){}




BOOL NetworkLayer::Receive(unsigned char * ppayload){
	return 0;
}

BOOL NetworkLayer::Send(unsigned char * ppayload, int segmentLength){
	return 0;
}




void NetworkLayer::SetDstAddress(unsigned char * pAddress){

}
void NetworkLayer::SetSrcAddress(unsigned char * pAddress){

}
unsigned char * NetworkLayer::GetDstAddress(){
	return nullptr;
}
unsigned char * NetworkLayer::GetSrcAddress(){
	return nullptr;
}
