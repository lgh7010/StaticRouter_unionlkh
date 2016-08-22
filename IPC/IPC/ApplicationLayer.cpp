#include "stdafx.h"
#include "ApplicationLayer.h"


ApplicationLayer::ApplicationLayer(char* pName) : LayerStructure(pName){
	this->ResetHeader();
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//아직 링크가 되기 전에 UpperLayer를 찾으려 하므로 안됨 --> Refresh함수 개념 도입.
}
ApplicationLayer::~ApplicationLayer(){}

void ApplicationLayer::Refresh() {
	//this->_pIPCDlg = (CIPCDlg*)this->GetUpperLayer();//여기서 문제가 생기는듯 하다. 이런식으로 변환해서 쓸수는 없는것 같다.
}

BOOL ApplicationLayer::Receive(unsigned char * ppayload){
	AfxMessageBox("어플리케이션 Receive 호출됨");
	APPDATA* appdata = (APPDATA*)ppayload;
	
	//주소체크. 현재는 네트워크로 통신이 이뤄지는 내용이 전혀 없기 때문에, 다른 레이어에서는 아무것도 하지 않고 Application에서만 주소체크를 한다.
	if ( ( appdata->_dstAddress != this->_appdata._srcAddress && appdata->_dstAddress != 0xff ) ||
		appdata->_srcAddress == this->_appdata._srcAddress ) {
		/*
			올라온 앱데이터의 목적지주소와 자신의 소스주소가 다르면서(자기한테 온게 아님) 올라온 앱데이터의 목적지주소가 255가 아니거나,	(브로드캐스트도 아님)
			올라온 앱데이터의 출발지 주소가 자기 자신의 주소와 같은 경우.(자기가 보낸 메시지인 경우)(RegMsgBroadCast이므로 자기 자신에게도 온다)
		*/
		return FALSE;
	}

	/*
		Send과정에서는 '사용자의 입력 문자열'이 어플리케이션 레이어로 내려온다. 엄밀히, 내려보내기 직전에
		Application레이어의 SRC, DST주소 Setter를 통해 어플리케이션 헤더의 정보를 바꾸고 나서 내려보낸다.

		하지만 Receive과정에서는 Application레이어에서 IPCDlg레이어로 올라가는 정보가 단지 '유저가 입력한 문자열'뿐이다.
		Send때와 동일하게 Receive로 위로 넘기기 직전에 IPCDlg의 어떤 변수(주소를 의미하는)를 수정해주는 방법을 생각할 수 있다.
		하지만 IPCDlg는 모든 레이어를 참조할 수 있는 반면, Application레이어는 단지 LayerStructure의 형태로만 참조가 가능하다.
		따라서 두가지 방법을 생각할 수 있다.

		1. Application레이어에서, 현재 존재하는 IPCDlg레이어를 참조할 수 있는 변수를 어떻게든 만든다. 그런 뒤 Send때와 동일하게 처리한다.

		2. Application레이어에서 현재 올라온 '상대편 유저가 입력한 문자열'을 일단 저장한 뒤, CString의 임시변수를 만들어 Format까지 설정해서
		완전히 IPCDlg의 Receive에서는 그냥 _chatList.AddString()만 하면 되도록 해서 올려보낸다.

		어떤것이 정답이라고 할 수는 없지만, 레이어의 독립성 측면에서는 전자가 더 좋아보인다. 다이얼로그와 관련된 작업(채팅창에 소스주소, 목적주소 보이기)
		은 IPCDlg에서만 처리하는것이 좋기 때문이다. 여기선 1번으로 처리한다. 변수를 만든 방법은, Application레이어의 생성자를 참조한다.
	*/
	//IPCDlg에서 전달받은 채팅메시지(appdata->_data)에다가 올바른 주소 형식을 붙여서([1:2] blabla..)출력할 수 있도록, APP헤더의 주소정보 전달
	//this->_pIPCDlg->SetCurrentMessageDstAddress(appdata->_dstAddress);
	//this->_pIPCDlg->SetCurrentMessageSrcAddress(appdata->_srcAddress);//와 시발 이게 뭐야. 이런게 문제가 생겨서 아래처럼 그냥 그때그때 상위레이어 참조해서 형변환 했더니 빌드에러는 안나네
	((CIPCDlg*)this->GetUpperLayer())->SetCurrentMessageDstAddress(appdata->_dstAddress);
	((CIPCDlg*)this->GetUpperLayer())->SetCurrentMessageSrcAddress(appdata->_srcAddress);
	//시발 뭐지 위에거는 안되고(말도안되는 신택스 빌드에러_알수없는 문제) 아래거는 되는거지

	BOOL isDone;
	isDone = this->_pUpperLayer->Receive((unsigned char*)appdata->_data);
	return isDone;
}

BOOL ApplicationLayer::Send(unsigned char * ppayload, int appdataSize){
	AfxMessageBox("어플리케이션 Send 호출됨");
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
