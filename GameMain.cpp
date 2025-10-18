#include "GameGlobal.h"
#include <mmsystem.h> // timeGetTime()
#pragma comment(lib, "winmm.lib")

void SetupConsole()
{
	AllocConsole();
	FILE* pConsole;
	freopen_s(&pConsole, "CONOUT$", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetupConsole();

	// Ŭ���̾�Ʈ ���� ũ��
	const int clientWidth = 1024;
	const int clientHeight = 600;

	RECT windowRect = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	std::cout << "windowRect: " << windowWidth << " " << windowHeight << std::endl;

	// ȭ�� �߾� ��ġ ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

	ZApp Game(_T("My D3D Test"), x, y, windowWidth, windowHeight, clientWidth, clientHeight);
	BOOL result = Game.Run();

	FreeConsole();

	return result;
}

ZApp::ZApp(const TCHAR* pszCaption,
	DWORD XPos, DWORD YPos, DWORD Width, DWORD Height, DWORD ClientWidth, DWORD ClientHeight)
	: ZApplication(XPos, YPos, Width, Height, ClientWidth, ClientHeight)
{
	m_pGraphics = nullptr;
	_tcscpy_s(m_Caption, pszCaption);
}

ZApp::~ZApp()
{
	SAFE_DELETE(m_pGraphics);
}

DWORD ZApp::GetWidth()
{
	return m_Width;
}

DWORD ZApp::GetHeight()
{
	return m_Height;
}

LRESULT CALLBACK ZApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ZApplication::MsgProc(hWnd, uMsg, wParam, lParam);
}

BOOL ZApp::Shutdown()
{
	return TRUE;
}

BOOL ZApp::Init()
{
	m_pGraphics = new ZGraphics(GetHWnd(), 
		(float)m_ClientHeight / (float)m_ClientWidth, 
		m_ClientWidth, m_ClientHeight);

	ShowMouse(TRUE);

	return TRUE;
}

BOOL ZApp::Frame()
{
	// timeGetTime() �Լ��� �ý����� ���۵� �� ����� �ð��� �и���(ms) ������ ��ȯ�մϴ�.
	DWORD timeValue = timeGetTime();
	// ��� �ð��� �� ������ ��ȯ�մϴ�. (1000ms = 1s)
	double dValue = timeValue / 1000.0;
	// sin �Լ��� ����Ͽ� �ð��� �帧�� ���� 0.0 ~ 1.0 ���̸� �ε巴�� �պ��ϴ� ���� ����մϴ�.
	// sin(dValue)�� ����� -1.0 ~ 1.0 �̹Ƿ�, �̸� 0.0 ~ 1.0 ������ ����ȭ�մϴ�.
	const float c = (float)sin(dValue) / 2.0f + 0.5f;
	// ���� 'c' ���� ����Ͽ� ȭ�� ������ �������� �����մϴ�.
	// Red�� Green ä���� 'c' ���� ���� ���ϹǷ�, ������ �Ķ���(0,0,1)�� û�ϻ�(1,1,1) ���̸� ������ �˴ϴ�.
	m_pGraphics->ClearBuffer(c, c, 1.0f);

	//m_pGraphics->DrawTriangle();
	//m_pGraphics->DrawIndexedTriangle();
	//m_pGraphics->DrawConstTriangle((float)dValue);


	// ������ ������ ���� ���콺 ��ġ
	RECT rect;
	GetWindowRect(GetHWnd(), &rect);
	POINT pt;
	GetCursorPos(&pt);
	pt.x -= rect.left;
	pt.y -= rect.top;
	std::cout << pt.x << " " << pt.y << std::endl;
	m_pGraphics->DrawDepthCube((float)dValue, pt.x, pt.y);

	// �������� �ĸ� ���۸� ȭ�鿡 ǥ���մϴ�.
	m_pGraphics->EndFrame();
	return TRUE;
}