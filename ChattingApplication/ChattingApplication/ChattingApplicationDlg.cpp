
// ChattingApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChattingApplication.h"
#include "ChattingApplicationDlg.h"
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


// CChattingApplicationDlg dialog



CChattingApplicationDlg::CChattingApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATTINGAPPLICATION_DIALOG, pParent), LayerStructure("DLG")
	, _message("")
	, _doesBroadcast(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->_doesBroadcast = FALSE;
	this->_sendReady = FALSE;
	this->_currentMessageSize = 0;
	memset(this->_currentMessageDstAddress, 0, 4);
	memset(this->_currentMessageSrcAddress, 0, 4);

	//각 레이어 객체를 만들고, 서로 상/하위 관계 설정
	this->_pPhysicsLayer = new PhysicsLayer("PHY");
	this->_pDatalinkLayer = new DatalinkLayer("DTL");
	this->_pNetworkLayer = new NetworkLayer("NET");
	this->_pTransportLayer = new TransportLayer("TRP");
	this->_pApplicationLayer = new ApplicationLayer("APP");

	this->LinkLayers();
	this->Refresh();
}
void CChattingApplicationDlg::LinkLayers() {
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

void CChattingApplicationDlg::Refresh() {
	this->_pPhysicsLayer->Refresh();
	this->_pDatalinkLayer->Refresh();
	this->_pNetworkLayer->Refresh();
	this->_pTransportLayer->Refresh();
	this->_pApplicationLayer->Refresh();
}

void CChattingApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _message);
	DDX_Check(pDX, IDC_CHECK_Broadcast, _doesBroadcast);
	DDX_Control(pDX, IDC_BUTTON_Set, _setButton);
	DDX_Control(pDX, IDC_BUTTON_Send, _sendButton);
	DDX_Control(pDX, IDC_LIST1, _chatList);
	DDX_Control(pDX, IDC_IPADDRESS1, _srcAddressPanel);
	DDX_Control(pDX, IDC_IPADDRESS2, _dstAddressPanel);
	DDX_Control(pDX, IDC_CHECK_Broadcast, _broadcastPanel);
}

