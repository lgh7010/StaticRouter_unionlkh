#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//���� ��ũ�� �Ǳ� ���� UpperLayer�� ã���� �ϹǷ� �ȵ� --> Refresh�Լ� ���� ����.
}
ApplicationLayer::~ApplicationLayer(){}

void ApplicationLayer::Refresh() {
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//���⼭ ������ ����µ� �ϴ�. �̷������� ��ȯ�ؼ� ������ ���°� ����.
}

BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	AfxMessageBox("���ø����̼� Receive ȣ���");
	APPDATA* appdata = (APPDATA*)ppayload;
	
	//�ּ�üũ. ����� ��Ʈ��ũ�� ����� �̷����� ������ ���� ���� ������, �ٸ� ���̾���� �ƹ��͵� ���� �ʰ� Application������ �ּ�üũ�� �Ѵ�.
	if ( ( appdata->_dstAddress != this->_appdata._srcAddress && appdata->_dstAddress != 0xff ) ||
		appdata->_srcAddress == this->_appdata._srcAddress ) {
		/*
			�ö�� �۵������� �������ּҿ� �ڽ��� �ҽ��ּҰ� �ٸ��鼭(�ڱ����� �°� �ƴ�) �ö�� �۵������� �������ּҰ� 255�� �ƴϰų�,	(��ε�ĳ��Ʈ�� �ƴ�)
			�ö�� �۵������� ����� �ּҰ� �ڱ� �ڽ��� �ּҿ� ���� ���.(�ڱⰡ ���� �޽����� ���)(RegMsgBroadCast�̹Ƿ� �ڱ� �ڽſ��Ե� �´�)
		*/
		return FALSE;
	}

	/*
		Send���������� '������� �Է� ���ڿ�'�� ���ø����̼� ���̾�� �����´�. ������, ���������� ������
		Application���̾��� SRC, DST�ּ� Setter�� ���� ���ø����̼� ����� ������ �ٲٰ� ���� ����������.

		������ Receive���������� Application���̾�� IPCDlg���̾�� �ö󰡴� ������ ���� '������ �Է��� ���ڿ�'���̴�.
		Send���� �����ϰ� Receive�� ���� �ѱ�� ������ IPCDlg�� � ����(�ּҸ� �ǹ��ϴ�)�� �������ִ� ����� ������ �� �ִ�.
		������ IPCDlg�� ��� ���̾ ������ �� �ִ� �ݸ�, Application���̾�� ���� LayerStructure�� ���·θ� ������ �����ϴ�.
		���� �ΰ��� ����� ������ �� �ִ�.

		1. Application���̾��, ���� �����ϴ� IPCDlg���̾ ������ �� �ִ� ������ ��Ե� �����. �׷� �� Send���� �����ϰ� ó���Ѵ�.

		2. Application���̾�� ���� �ö�� '����� ������ �Է��� ���ڿ�'�� �ϴ� ������ ��, CString�� �ӽú����� ����� Format���� �����ؼ�
		������ IPCDlg�� Receive������ �׳� _chatList.AddString()�� �ϸ� �ǵ��� �ؼ� �÷�������.

		����� �����̶�� �� ���� ������, ���̾��� ������ ���鿡���� ���ڰ� �� ���ƺ��δ�. ���̾�α׿� ���õ� �۾�(ä��â�� �ҽ��ּ�, �����ּ� ���̱�)
		�� IPCDlg������ ó���ϴ°��� ���� �����̴�. ���⼱ 1������ ó���Ѵ�. ������ ���� �����, Application���̾��� �����ڸ� �����Ѵ�.
	*/
	//IPCDlg���� ���޹��� ä�ø޽���(appdata->_data)���ٰ� �ùٸ� �ּ� ������ �ٿ���([1:2] blabla..)����� �� �ֵ���, APP����� �ּ����� ����
	//this->_pIPCDlg->SetCurrentMessageDstAddress(appdata->_dstAddress);
	//this->_pIPCDlg->SetCurrentMessageSrcAddress(appdata->_srcAddress);//�� �ù� �̰� ����. �̷��� ������ ���ܼ� �Ʒ�ó�� �׳� �׶��׶� �������̾� �����ؼ� ����ȯ �ߴ��� ���忡���� �ȳ���
	((CIPCDlg*)this->GetUpperLayer())->SetCurrentMessageDstAddress(appdata->_dstAddress);
	((CIPCDlg*)this->GetUpperLayer())->SetCurrentMessageSrcAddress(appdata->_srcAddress);
	//�ù� ���� �����Ŵ� �ȵǰ�(�����ȵǴ� ���ý� ���忡��_�˼����� ����) �Ʒ��Ŵ� �Ǵ°���

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char * ppayload, int appdataSize){
	AfxMessageBox("���ø����̼� Send ȣ���");
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
