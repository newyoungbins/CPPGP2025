#pragma once

// D3D 11�� �ʱ�ȭ �� �ٽ� �������̽� ����

class ZGraphics
{
private:
	ID3D11Device* pDevice = nullptr;				// D3D11 ��ġ, ���ҽ� ���� �� ����
	IDXGISwapChain* pSwap = nullptr;				// ���� ü��, �ĸ� ���ۿ� ȭ�� ����� ��ü
	ID3D11DeviceContext* pContext = nullptr;		// D3D11 ��ġ ���ؽ�Ʈ, ������ ����� GPU�� ����
	ID3D11RenderTargetView* pTarget = nullptr;		// ���� Ÿ�� ��, ������ ����� ����Ǵ� ��

public:
	ZGraphics(HWND hWnd);

	// ���� �����ڿ� ���� �����ڸ� ������� �ʴ´�. (��ü ���� ����)
	// -- D3D ���ҽ� �����ϰ� ����
	ZGraphics(const ZGraphics&) = delete;
	ZGraphics& operator=(const ZGraphics&) = delete;

	~ZGraphics();
	
	// ���� �������� �������� ������ �ĸ� ���۸� ȭ�鿡 ǥ���Ѵ�.
	void EndFrame();
	// �ĸ� ���۸� ������ �������� �ʱ�ȭ
	// red (0,0f ~ 1.0f)
	// green (0,0f ~ 1.0f)
	// blue (0,0f ~ 1.0f)
	void ClearBuffer(float red, float green, float blue) noexcept;
};
