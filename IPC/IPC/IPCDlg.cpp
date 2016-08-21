
// IPCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IPC.h"
#include "IPCDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPCDlg dialog



CIPCDlg::CIPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IPC_DIALOG, pParent), LayerStructure("DLG")
	, _message("")
	, _isBroadcastMode(FALSE)
	, _srcAddress(0)
	, _dstAddress(0)
	, _chatList()//("")왜 이게 매개로 기본적으로 들어가있었던거지
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	/*
		이 아래에 프로그램 초기화와 관련된 코드를 모두 입력한다. 각 레이어 객체들 생성, Upper/Under레이어 관계 설정까지.
	*/
	this->_isBroadcastMode = FALSE;
	this->_sendReady = FALSE;
	this->_doesGetAck = FALSE;
	//this->_sendButton.EnableWindow(FALSE);//이걸 왜 여기서 하면 에러가 나는거지? 아마 대화상자가 제대로 만들어지기 전에 그걸 Disable부터 하려고 해서인듯

	//각 레이어 객체를 만들고, 서로 상/하위 관계 설정
	this->_pPhysicsLayer = new PhysicsLayer("PHY");
	this->_pDatalinkLayer = new DatalinkLayer("DTL");
	this->_pNetworkLayer = new NetworkLayer("NET");
	this->_pTransportLayer = new TransportLayer("TRP");
	this->_pApplicationLayer = new ApplicationLayer("APP");

	this->LinkLayers();
}
void CIPCDlg::LinkLayers() {
	this->_pPhysicsLayer->SetUpperLayer(this->_pDatalinkLayer);
	this->_pDatalinkLayer->SetUnderLayer(this->_pPhysicsLayer);

	this->_pDatalinkLayer->SetUpperLayer(this->_pNetworkLayer);
	this->_pNetworkLayer->SetUnderLayer(this->_pDatalinkLayer);

	this->_pNetworkLayer->SetUpperLayer(this->_pTransportLayer);
	this->_pTransportLayer->SetUnderLayer(this->_pNetworkLayer);

	this->_pTransportLayer->SetUpperLayer(this->_pApplicationLayer);
	this->_pApplicationLayer->SetUnderLayer(this->_pTransportLayer);

	this->_pApplicationLayer->SetUpperLayer(this);
	this->SetUnderLayer(this->_pApplicationLayer);
}

void CIPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, _message);
	DDX_Check(pDX, IDC_CHECK_BROAD, _isBroadcastMode);
	DDX_Text(pDX, IDC_EDIT1, _srcAddress);
	DDX_Text(pDX, IDC_EDIT2, _dstAddress);
	DDX_Control(pDX, IDC_EDIT1, _srcAddressPanel);
	DDX_Control(pDX, IDC_EDIT2, _dstAddressPanel);
	DDX_Control(pDX, IDC_BUTTON_SET, _setResetButton);
	DDX_Control(pDX, IDC_CHECK_BROAD, _broadcastPanel);
	DDX_Control(pDX, IDC_LIST1, _chatList);
	DDX_Control(pDX, IDC_BUTTON_SEND, _sendButton);
}

UINT RegSendMsg;//메시지는 헤더에 선언하고 this->로 사용할 수 없다.
UINT RegAckMsg;//그리고 선언 자체도 헤더에 하면 컴파일에러가 나는데, 구체적인 이유는 아직 모르겠다. 그리고 이 변수이름은 당연히 자유롭다.

BEGIN_MESSAGE_MAP(CIPCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CIPCDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CIPCDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_BROAD, &CIPCDlg::OnCheckBroadcast)

	/////////이하는 내가 추가한 메시지맵. REGISTERED_MESSAGE는 이미 있는거네??
	//On_REGISTERED_MESSAGE(메시지, 해당 메시지로 동작시킬 함수이름)인듯
	ON_REGISTERED_MESSAGE(RegSendMsg, &CIPCDlg::OnSystemMsgSend)
	ON_REGISTERED_MESSAGE(RegAckMsg, &CIPCDlg::OnSystemMsgAck)
	
END_MESSAGE_MAP()


// CIPCDlg message handlers

