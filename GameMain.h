#pragma once

// 霸烙 扩档快 积己/包府

class ZApp : public ZApplication
{
private:
	ZGraphics* m_pGraphics;

public:
	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

	ZApp(const TCHAR* pszCaption, DWORD XPos = 0, DWORD YPos = 0,
			DWORD Width = 640, DWORD Height = 480, 
			DWORD ClientWidth = 640, DWORD ClientHeight = 480);
	~ZApp();

	DWORD GetWidth();
	DWORD GetHeight();

	BOOL Shutdown();
	BOOL Init();
	BOOL Frame();
};