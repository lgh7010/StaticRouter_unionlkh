#include "stdafx.h"
#include "DatalinkLayer.h"

DatalinkLayer::DatalinkLayer(char* pName) : LayerStructure(pName) {
	this->ResetFrame();
}
DatalinkLayer::~DatalinkLayer() {}
void DatalinkLayer::Refresh() {}

BOOL DatalinkLayer::Receive(unsigned char * ppayload) {
	//AfxMessageBox("데이터링크 Receive 호출됨");
	ETHERNET_FRAME* pFrame = (ETHERNET_FRAME*)ppayload;

	if (memcmp(pFrame->EF_dstaddr, this->_frame.EF_srcaddr, 6)) {
		BOOL isDone;
		isDone = this->GetUpperLayer()->Receive((unsigned char*)pFrame->EF_data);
		return isDone;
	} else {
		return FALSE;
	}
}

BOOL DatalinkLayer::Send(unsigned char * ppayload, int packetLength) {
	//AfxMessageBox("데이터링크 Send 호출됨");
	memcpy(this->_frame.EF_data, ppayload, packetLength);

	BOOL isDone;
	isDone = this->GetUnderLayer()->Send((unsigned char*)&this->_frame, packetLength + ETHERNET_FRAME_HEADER_SIZE);
	return isDone;
}








void DatalinkLayer::SetDstAddress(unsigned char * pAddress) {
	memcpy(this->_frame.EF_dstaddr, pAddress, 6);
}
void DatalinkLayer::SetSrcAddress(unsigned char * pAddress) {
	memcpy(this->_frame.EF_srcaddr, pAddress, 6);
}
unsigned char* DatalinkLayer::GetDstAddress() {
	return this->_frame.EF_dstaddr;
}
unsigned char* DatalinkLayer::GetSrcAddress() {
	return this->_frame.EF_srcaddr;
}



void DatalinkLayer::ResetFrame() {
	//memset(this->_frame.EF_dstaddr, 0, 6);//이건 무조건 동방 공유기 MAC(혹은 사용가능한 라우터의 MAC)으로 세팅해놓는다.
	//16.8.30 현재 GROW동방 MAC 주소 : 54-b8-0a-aa-98-04

	this->_frame.EF_dstaddr[0] = 0x54;
	this->_frame.EF_dstaddr[1] = 0xb8;
	this->_frame.EF_dstaddr[2] = 0x0a;
	this->_frame.EF_dstaddr[3] = 0xaa;
	this->_frame.EF_dstaddr[4] = 0x98;
	this->_frame.EF_dstaddr[5] = 0x04;

	//memset(this->_frame.EF_srcaddr, 0, 6);
	this->setSrcMACAddress();
	this->_frame.EF_type = 0;
	memset(this->_frame.EF_data, 0, ETHERNET_FRAME_DATA_SIZE);
}

