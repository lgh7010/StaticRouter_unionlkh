
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



void CChattingApplicationDlg::OnBnClickedButtonSend(){
	UpdateData(TRUE);

	if (!this->_message.IsEmpty()) {
		//ȣ��Ʈ������ ä�� ó��
		CString msgFront;
		if (this->_doesBroadcast) {
			msgFront.Format("[%d.%d.%d.%d : BROADCAST] ", this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);
		}
		else {
			msgFront.Format("[%d.%d.%d.%d : %d.%d.%d.%d] ", this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3], this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);
		}
		this->_chatList.AddString(msgFront + this->_message);

		//�۽� ó��
		//this->_doesGetAck = FALSE;//�����ڰ� ��ε�ĳ��Ʈ�� �������͸޽����� ������ ACK �޽����� ��ε�ĳ��Ʈ�ϴµ�, �װ� ������ �̰� TRUE�� �ٲ۴�. �� ������ ���� �۽��ڰ� �ڽ��� ������ �����ڰ� �� �޾Ҵ��� �ȹ޾Ҵ��� üũ�Ѵ�.																
		SetTimer(1, 2000, NULL);

		int msgLength = this->_message.GetLength();
		unsigned char* ppayload = new unsigned char[msgLength + 1];
		memcpy(ppayload, (unsigned char*)(LPCTSTR)this->_message, msgLength);
		ppayload[msgLength] = '\0';

		this->_pApplicationLayer->SetDstAddress(this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);//���ø����̼����� ��� ������ ����� �ְ� ����, Send�� '������'�� �����Ѵ�.
		this->_pApplicationLayer->SetSrcAddress(this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);//��, ���⼭ ppayload�� ������ ������ �Է��� ���ڿ� �����͸��� �ǹ��Ѵ�.
		this->_pApplicationLayer->SetSize(msgLength);
		this->_pApplicationLayer->Send(ppayload, msgLength);//���̴� �� ���� �Ǵ°��� Ȯ���ߴ�.

		//������ �� �Ϸ�Ǹ�, ȣ��Ʈ�� ä��â�� ����� �Ѵ�.
		this->_message = "";
	}

	UpdateData(FALSE);
}

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
		this->_dstAddressPanel.SetWindowText("255,255,255,255");
		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_doesBroadcast = TRUE;
	}
}

void CChattingApplicationDlg::OnBnClickedButtonSet(){ //�ҽ�, ������ �ּҰ��� _srcAddress, _dstAddress������ ����
	UpdateData(TRUE);

	if (this->_sendReady) {//������ ������ �����
		this->_setButton.SetWindowText("Set");
		this->_sendReady = FALSE;

		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_srcAddressPanel.EnableWindow(TRUE);
		this->_broadcastPanel.EnableWindow(TRUE);
		this->_sendButton.EnableWindow(FALSE);
	}
	else {//���� ������ �����
		unsigned char t1, t2, t3, t4, p1, p2, p3, p4;
		this->_srcAddressPanel.GetAddress(t1, t2, t3, t4);
		this->_dstAddressPanel.GetAddress(p1, p2, p3, p4);
		if (!t1|| !p1) {
			MessageBox("�ּҰ� �ùٸ��� �ʽ��ϴ�.", "���", MB_OK | MB_ICONSTOP);
			return;
		}

		this->_setButton.SetWindowText("Reset");
		this->_sendReady = TRUE;
		this->_srcAddressPanel.GetAddress(this->_srcAddress[0], this->_srcAddress[1], this->_srcAddress[2], this->_srcAddress[3]);
		this->_dstAddressPanel.GetAddress(this->_dstAddress[0], this->_dstAddress[1], this->_dstAddress[2], this->_dstAddress[3]);

		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_srcAddressPanel.EnableWindow(FALSE);
		this->_broadcastPanel.EnableWindow(FALSE);
		this->_sendButton.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}












void CIPCDlg::SetCurrentMessageSrcAddress(UINT n) {
	this->_currentMessageSrcAddress = n;
}
void CIPCDlg::SetCurrentMessageDstAddress(UINT n) {
	this->_currentMessageDstAddress = n;
}
void CIPCDlg::SetCurrentMessageSize(int n) {
	this->_currentMessageSize = n;
}