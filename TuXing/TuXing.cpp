// TuXing.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TuXing.h"

/////////////////////////////////////////////////////////////////////////////
// Global CApp object

CApp gApp;

/////////////////////////////////////////////////////////////////////////////
// Global functions
/////////////////////////////////////////////////////////////////////////////

//
// WinMain()
// The applications entry point.
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!gApp.InitInstance(hInstance, nCmdShow, lpCmdLine)) 
		return FALSE;

	int nRet = gApp.Run();

	gApp.ExitInstance();

	return nRet;
}

//
// WndProc()
// Window procedure for the main window. Simply calls CApp::WindowProc.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return gApp.WindowProc(hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CApp construction/destruction

CApp::CApp():
	m_hInst(0),
	m_pMainWnd(NULL)	
{
}

CApp::~CApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// CApp member functions
/////////////////////////////////////////////////////////////////////////////

//
// InitInstance()
// Called when the application starts. Creates and shows the main window.
//
BOOL CApp::InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine)
{
	const char szWindowClass[] = "TuXingWindowClass";
	const char szWindowTitle[] = "TuXing";

	m_hInst = hInstance;

	// Register main window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUXING);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	if(!RegisterClassEx(&wcex))
		return FALSE;

	// Create main window
	m_pMainWnd = new CMainWnd;
	m_pMainWnd->m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, szWindowTitle, 
		WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION |WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	if (!m_pMainWnd->m_hWnd)
	{
		delete m_pMainWnd;
		return FALSE;
	}
	OnCreate();

	// Show the main window
	ShowWindow(m_pMainWnd->m_hWnd, nCmdShow);
	UpdateWindow(m_pMainWnd->m_hWnd);

	return TRUE;
}

//
// ExitInstance()
// Called when the application exits.
//
void CApp::ExitInstance()
{
}

//
// OnCommand()
// Handles WM_COMMAND messages
//
BOOL CApp::OnCommand(int nCmdID, int nEvent)
{
	switch (nCmdID)
	{				
		case IDM_EXIT:
			SendMessage(m_pMainWnd->m_hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

//
// Run()
// Starts the message pump
//
int CApp::Run()
{  
	MSG msg;
	while(TRUE)
  {
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      if(!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
    }
    else
      OnIdle();
	}

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return msg.wParam;
}

//
// WindowProc()
// Handles messages sent to the main window
//
LRESULT CApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (message) 
	{
		case WM_CLOSE:
			DestroyWindow(m_pMainWnd->m_hWnd);
			m_pMainWnd->m_hWnd = 0;
			break;
		case WM_COMMAND:
			if(!OnCommand(LOWORD(wParam), HIWORD(wParam)))
				return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			OnPaint(hDC);
			EndPaint(hWnd, &ps);
			break;
		case WM_CREATE:
			break;
		case WM_DESTROY:
			OnDestroy();
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			OnSize(LOWORD(lParam), HIWORD(lParam));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//
// OnIdle()
// Called when there are no messages in the message queue. 
// Sets current OpenGL RC and notifies the main window class that another frame 
// is to be drawn. 
//
void CApp::OnIdle()
{
	BOOL bRedraw = TRUE;
	m_pMainWnd->Tick(bRedraw);
	if(bRedraw)
	{
		HDC hDC = GetDC(m_pMainWnd->m_hWnd);
		if(hDC)
		{
			wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
			m_pMainWnd->DrawScene(); 
			SwapBuffers(hDC);
			wglMakeCurrent(0, 0);
		}
		ReleaseDC(m_pMainWnd->m_hWnd, hDC);
	}
}

//
// OnCreate()
// Called when the main window has been created. Initializes OpenGL.
//
void CApp::OnCreate()
{
	// Define pixel format
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;	
	memset(&pfd, NULL, sizeof(pfd));    
  pfd.nSize      = sizeof(pfd);
  pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
	
	// Set pixel format
	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
  nPixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, nPixelFormat, &pfd);
	
	// Create RC
	m_pMainWnd->m_hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	  
	m_pMainWnd->InitScene();
  wglMakeCurrent(0, 0);

	// Clean up
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);

	// Size viewport
	RECT rc;
	GetClientRect(m_pMainWnd->m_hWnd, &rc);
	OnSize(rc.right-rc.left, rc.bottom-rc.top);
}

//
// OnDestroy()
// Called when the main window is destroyed. Shuts down OpenGL
//
void CApp::OnDestroy()
{
	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	m_pMainWnd->KillScene();
  wglMakeCurrent(0, 0);
	wglDeleteContext(m_pMainWnd->m_hRC);
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);
}

//
// OnSize()
// Called when the main window is resized. Resizes the OpenGL
// viewport.
//
void CApp::OnSize(int cx, int cy)
{
	if(cx==0 || cy ==0 || m_pMainWnd->m_hWnd==NULL)
		return;

	GLfloat fFovy  = 30.0f; // Field-of-view
  GLfloat fZNear = 1.0f;  // Near clipping plane
  GLfloat fZFar = 10.0f;  // Far clipping plane

	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	
	// Calculate viewport aspect
	RECT rv;
	GetClientRect(m_pMainWnd->m_hWnd, &rv);
	GLfloat fAspect = (GLfloat)(rv.right-rv.left) / (GLfloat)(rv.bottom-rv.top);

	// Define viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fFovy, fAspect, fZNear, fZFar);
	glViewport(rv.left, rv.top, rv.right-rv.left, rv.bottom-rv.top);
	glMatrixMode(GL_MODELVIEW);

	wglMakeCurrent(0, 0);
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);
}

//
// OnPaint()
// Handles WM_PAINT messages. Redraws the OpenGL scene.
//
void CApp::OnPaint(HDC hDC)
{
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	m_pMainWnd->DrawScene();
	SwapBuffers(hDC);
	wglMakeCurrent(0, 0);
}
