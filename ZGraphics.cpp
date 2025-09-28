#include "ZD3D11.h"

#pragma comment(lib,"d3d11.lib")

ZGraphics::ZGraphics(HWND hWnd)
{
    // ���� ü���� �����ϱ� ���� ����ü�Դϴ�.
    DXGI_SWAP_CHAIN_DESC sd = {};
    // �ĸ� ������ �ʺ�� ���̸� �����մϴ�. 0���� �����ϸ� ������ ũ�⿡ ���� �ڵ����� �����˴ϴ�.
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    // �ĸ� ������ �ȼ� ������ �����մϴ�. (B8G8R8A8_UNORM: 32��Ʈ, ä�δ� 8��Ʈ, BGRA ����)
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    // ȭ�� �ֻ���(Refresh Rate)�� �����մϴ�. 0���� �����ϸ� �ý��� �⺻���� ����մϴ�.
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    // ���÷��� �����ϸ� ��带 �����մϴ�.
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    // ��ĵ ���� ������ ������ �����մϴ�.
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // ��Ƽ���ø�(MSAA) ������ �մϴ�. Count=1, Quality=0�� ��Ƽ���ø��� ������� ������ �ǹ��մϴ�.
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    // �ĸ� ������ ��� �뵵�� �����մϴ�. ���� Ÿ������ ���˴ϴ�.
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    // �ĸ� ������ ������ �����մϴ�. �Ϲ������� 1��(���� ���۸�) �Ǵ� 2��(Ʈ���� ���۸�)�� ����մϴ�.
    sd.BufferCount = 1;
    // ������ ����� ����� �������� �ڵ��Դϴ�.
    sd.OutputWindow = hWnd;
    // â ���(TRUE) �Ǵ� ��ü ȭ�� ���(FALSE)�� �����մϴ�.
    sd.Windowed = TRUE;
    // �ĸ� ���۸� ȭ�鿡 ǥ���� ���� ó�� ����� �����մϴ�. (DISCARD: ������ ����)
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    // �߰����� �÷��׸� �����մϴ�.
    sd.Flags = 0;

    // D3D11 ��ġ(Device), ��ġ ���ؽ�Ʈ(Context), ���� ü��(Swap Chain)�� �����մϴ�.
    D3D11CreateDeviceAndSwapChain(
        nullptr,                        // pAdapter: �⺻ �����(�׷��� ī��)�� ����մϴ�.
        D3D_DRIVER_TYPE_HARDWARE,        // DriverType: �ϵ���� ������ ����մϴ�.
        nullptr,                        // Software: �ϵ���� ����̹��� ����ϹǷ� NULL�Դϴ�.
        0,                            // Flags: �߰����� ���� �÷��� (��: ����� ���)
        nullptr,                        // pFeatureLevels: ������ ��� ���� �迭, NULL�̸� �⺻���� ����մϴ�.
        0,                            // FeatureLevels: ��� ���� �迭�� ũ���Դϴ�.
        D3D11_SDK_VERSION,            // SDKVersion: �׻� D3D11_SDK_VERSION���� �����մϴ�.
        &sd,                        // pSwapChainDesc: ������ ������ ���� ü�� ���� ����ü�Դϴ�.
        &pSwap,                        // ppSwapChain: ������ ���� ü�� �������̽��� ���� �������Դϴ�.
        &pDevice,                    // ppDevice: ������ ��ġ �������̽��� ���� �������Դϴ�.
        nullptr,                        // pFeatureLevel: ������ ���õ� ��� ������ ���� �������Դϴ�.
        &pContext                    // ppImmediateContext: ������ ��ġ ���ؽ�Ʈ�� ���� �������Դϴ�.
    );

    // ���� ü�����κ��� �ĸ� ���ۿ� �����ϱ� ���� ���ҽ��� �����ɴϴ�.
    ID3D11Resource* pBackBuffer = nullptr;
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

    // �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����մϴ�.
    // ������ ������������ ���� ����� �� ���� Ÿ�� �信 �������ϴ�.
    pDevice->CreateRenderTargetView(
        pBackBuffer,        // pResource: ���� Ÿ������ ����� ���ҽ� (�ĸ� ����)
        nullptr,            // pDesc: ���� Ÿ�� �� ����, NULL�̸� ���ҽ��� �⺻ ������ �����ϴ�.
        &pTarget            // ppRTView: ������ ���� Ÿ�� �並 ���� �������Դϴ�.
    );

    // ���� Ÿ�� �並 ���������Ƿ� �ĸ� ���� ���ҽ��� �� �̻� �ʿ� �����Ƿ� �����մϴ�.
    pBackBuffer->Release();
}

ZGraphics::~ZGraphics()
{
    // ������ ������ �������� COM ��ü���� �����ϰ� �����մϴ�.
    // SAFE_RELEASE ��ũ�θ� ����ϸ� �� �����ϰ� ó���� �� �ֽ��ϴ�.
    if (pTarget != nullptr)
    {
        pTarget->Release();
    }
    if (pContext != nullptr)
    {
        pContext->Release();
    }
    if (pSwap != nullptr)
    {
        pSwap->Release();
    }
    if (pDevice != nullptr)
    {
        pDevice->Release();
    }
}

void ZGraphics::EndFrame()
{
    // �ĸ� ������ ������ ȭ�鿡 ǥ��(Present)�մϴ�.
    // ù ��° ����(SyncInterval): ���� ����ȭ �ɼ�. 1�� ���� ����ȭ�� �մϴ�.
    // �� ��° ����(Flags): �߰����� ���������̼� �ɼ�.
    pSwap->Present(1u, 0u);
}

void ZGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
    // ���� Ÿ�� �並 ������ �������� �ʱ�ȭ�մϴ�.
    const float color[] = { red,green,blue,1.0f }; // RGBA ����
    pContext->ClearRenderTargetView(pTarget, color);
}