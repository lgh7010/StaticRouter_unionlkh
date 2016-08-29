
// ChattingApplicationDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "LayerStructure.h"
#include "PhysicsLayer.h"
#include "DatalinkLayer.h"
#include "NetworkLayer.h"
#include "TransportLayer.h"
#include "ApplicationLayer.h"


// CChattingApplicationDlg dialog
class CChattingApplicationDlg : public CDialogEx, public LayerStructure
{
// Construction
public:
	CChattingApplicationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATTINGAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedCheckBroadcast();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonSend();

	BOOL				Receive(unsigned char* ppayload);
	void				Refresh();

	void				SetCurrentMessageSrcAddress(unsigned char* pAddress);
	void				SetCurrentMessageDstAddress(unsigned char* pAddress);
	void				SetCurrentMessageSize(int n);

private:
	CString _message;// 유저가 채팅창에 입력한 메시지
	BOOL _doesBroadcast;//브로드캐스트가 체크되었는지
	CButton _setButton;//설정 버튼
	CButton _sendButton;//보내기 버튼
	CListBox _chatList;// 채팅리스트
	CIPAddressCtrl _srcAddressPanel;//소스주소 IP패널
	CIPAddressCtrl _dstAddressPanel;//목적지주소 IP패널
	CButton _broadcastPanel;
	//이상은 MFC를 통해 추가된 변수들

	PhysicsLayer*		_pPhysicsLayer;
	DatalinkLayer*		_pDatalinkLayer;
	NetworkLayer*		_pNetworkLayer;
	TransportLayer*		_pTransportLayer;
	ApplicationLayer*	_pApplicationLayer;
	void				LinkLayers();

	BOOL				_sendReady;
	unsigned char		_srcAddress[4];
	unsigned char		_dstAddress[4];

	unsigned char		_currentMessageSrcAddress[4];
	unsigned char		_currentMessageDstAddress[4];
	int					_currentMessageSize;
};
