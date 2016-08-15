
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
	: CDialogEx(IDD_IPC_DIALOG, pParent), LayerStructure("Application Dialog")
	, _message(_T(""))
	, _isBroadcastMode(FALSE)
	, _srcAddress(0)
	, _dstAddress(0)
	, _chatList(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	/*
		이 아래에 프로그램 초기화와 관련된 코드를 모두 입력한다. 각 레이어 객체들 생성, Upper/Under레이어 관계 설정까지.
	*/
	this->_isBroadcastMode = FALSE;
	this->_sendReady = FALSE;

	this->_physicsLayer = new PhysicsLayer();
	this->_datalinkLayer = new DatalinkLayer();
	this->_networkLayer = new NetworkLayer();
	this->_transportLayer = new TransportLayer();
	this->_applicationLayer = new ApplicationLayer();

	this->LinkLayers();
}
void CIPCDlg::LinkLayers() {
	this->_physicsLayer->SetUpperLayer(this->_datalinkLayer);
	this->_datalinkLayer->SetUnderLayer(this->_physicsLayer);

	this->_datalinkLayer->SetUpperLayer(this->_networkLayer);
	this->_networkLayer->SetUnderLayer(this->_datalinkLayer);

	this->_networkLayer->SetUpperLayer(this->_transportLayer);
	this->_transportLayer->SetUnderLayer(this->_networkLayer);

	this->_transportLayer->SetUpperLayer(this->_applicationLayer);
	this->_applicationLayer->SetUnderLayer(this->_transportLayer);
}

void CIPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, _message);
	DDX_Check(pDX, IDC_CHECK_BROAD, _isBroadcastMode);
	DDX_Text(pDX, IDC_EDIT1, _srcAddress);
	DDX_Text(pDX, IDC_EDIT2, _dstAddress);
	DDX_LBString(pDX, IDC_LIST1, _chatList);
	DDX_Control(pDX, IDC_EDIT1, _srcAddressPanel);
	DDX_Control(pDX, IDC_EDIT2, _dstAddressPanel);
	DDX_Control(pDX, IDC_BUTTON_SET, _setResetButton);
	DDX_Control(pDX, IDC_CHECK_BROAD, _broadcastPanel);
}

BEGIN_MESSAGE_MAP(CIPCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CIPCDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CIPCDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_BROAD, &CIPCDlg::OnCheckBroadcast)
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
		this->_setResetButton.SetWindowTextW(_T("Set"));
		this->_sendReady = FALSE;

		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_srcAddressPanel.EnableWindow(TRUE);
		this->_broadcastPanel.EnableWindow(TRUE);
	} else {//셋을 누르는 경우임
		if (!this->_srcAddress || !this->_dstAddress) {
			MessageBox(_T("주소가 올바르지 않습니다."), _T("경고"), MB_OK | MB_ICONSTOP);
			this->_srcAddress = 0;
			this->_srcAddressPanel.SetWindowTextW(_T("0"));
			this->_dstAddress = 0;
			this->_dstAddressPanel.SetWindowTextW(_T("0"));
			return;
		}

		this->_setResetButton.SetWindowTextW(_T("Reset"));
		this->_sendReady = TRUE;
		this->_applicationLayer->SetSrcAddress(this->_srcAddress);
		this->_applicationLayer->SetDstAddress(this->_dstAddress);

		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_srcAddressPanel.EnableWindow(FALSE);
		this->_broadcastPanel.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}


void CIPCDlg::OnBnClickedButtonSend(){//Send버튼을 누른 경우의 작동
	UpdateData(TRUE);

	if (!this->_message.IsEmpty()) {

	}

	UpdateData(FALSE);
}

void CIPCDlg::OnCheckBroadcast(){
	if (this->_isBroadcastMode) {
		UpdateData(TRUE);
		MessageBox(_T("일반 모드로 전환"), _T("알림"), MB_OK | MB_ICONSTOP);
		this->_dstAddress = 0;
		this->_dstAddressPanel.SetWindowTextW(_T("0"));
		this->_dstAddressPanel.EnableWindow(TRUE);
		this->_isBroadcastMode = FALSE;
	} else {
		UpdateData(TRUE);
		MessageBox(_T("브로드캐스트 모드로 전환"), _T("알림"), MB_OK | MB_ICONSTOP);
		this->_dstAddress = (unsigned int)0xff;
		this->_dstAddressPanel.SetWindowTextW(_T("255"));
		this->_dstAddressPanel.EnableWindow(FALSE);
		this->_isBroadcastMode = TRUE;
	}
}
