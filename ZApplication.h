#pragma once

// 게임 윈도우

class ZApplication
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;

protected:
	TCHAR		m_Class[MAX_PATH];
	TCHAR		m_Caption[MAX_PATH];

	WNDCLASSEX	m_wcex;

	DWORD		m_Style;
	DWORD		m_XPos;
	DWORD		m_YPos;
	DWORD		m_Width;
	DWORD		m_Height;

public:
	ZApplication(DWORD XPos = 0, DWORD YPos = 0, DWORD Width = 640, DWORD Height = 480);
	
	HWND		GetHWnd();
	HINSTANCE	GetHInst();

	BOOL Run();
	BOOL Error(BOOL Fatal, const TCHAR* Text, ...);

	BOOL Move(long XPos, long YPos);
	BOOL Resize(long Width, long Height);

	BOOL ShowMouse(BOOL Show = TRUE);

	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	virtual BOOL Init() { return TRUE; }
	virtual BOOL Shutdown() { return TRUE; }
	virtual BOOL Frame() { return TRUE; }

};

static ZApplication* g_pApplication = NULL;
static LRESULT CALLBACK AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);