BEGIN_MESSAGE_MAP(CChattingApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Send, &CChattingApplicationDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_Broadcast, &CChattingApplicationDlg::OnBnClickedCheckBroadcast)
	ON_BN_CLICKED(IDC_BUTTON_Set, &CChattingApplicationDlg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CChattingApplicationDlg message handlers

BOOL CChattingApplicationDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	///////////////////////////////////////////////////////////////////////////////Under this was added by me
	UpdateData(TRUE);

	memcpy(this->_srcAddress, this->_pNetworkLayer->GetSrcAddress(), 4);
	this->_srcAddressPanel.SetAddress(this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);
	this->_srcAddressPanel.EnableWindow(FALSE);
	this->_dstAddressPanel.SetAddress(0, 0, 0, 0);
	this->_dstAddressPanel.GetAddress(this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);

	UpdateData(FALSE);
	///////////////////////////////////////////////////////////////////////////////Over this was added by me

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChattingApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChattingApplicationDlg::OnPaint()
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
HCURSOR CChattingApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CChattingApplicationDlg::Receive(unsigned char* ppayload) {
	unsigned char* buffer = new unsigned char[APP_DATA_SIZE];//하위에서 Receive로 올려보내는건, 어쨋든 APP자료구조의 data부분이므로 이 크기를 넘지 않음
	memset(buffer, '\0', APP_DATA_SIZE);
	memcpy(buffer, ppayload, this->_currentMessageSize);

	CString messageFormat;
	if (this->_currentMessageDstAddress[0] == 0xff &&
		this->_currentMessageDstAddress[1] == 0xff && 
		this->_currentMessageDstAddress[2] == 0xff && 
		this->_currentMessageDstAddress[3] == 0xff) {
		messageFormat.Format("[%d.%d.%d.%d : BROADCAST] %s", this->_currentMessageSrcAddress[0], this->_currentMessageSrcAddress[1], this->_currentMessageSrcAddress[2], this->_currentMessageSrcAddress[3], buffer);
	} else {
		messageFormat.Format("[%d.%d.%d.%d : %d.%d.%d.%d] %s", this->_currentMessageSrcAddress[0], this->_currentMessageSrcAddress[1], this->_currentMessageSrcAddress[2], this->_currentMessageSrcAddress[3], this->_currentMessageDstAddress[0], this->_currentMessageDstAddress[1], this->_currentMessageDstAddress[2], this->_currentMessageDstAddress[3], buffer);
	}

	this->_chatList.AddString(messageFormat);
	return TRUE;
}

void CChattingApplicationDlg::OnBnClickedButtonSend(){
	UpdateData(TRUE);

	if (!this->_message.IsEmpty()) {
		//호스트에서의 채팅 처리
		CString msgFront;
		if (this->_doesBroadcast) {
			msgFront.Format("[%d.%d.%d.%d : BROADCAST] ", this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);
		} else {
			msgFront.Format("[%d.%d.%d.%d : %d.%d.%d.%d] ", this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3], this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);
		}
		this->_chatList.AddString(msgFront + this->_message);

		//송신 처리
		int msgLength = this->_message.GetLength();
		unsigned char* ppayload = new unsigned char[msgLength + 1];
		memcpy(ppayload, (unsigned char*)(LPCTSTR)this->_message, msgLength);
		ppayload[msgLength] = '\0';

		this->_pApplicationLayer->SetDstAddress(this->_dstAddress);//어플리케이션층의 헤더 정보를 만들어 주고 나서, Send로 '데이터'를 전송한다.
		this->_pApplicationLayer->SetSrcAddress(this->_srcAddress);//즉, 여기서 ppayload는 오로지 유저가 입력한 문자열 데이터만을 의미한다.
		this->_pApplicationLayer->SetSize(msgLength);
		this->_pApplicationLayer->Send(ppayload, msgLength);

		this->_pNetworkLayer->SetDstAddress(this->_dstAddress);//네트워크층의 목적지 IP주소만 원하는곳으로 해놓고, 데이터링크층의 목적지 MAC주소는 무조건 동방 공유기로 해놓고 프레임을 전송하면 아마도 원하는곳으로 전송될 것이다.

		//전송이 잘 완료되면, 호스트의 채팅창을 비워야 한다.
		this->_message = "";
	}

	UpdateData(FALSE);
}
/*
	진정한 네트워크라면....

	Application레이어에서는 카톡처럼 예를들어서 '철수'에게 보낸다는걸 지정한다.

	Transport레이어에서는 그 철수에게 전송할 방식을 지정한다.(맞나?)

	Network레이어에서는 철수에게 전송하기 위해서는 연결되어있는 라우터/컴퓨터들중 누구에게 전송해야 하는지 지정한다.
			(멀리 전송할 수 있도록 해줌)(또, MAC을 모르는경우 MAC도 알아냄 - ARP)
			또, 그 라우터(철수에게 가는 데이터를 중간에 받은)는 다시 또 이 데이터가 철수에게 가기 위해서는 자신과 연결되어있는
			라우터/컴퓨터들중 누구에게 전송해야 하는지 지정한다. 이것이 반복되어 멀리까지 전송된다.

	Datalink레이어에서는 연결되어 있는 기기끼리 직접적인 '전송'을 담당한다(MAC이용)

	Physics레이어에서는 실제로 데이터를 '시그널'로 바꾸어서, 전깃줄에 전압을 걸어준다.
*/
/*
	하지만 여기서는 편의상 그냥 Application레이어에서 바로 ip주소를 줘버렸다.
	사실, 네트워크레이어에서는 이 목적지주소를 가지고 라우팅테이블을 이용해 목적지 IP로 보내려면 어떤 라우터(혹은 라우터 기능을 하는 컴퓨터)
	에게 보내야 하는지 알아내고, 그 컴퓨터의 MAC이 없는경우에는 MAC까지 정해가면서 바쁜 작업을 수행해야 한다.

	네트워크레이어가 MAC을 알아내면, 데이터링크레이어는 실제로 그 MAC에게 정보를 전송해야 한다. (pcap_sendPacket함수 이용)
	사실 물리층은 소프트웨어적으로 할 필요가 없다. pcap함수가 알아서 랜카드에 정보를 전송하고, 실제로 보내는건 랜카드가 하기 때문이다.

	pcap함수는 실제로 내가 만든 구조의 이더넷 프레임을 전송하기 때문에, 나는 데이터링크층의 자료구조(프레임)에 올바른 목적지 MAC주소를
	넣어주어야 한다. 하지만 나는 아직 다이나믹 라우터를 만들지 못했기 때문에, 상대방의 MAC주소를 알아낼 방법이 없다.
	따라서, 나는 다소 작위적이지만, 특정 컴퓨터로의 전송만을 목표로 한다.

	아 잠깐만. 학교에 있는 라우터 어딘가의 주소만 알아내면... 나머지는 그 라우터가 알아서 할테니까 나는 그냥 무조건 항상 학교 라우터로
	프레임을 보내면 되잖아? 목적지 주소IP만 '패킷'자료구조에 잘 적어놓고, MAC은 무조건 그로우 라우터로 해놓으면,
	내가 이 채팅프로그램으로 보내는 모든 프레임은 그로우 라우터로 갈테고, 그로우 라우터는 그걸 까서 목적지 IP를 확인하고, 알아서
	잘 보내주겠지!
*/
/*
	그러면 정해졌다. 일반적인 패킷의 TCP층 자료구조의 일반적인 값들을 알아내서 적어넣고, 네트워크층의 목적지주소 IP는 '채팅앱'에서 바로 정하도록
	한 뒤, 데이터링크층의 목적지 주소는 무조건 그로우의 라우터로 해놓고, 데이터링크층에서 pcap_send로 그로우 라우터로 패킷을 보내자.

*/