BOOL CIPCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	this->_sendButton.EnableWindow(FALSE);//이걸 위에서 초기화하는거 다 넣는곳에 넣으려고 했는데 그러면 에러가 나서 할수없이 여기에 넣었다.

	//System메시지 설정. 이걸 Dlg생성자에서 하면 당연히 선언되어있지 않다는 빌드에러가 뜬다. 그래서 여기에 둔다.
	RegSendMsg = RegisterWindowMessage("SendMsg");
	RegAckMsg = RegisterWindowMessage("AckMsg");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIPCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIPCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIPCDlg::OnBnClickedButtonSet(){//Set버튼을 누른 경우의 작동
	UpdateData(TRUE);

	if (this->_sendReady) {//리셋을 누르는 경우임
		this->_setResetButton.SetWindowText("Set");
		this->_sendReady = FALSE;

		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_srcAddressPanel.EnableWindow(TRUE);
		this->_broadcastPanel.EnableWindow(TRUE);
		this->_sendButton.EnableWindow(FALSE);
	} else {//셋을 누르는 경우임
		if (!this->_srcAddress || !this->_dstAddress) {
			MessageBox("주소가 올바르지 않습니다.", "경고", MB_OK | MB_ICONSTOP);
			return;
		}

		this->_setResetButton.SetWindowText("Reset");
		this->_sendReady = TRUE;
		this->_pApplicationLayer->SetSrcAddress(this->_srcAddress);
		this->_pApplicationLayer->SetDstAddress(this->_dstAddress);

		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_srcAddressPanel.EnableWindow(FALSE);
		this->_broadcastPanel.EnableWindow(FALSE);
		this->_sendButton.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}


void CIPCDlg::OnBnClickedButtonSend(){//Send버튼을 누른 경우의 작동
	UpdateData(TRUE);

	if (!this->_message.IsEmpty()) {
		//호스트에서의 채팅 처리
		CString msgFront;
		if (this->_isBroadcastMode) {
			msgFront.Format("[%d:BROADCAST] ", this->_srcAddress);
		} else {
			msgFront.Format("[%d:%d] ", this->_srcAddress, this->_dstAddress);
		}
		this->_chatList.AddString(msgFront + this->_message);

		//송신 처리
		this->_doesGetAck = FALSE;//수신자가 브로드캐스트된 레지스터메시지를 받으면 ACK 메시지를 브로드캐스트하는데, 그걸 받으면 이걸 TRUE로 바꾼다. 이 변수는 최초 송신자가 자신이 보낸걸 수신자가 잘 받았는지 안받았는지 체크한다.																

		int msgLength = this->_message.GetLength();
		unsigned char* ppayload = new unsigned char[msgLength + 1];
		memcpy(ppayload, (unsigned char*)(LPCTSTR)this->_message, msgLength);
		ppayload[msgLength] = '\0';

		this->_pApplicationLayer->SetDstAddress(this->_dstAddress);//어플리케이션층의 헤더 정보를 만들어 주고 나서, Send로 '데이터'를 전송한다.
		this->_pApplicationLayer->SetSrcAddress(this->_srcAddress);//즉, 여기서 ppayload는 오로지 유저가 입력한 문자열 데이터만을 의미한다.
		this->_pApplicationLayer->Send(ppayload, this->_message.GetLength());//길이는 잘 전달 되는것을 확인했다.

		//시스템의 모든 프로세스에게 메시지를 보낸다. 이 메시지를 받은 IPC프로그램은 물리층에서부터 레이어들을 거쳐 올라오며, 최종적으로 올바른 수신자에게만 메시지를 표시하게 된다
		::SendMessageA(HWND_BROADCAST, RegSendMsg, 0, 0);

		//전송이 잘 완료되면, 호스트의 채팅창을 비워야 한다.
		this->_message = "";
	}

	UpdateData(FALSE);
}

void CIPCDlg::OnCheckBroadcast(){
	if (this->_isBroadcastMode) {
		UpdateData(TRUE);
		this->_dstAddress = 0;
		this->_dstAddressPanel.SetWindowText("0");
		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_isBroadcastMode = FALSE;
	} else {
		UpdateData(TRUE);
		this->_dstAddress = (unsigned int)0xff;
		this->_dstAddressPanel.SetWindowText("255");
		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_isBroadcastMode = TRUE;
	}
}

BOOL CIPCDlg::Receive(unsigned char* ppayload){
	if (!this->_doesGetAck)
		::SendMessageA(HWND_BROADCAST, RegAckMsg, 0, 0);
	
	this->_chatList.AddString((char*) ppayload);
	return TRUE;
}

LRESULT CIPCDlg::OnSystemMsgSend(WPARAM wParam, LPARAM lParam){
	AfxMessageBox("SystemMsg Send 수신");
	this->_pPhysicsLayer->Receive();
	return 0;
}

LRESULT CIPCDlg::OnSystemMsgAck(WPARAM wParam, LPARAM lParam) {
	AfxMessageBox("ACK 수신");
	if (!this->_doesGetAck) {
		this->_doesGetAck = TRUE;
	}
	return 0;
}
