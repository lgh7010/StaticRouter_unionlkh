#include "stdafx.h"
#include "DatalinkLayer.h"

//���� MAC�ּҸ� �޾ƿ��� ���� �߰���
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")


DatalinkLayer::DatalinkLayer(char* pName) : LayerStructure(pName) {
	this->ResetFrame();
}
DatalinkLayer::~DatalinkLayer() {}
void DatalinkLayer::Refresh() {}

BOOL DatalinkLayer::Receive(unsigned char * ppayload) {
	//AfxMessageBox("�����͸�ũ Receive ȣ���");
	ETHERNET_FRAME* pFrame = (ETHERNET_FRAME*)ppayload;

	BOOL isDone;
	isDone = this->GetUpperLayer()->Receive((unsigned char*)pFrame->EF_data);
	return isDone;
}

BOOL DatalinkLayer::Send(unsigned char * ppayload, int packetLength) {
	//AfxMessageBox("�����͸�ũ Send ȣ���");
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
	//memset(this->_frame.EF_dstaddr, 0, 6);//�̰� ������ ���� ������ MAC(Ȥ�� ��밡���� ������� MAC)���� �����س��´�.
	//16.8.30 ���� GROW���� MAC �ּ� : 54-b8-0a-aa-98-04

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

CString DatalinkLayer::getLocalMACAddress(){//���� MAC�ּҸ� �޾ƿ��� �Լ�. �۵��� Ȯ���ߴ�.
	/* �Ʒ� ����� �Բ� ���Ǿ�� �Ѵ�.
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
