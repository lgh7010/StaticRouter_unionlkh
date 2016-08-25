
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#define ETHERNET_FRAME_MAX_SIZE			1514
#define ETHERNET_FRAME_HEADER_SIZE		14
#define ETHERNET_FRAME_DATA_SIZE		( ETHERNET_FRAME_MAX_SIZE - ETHERNET_FRAME_HEADER_SIZE )

#define IP_PACKET_DATA_SIZE				1480
#define IP_PACKET_HEADER_SIZE			20

#define TP_SEGMENT_OP_SIZE				4//일단 4로 한다. 이건 사실 그때그때 달라지는 값이다.
#define TP_SEGMENT_HEADER_SIZE			( 20 + TP_SEGMENT_OP_SIZE )
#define TP_SEGMENT_DATA_SIZE			( IP_PACKET_DATA_SIZE - TP_SEGMENT_HEADER_SIZE )

#define APP_DATA_SIZE					( TP_SEGMENT_DATA_SIZE - APP_HEADER_SIZE )
#define APP_HEADER_SIZE					12//UINT 2개, int 1개이므로 총 12바이트



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


