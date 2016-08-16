
// IPCDlg.h : header file
//

#pragma once
#include "LayerStructure.h"
#include "PhysicsLayer.h"
#include "DatalinkLayer.h"
#include "NetworkLayer.h"
#include "TransportLayer.h"
#include "ApplicationLayer.h"
#include "afxwin.h"


// CIPCDlg dialog
class CIPCDlg : public CDialogEx, public LayerStructure
{
// Construction
public:
	CIPCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPC_DIALOG };
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

private://�� ���̾� ��ü�� ����
	PhysicsLayer*		_pPhysicsLayer;
	DatalinkLayer*		_pDatalinkLayer;
	NetworkLayer*		_pNetworkLayer;
	TransportLayer*		_pTransportLayer;
	ApplicationLayer*	_pApplicationLayer;
	void				LinkLayers();

	BOOL				_sendReady;

public:
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnCheckBroadcast();

	BOOL				Receive(unsigned char* ppayload);

private:
	CString				_message;				// ������ �Է��� ä�� �޽���
	CButton				_broadcastPanel;
	BOOL				_isBroadcastMode;
	UINT				_srcAddress;
	UINT				_dstAddress;
	CEdit				_srcAddressPanel;
	CEdit				_dstAddressPanel;
	CButton				_setResetButton;
	CListBox			_chatList;
	CButton				_sendButton;

	LRESULT				OnSystemMsgSend(WPARAM wParam, LPARAM lParam);//�� �Լ��̸��� �ƹ����Գ� �ص�, '�޽�����'�� ��ϸ� ���ϸ� �ȴ�.
	LRESULT				OnSystemMsgAck(WPARAM wParam, LPARAM lParam);

	BOOL				_doesGetAck;
};
