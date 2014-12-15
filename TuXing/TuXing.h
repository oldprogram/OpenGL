#if !defined(_TUXING_H_INCLUDED)
#define _TUXING_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "MainWnd.h"
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CApp
// Application class

class CApp
{
protected:
	HINSTANCE m_hInst;
	CMainWnd* m_pMainWnd;

public:
	CApp();
	~CApp();

	int Run();
	void OnIdle();
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine);	
	void ExitInstance();
	BOOL OnCommand(int nCmdID, int nEvent);
	LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void OnPaint(HDC hDC);
	void OnDestroy();
	void OnCreate();
	void OnSize(int cx, int cy);
};

#endif // !defined(TUXING_H_INCLUDED)