void CChattingApplicationDlg::OnBnClickedCheckBroadcast(){
	if (this->_doesBroadcast) {
		UpdateData(TRUE);
		this->_dstAddress[0] = 0;
		this->_dstAddress[1] = 0;
		this->_dstAddress[2] = 0;
		this->_dstAddress[3] = 0;
		this->_dstAddressPanel.SetWindowText("0,0,0,0");
		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_doesBroadcast = FALSE;
	}
	else {
		UpdateData(TRUE);
		this->_dstAddress[0] = 0xff;
		this->_dstAddress[1] = 0xff;
		this->_dstAddress[2] = 0xff;
		this->_dstAddress[3] = 0xff;
		this->_dstAddressPanel.SetAddress(this->_srcAddress[0], this->_srcAddress[1], 0, 0);//현재 충남대학교는 B클래스 주소 사용
		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_doesBroadcast = TRUE;
	}
}

void CChattingApplicationDlg::OnBnClickedButtonSet(){ //소스, 목적지 주소값을 _srcAddress, _dstAddress변수에 세팅
	UpdateData(TRUE);

	if (this->_sendReady) {//리셋을 누르는 경우임
		this->_setButton.SetWindowText("Set");
		this->_sendReady = FALSE;

		if(!this->_doesBroadcast)
			this->_dstAddressPanel.EnableWindow(TRUE);
		//this->_srcAddressPanel.EnableWindow(TRUE);//소스주소는 바뀔 이유가 없다.
		this->_broadcastPanel.EnableWindow(TRUE);
		this->_sendButton.EnableWindow(FALSE);
	}
	else {//셋을 누르는 경우임
		unsigned char p1, p2, p3, p4;
		//this->_srcAddressPanel.GetAddress(t1, t2, t3, t4);//소스주소는 바뀔 이유가 없다.

		this->_setButton.SetWindowText("Reset");
		this->_sendReady = TRUE;
		this->_srcAddressPanel.GetAddress(this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);
		this->_dstAddressPanel.GetAddress(this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);

		this->_dstAddressPanel.EnableWindow(FALSE);
		//this->_srcAddressPanel.EnableWindow(FALSE);//소스주소는 바뀔 이유가 없다.
		this->_broadcastPanel.EnableWindow(FALSE);
		this->_sendButton.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}












void CChattingApplicationDlg::SetCurrentMessageSrcAddress(unsigned char* pAddress) {
	memcpy(this->_currentMessageSrcAddress, pAddress, 4);
}
void CChattingApplicationDlg::SetCurrentMessageDstAddress(unsigned char* pAddress) {
	memcpy(this->_currentMessageDstAddress, pAddress, 4);
}
void CChattingApplicationDlg::SetCurrentMessageSize(int n) {
	this->_currentMessageSize = n;
}