
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
	, _chatList()//("")�� �̰� �Ű��� �⺻������ ���־�������
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	/*
		�� �Ʒ��� ���α׷� �ʱ�ȭ�� ���õ� �ڵ带 ��� �Է��Ѵ�. �� ���̾� ��ü�� ����, Upper/Under���̾� ���� ��������.
	*/
	this->_isBroadcastMode = FALSE;
	this->_sendReady = FALSE;
	this->_doesGetAck = FALSE;
	//this->_sendButton.EnableWindow(FALSE);//�̰� �� ���⼭ �ϸ� ������ ���°���? �Ƹ� ��ȭ���ڰ� ����� ��������� ���� �װ� Disable���� �Ϸ��� �ؼ��ε�

	//�� ���̾� ��ü�� �����, ���� ��/���� ���� ����
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

UINT RegSendMsg;//�޽����� ����� �����ϰ� this->�� ����� �� ����.
UINT RegAckMsg;//�׸��� ���� ��ü�� ����� �ϸ� �����Ͽ����� ���µ�, ��ü���� ������ ���� �𸣰ڴ�. �׸��� �� �����̸��� �翬�� �����Ӵ�.

BEGIN_MESSAGE_MAP(CIPCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CIPCDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CIPCDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_BROAD, &CIPCDlg::OnCheckBroadcast)

	/////////���ϴ� ���� �߰��� �޽�����. REGISTERED_MESSAGE�� �̹� �ִ°ų�??
	//On_REGISTERED_MESSAGE(�޽���, �ش� �޽����� ���۽�ų �Լ��̸�)�ε�
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
	this->_sendButton.EnableWindow(FALSE);//�̰� ������ �ʱ�ȭ�ϴ°� �� �ִ°��� �������� �ߴµ� �׷��� ������ ���� �Ҽ����� ���⿡ �־���.

	//System�޽��� ����. �̰� Dlg�����ڿ��� �ϸ� �翬�� ����Ǿ����� �ʴٴ� ���忡���� ���. �׷��� ���⿡ �д�.
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

void CIPCDlg::OnBnClickedButtonSet(){//Set��ư�� ���� ����� �۵�
	UpdateData(TRUE);

	if (this->_sendReady) {//������ ������ �����
		this->_setResetButton.SetWindowText("Set");
		this->_sendReady = FALSE;

		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_srcAddressPanel.EnableWindow(TRUE);
		this->_broadcastPanel.EnableWindow(TRUE);
		this->_sendButton.EnableWindow(FALSE);
	} else {//���� ������ �����
		if (!this->_srcAddress || !this->_dstAddress) {
			MessageBox("�ּҰ� �ùٸ��� �ʽ��ϴ�.", "���", MB_OK | MB_ICONSTOP);
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


void CIPCDlg::OnBnClickedButtonSend(){//Send��ư�� ���� ����� �۵�
	UpdateData(TRUE);

	if (!this->_message.IsEmpty()) {
		//ȣ��Ʈ������ ä�� ó��
		CString msgFront;
		if (this->_isBroadcastMode) {
			msgFront.Format("[%d:BROADCAST] ", this->_srcAddress);
		} else {
			msgFront.Format("[%d:%d] ", this->_srcAddress, this->_dstAddress);
		}
		this->_chatList.AddString(msgFront + this->_message);

		//�۽� ó��
		this->_doesGetAck = FALSE;//�����ڰ� ��ε�ĳ��Ʈ�� �������͸޽����� ������ ACK �޽����� ��ε�ĳ��Ʈ�ϴµ�, �װ� ������ �̰� TRUE�� �ٲ۴�. �� ������ ���� �۽��ڰ� �ڽ��� ������ �����ڰ� �� �޾Ҵ��� �ȹ޾Ҵ��� üũ�Ѵ�.																

		int msgLength = this->_message.GetLength();
		unsigned char* ppayload = new unsigned char[msgLength + 1];
		memcpy(ppayload, (unsigned char*)(LPCTSTR)this->_message, msgLength);
		ppayload[msgLength] = '\0';

		this->_pApplicationLayer->SetDstAddress(this->_dstAddress);//���ø����̼����� ��� ������ ����� �ְ� ����, Send�� '������'�� �����Ѵ�.
		this->_pApplicationLayer->SetSrcAddress(this->_srcAddress);//��, ���⼭ ppayload�� ������ ������ �Է��� ���ڿ� �����͸��� �ǹ��Ѵ�.
		this->_pApplicationLayer->Send(ppayload, this->_message.GetLength());//���̴� �� ���� �Ǵ°��� Ȯ���ߴ�.

		//�ý����� ��� ���μ������� �޽����� ������. �� �޽����� ���� IPC���α׷��� �������������� ���̾���� ���� �ö����, ���������� �ùٸ� �����ڿ��Ը� �޽����� ǥ���ϰ� �ȴ�
		::SendMessageA(HWND_BROADCAST, RegSendMsg, 0, 0);

		//������ �� �Ϸ�Ǹ�, ȣ��Ʈ�� ä��â�� ����� �Ѵ�.
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
	AfxMessageBox("SystemMsg Send ����");
	this->_pPhysicsLayer->Receive();
	return 0;
}

LRESULT CIPCDlg::OnSystemMsgAck(WPARAM wParam, LPARAM lParam) {
	AfxMessageBox("ACK ����");
	if (!this->_doesGetAck) {
		this->_doesGetAck = TRUE;
	}
	return 0;
}
