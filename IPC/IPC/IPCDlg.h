
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

private://각 레이어 객체들 선언
	PhysicsLayer*		_physicsLayer;
	DatalinkLayer*		_datalinkLayer;
	NetworkLayer*		_networkLayer;
	TransportLayer*		_transportLayer;
	ApplicationLayer*	_applicationLayer;
	void				LinkLayers();

	BOOL				_sendReady;

public:
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnCheckBroadcast();

private:
	CString				_message;				// 유저가 입력한 채팅 메시지
	CButton				_broadcastPanel;
	BOOL				_isBroadcastMode;
	UINT				_srcAddress;
	UINT				_dstAddress;
	CString				_chatList;
	CEdit				_srcAddressPanel;
	CEdit				_dstAddressPanel;
	CButton				_setResetButton;
};
