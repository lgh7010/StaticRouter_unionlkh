
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
	CString _message;// ������ ä��â�� �Է��� �޽���
	BOOL _doesBroadcast;//��ε�ĳ��Ʈ�� üũ�Ǿ�����
	CButton _setButton;//���� ��ư
	CButton _sendButton;//������ ��ư
	CListBox _chatList;// ä�ø���Ʈ
	CIPAddressCtrl _srcAddressPanel;//�ҽ��ּ� IP�г�
	CIPAddressCtrl _dstAddressPanel;//�������ּ� IP�г�
	CButton _broadcastPanel;
	//�̻��� MFC�� ���� �߰��� ������

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
