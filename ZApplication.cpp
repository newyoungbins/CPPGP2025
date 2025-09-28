//---------------------------------------------------------------------------

#include "ZD3D11.h"

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

ZApplication::ZApplication(DWORD XPos, DWORD YPos, DWORD Width, DWORD Height)
{
	// Save instance handle
	g_pApplication = this;

	// Get the instance handle
	m_hInst = GetModuleHandle(NULL);

	// Set a default window class and caption
	_tcscpy_s(m_Class, _T("AppClass"));
	_tcscpy_s(m_Caption, _T(""));

	// Set default window style, position, width, height
	//m_Style  = WS_OVERLAPPEDWINDOW;
	m_Style = WS_CAPTION | WS_SYSMENU;
	m_XPos = XPos;
	m_YPos = YPos;
	m_Width = Width;
	m_Height = Height;

	// Set default WNDCLASSEX structure
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_CLASSDC;
	m_wcex.lpfnWndProc = AppWindowProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = m_hInst;
	m_wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wcex.hbrBackground = NULL;
	m_wcex.lpszMenuName = NULL;
	m_wcex.lpszClassName = m_Class;
	m_wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
}

//---------------------------------------------------------------------------

HWND ZApplication::GetHWnd()
{
	return m_hWnd;
}

//---------------------------------------------------------------------------

HINSTANCE ZApplication::GetHInst()
{
	return m_hInst;
}

//---------------------------------------------------------------------------

BOOL ZApplication::Run()
{
	MSG Msg;

	// Register window class
	if (!RegisterClassEx(&m_wcex))
		return FALSE;

	// Create the Main Window
	m_hWnd = CreateWindow(m_Class, m_Caption,
		m_Style,
		m_XPos, m_YPos,
		m_Width, m_Height,
		NULL, NULL, m_hInst, NULL);

	if (!m_hWnd) return FALSE;

	// Show and update the window
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);

	// Make sure client area is correct size
	// 클라이언트 영역 크기를 WxH로 만든다.(테투리가 없을때 유용함)
	//Resize(m_Width, m_Height);

	// Initialize COM
	CoInitialize(NULL);

	if (Init() == TRUE)
	{
		// Enter the message pump
		ZeroMemory(&Msg, sizeof(MSG));
		while (Msg.message != WM_QUIT)
		{
			// Handle Windows messages (if any)
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else {
				// Do per-frame processing, break on FALSE return value
				if (Frame() == FALSE)
					break;
			}
		}
	}

	Shutdown();

	// Shutdown COM
	CoUninitialize();

	// Unregister the window class
	UnregisterClass(m_Class, m_hInst);

	return TRUE;
}

//---------------------------------------------------------------------------

BOOL ZApplication::Error(BOOL Fatal, const TCHAR* Text, ...)
{
	TCHAR CaptionText[12] = _T("");
	TCHAR ErrorText[2048] = _T("");
	va_list valist;

	// Build the message box caption based on fatal flag
	if (Fatal == FALSE)
		_tcscpy_s(CaptionText, _T("Error"));
	else
		_tcscpy_s(CaptionText, _T("Fatal Error"));

	// Build variable text buffer
	va_start(valist, Text);
	_vstprintf_s(ErrorText, _countof(ErrorText), Text, valist);
	va_end(valist);

	// Display the message box
	MessageBox(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);

	// Post a quit message if error was fatal
	if (Fatal == TRUE)
		// PostQuitMessage Parameter 1
		// Specifies an application exsit code. 
		// This value is used as the wParam parameter of the WM_QUIT message.
		PostQuitMessage(0);

	return TRUE;
}

//---------------------------------------------------------------------------

BOOL ZApplication::Move(long XPos, long YPos)
{
	RECT ClientRect;

	GetClientRect(m_hWnd, &ClientRect);
	MoveWindow(m_hWnd, XPos, YPos, ClientRect.right, ClientRect.bottom, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
// 클라이언트 영역을 원하는 크기로 세팅

BOOL ZApplication::Resize(long Width, long Height)
{
	RECT WndRect, ClientRect;
	long WndWidth, WndHeight;

	GetWindowRect(m_hWnd, &WndRect);
	GetClientRect(m_hWnd, &ClientRect);

	WndWidth = (WndRect.right - (ClientRect.right - Width)) - WndRect.left;
	WndHeight = (WndRect.bottom - (ClientRect.bottom - Height)) - WndRect.top;

	MoveWindow(m_hWnd, WndRect.left, WndRect.top, WndWidth, WndHeight, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------

BOOL ZApplication::ShowMouse(BOOL Show)
{
	ShowCursor(Show);
	return TRUE;
}

//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// The message procedure - empty except for destroy message
// 게임 윈도우 메시지 창

LRESULT CALLBACK AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default: return g_pApplication->MsgProc(hWnd, uMsg, wParam, lParam);
	}
}

//---------------------------------------------------------------------------