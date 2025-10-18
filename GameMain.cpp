#include "GameGlobal.h"
#include <mmsystem.h> // timeGetTime()
#pragma comment(lib, "winmm.lib")

// �ܼ� â�� �����ϴ� �Լ�
void SetupConsole()
{
	AllocConsole(); // ���� ���μ����� ���ο� �ܼ� â�� �Ҵ��մϴ�.
	FILE* pConsole;
	// freopen_s: ǥ�� ��Ʈ���� �ٸ� �����̳� ��ġ�� �����ϰ� ���𷺼��ϴ� �Լ��Դϴ�.
	// &pConsole: [out] ���� ���� ���� ��Ʈ���� ����ų ������ ������ �ּ��Դϴ�.
	// "CONOUT$": [in] ���𷺼��� ��� ��ġ�Դϴ�. "CONOUT$"�� �����쿡�� '�ܼ� ��� ��ġ'�� �ǹ��ϴ� Ư�� ���� �̸��Դϴ�.
	// "w": [in] ����(write) ���� ���ϴ�.
	// stdout: [in] ���𷺼��� ǥ�� ��Ʈ���Դϴ�. ���⼭�� 'ǥ�� ���'�� �ǹ��մϴ�.
	// �� �Լ��� ����Ǹ�, ���� stdout(�׸��� �̸� ����ϴ� std::cout)�� ���� ��� ������ ���� �Ҵ�� �ܼ� â�� ��µ˴ϴ�.
	freopen_s(&pConsole, "CONOUT$", "w", stdout);
	// [�ٽ� ����: �� ���� ���� pConsole�� ������� ��������?]
	// 1. stdout �̶�? : ���α׷� ��ü���� ��ȿ�� '���� ���� ��Ʈ��'����, 'ǥ�� ���'�� ����� �����մϴ�.
	// 2. freopen_s�� ����: �� �Լ��� '����' stdout ��ü�� ���� ���¸� ���� �����Ͽ�, ����� ���������� �� �ܼ��� ���ϵ��� ����ϴ�.
	//    ���� �Լ� ���� ���� ���� pConsole�� �ӽ� ������ �޴� ������ ���̸�, �Լ� ���� �� ������� stdout�� ����� ������ ��� �����˴ϴ�.    
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �ܼ� ����
	SetupConsole();
	std::cout << "Console window is ready for logging!" << std::endl;

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

	// ���α׷� ���� �� �ܼ� ����
	FreeConsole();

	return result;
}

//------------------------------------------------------------------------------

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

DWORD ZApp::GetClientWidth()
{
	return m_ClientWidth;
}

DWORD ZApp::GetClientHeight()
{
	return m_ClientHeight;
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
	std::cout << "Window client size: " << m_ClientWidth << "x" << m_ClientHeight << std::endl;
	std::cout << (float)m_ClientHeight / (float)m_ClientWidth << std::endl;

	m_pGraphics = new ZGraphics(GetHWnd(), (float)m_ClientHeight / (float)m_ClientWidth, m_ClientWidth, m_ClientHeight);

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


	//m_pGraphics->DrawTestTriangle();
	//m_pGraphics->DrawIndexedTriangle();
	//m_pGraphics->DrawConstantBuffer((float)dValue);

	// ������ ������ ���� ���콺 ��ġ
	RECT rect;
	GetWindowRect(GetHWnd(), &rect);
	POINT pt;
	GetCursorPos(&pt);
	pt.x -= rect.left;
	pt.y -= rect.top;
	//std::cout << pt.x << " " << pt.y << std::endl;

	//m_pGraphics->DrawConstantBufferWithDXMath(
	//    (float)dValue,
	//    ((float)pt.x / ((float)m_ClientWidth / 2.0f)) - 1.0f,
	//    (-(float)pt.y / ((float)m_ClientHeight / 2.0f)) + 1.0f
	//);

	//m_pGraphics->DrawCube(
	//    (float)dValue,
	//    ((float)pt.x / ((float)m_ClientWidth / 2.0f)) - 1.0f,
	//    (-(float)pt.y / ((float)m_ClientHeight / 2.0f)) + 1.0f
	//);


	// ������ġ
	m_pGraphics->DrawCubeDepth(
		-(float)dValue,
		0.0f,
		0.0f
	);

	// ���콺�� y������ ���� ����
	m_pGraphics->DrawCubeDepth(
		(float)dValue,
		((float)pt.x / ((float)m_ClientWidth / 2.0f)) - 1.0f,
		(-(float)pt.y / ((float)m_ClientHeight / 2.0f)) + 1.0f
	);


	// �������� �ĸ� ���۸� ȭ�鿡 ǥ���մϴ�.
	m_pGraphics->EndFrame();
	return TRUE;
}