CString DatalinkLayer::getLocalMACAddress(){//로컬 MAC주소를 받아오는 함수. 작동을 확인했다.
	/* 아래 헤더와 함께 사용되어야 한다.
		#include <IPHlpApi.h>
		#pragma comment(lib, "iphlpapi.lib")
	*/
	CString strGateWay = "";
	CString strMACAddress = "";
	IP_ADAPTER_INFO ipAdapterInfo[5];
	DWORD dwBuflen = sizeof(ipAdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(ipAdapterInfo, &dwBuflen);

	if (dwStatus != ERROR_SUCCESS)	{
		strMACAddress.Format("Error for GetAdaptersInfo : %d", dwStatus);
		AfxMessageBox(strMACAddress);
		return "";
	}

	PIP_ADAPTER_INFO pIpAdapterInfo = ipAdapterInfo;

	do {
		strGateWay = (CString)pIpAdapterInfo->GatewayList.IpAddress.String;
		if (strGateWay[0] == '0') {
			pIpAdapterInfo = pIpAdapterInfo->Next;
		} else {
			strMACAddress.Format("%02X-%02X-%02X-%02X-%02X-%02X",
				pIpAdapterInfo->Address[0],
				pIpAdapterInfo->Address[1],
				pIpAdapterInfo->Address[2],
				pIpAdapterInfo->Address[3],
				pIpAdapterInfo->Address[4],
				pIpAdapterInfo->Address[5]
			);

			break;
		}
	} while (pIpAdapterInfo);

	return strMACAddress;
}
void DatalinkLayer::setSrcMACAddress() {
	CString strGateWay = "";
	CString strMACAddress = "";
	IP_ADAPTER_INFO ipAdapterInfo[5];
	DWORD dwBuflen = sizeof(ipAdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(ipAdapterInfo, &dwBuflen);

	if (dwStatus != ERROR_SUCCESS) {
		strMACAddress.Format("Error for GetAdaptersInfo : %d", dwStatus);
		AfxMessageBox(strMACAddress);
		return;
	}

	PIP_ADAPTER_INFO pIpAdapterInfo = ipAdapterInfo;

	do {
		strGateWay = (CString)pIpAdapterInfo->GatewayList.IpAddress.String;
		if (strGateWay[0] == '0') {
			pIpAdapterInfo = pIpAdapterInfo->Next;
		} else {
			this->_frame.EF_srcaddr[0] = pIpAdapterInfo->Address[0];
			this->_frame.EF_srcaddr[1] = pIpAdapterInfo->Address[1];
			this->_frame.EF_srcaddr[2] = pIpAdapterInfo->Address[2];
			this->_frame.EF_srcaddr[3] = pIpAdapterInfo->Address[3];
			this->_frame.EF_srcaddr[4] = pIpAdapterInfo->Address[4];
			this->_frame.EF_srcaddr[5] = pIpAdapterInfo->Address[5];
			break;
		}
	} while (pIpAdapterInfo);
}


















/*#define ETHER_ADDR_LEN	6

int DatalinkLayer::test(){
	int i = 0;
	int length = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	unsigned char packet[1500];

	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *fp;

	bpf_u_int32 net;
	bpf_u_int32 mask;
	struct in_addr net_addr;
	struct in_addr mask_addr;

	LPADAPTER adapter = NULL;
	PPACKET_OID_DATA pOidData;

	ETHERNET_FRAME eth;

	pOidData = (PPACKET_OID_DATA)malloc(sizeof(PACKET_OID_DATA));
	pOidData->Oid = 0x01010101;
	pOidData->Length = 6;

	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	for (d = alldevs; d; d = d->next) {
		printf("%d, %d", ++i, d->name);
		if (d->description)
			printf("(%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	for (d = alldevs, i = 0; i < 1 - 1; d = d->next, i++);

	if (pcap_lookupnet(d->name, &net, &mask, errbuf) < 0) {
		printf("네트워크 디바이스 정보를 가져올 수 없습니다\n");
		return 0;
	}

	adapter = PacketOpenAdapter(d->name);
	PacketRequest(adapter, FALSE, pOidData);

	net_addr.s_addr = net;
	mask_addr.s_addr = mask;

	printf("Device Name : %s \n", d->name);
	printf("Network Address : %s \n", inet_ntoa(net_addr));
	printf("Netmask Info : %s \n", inet_ntoa(mask_addr));
	printf("Mac Address : %02x-%02x-%02x-%02x-%02x-%02x\n",
		pOidData->Data[0], pOidData->Data[1], pOidData->Data[2],
		pOidData->Data[3], pOidData->Data[4], pOidData->Data[5]);

	if ((fp = pcap_open_live(d->name, 65536, 0, 1000, errbuf)) == NULL) {
		return -1;
	}

	memset(packet, 0, sizeof(packet));

	eth.EF_dstaddr[0] = this->_frame.EF_dstaddr[0];	eth.EF_dstaddr[1] = this->_frame.EF_dstaddr[1];
	eth.EF_dstaddr[2] = this->_frame.EF_dstaddr[2];	eth.EF_dstaddr[3] = this->_frame.EF_dstaddr[3];
	eth.EF_dstaddr[4] = this->_frame.EF_dstaddr[4];	eth.EF_dstaddr[5] = this->_frame.EF_dstaddr[5];

	eth.EF_srcaddr[0] = pOidData->Data[0];	eth.EF_srcaddr[1] = pOidData->Data[1];
	eth.EF_srcaddr[2] = pOidData->Data[2];	eth.EF_srcaddr[3] = pOidData->Data[3];
	eth.EF_srcaddr[4] = pOidData->Data[4];	eth.EF_srcaddr[5] = pOidData->Data[5];

	eth.EF_type = 0x1234;

	memcpy(packet, &eth, sizeof(eth));
	length += sizeof(eth);

	if (pcap_sendpacket(fp, packet, length) != 0) {
		fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(fp));
		return -1;
	}

	return 0;
}*/