#pragma once

// 霸烙 扩档快 积己/包府

class ZApp : public ZApplication
{
private:
	ZGraphics* m_pGraphics;
	std::vector<std::unique_ptr<class SampleBox>> boxes;
	DWORD m_lastTime = 0;

public:
	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	ZApp(const TCHAR* pszCaption, DWORD XPos = 0, DWORD YPos = 0,
		DWORD Width = 640, DWORD Height = 480,
		DWORD ClientWidth = 640, DWORD ClientHeight = 480);
	virtual ~ZApp();

	DWORD GetWidth();
	DWORD GetHeight();

	BOOL Shutdown();
	BOOL Init();
	BOOL Frame();
};