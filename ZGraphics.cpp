#include "ZD3D11.h"
#include <sstream>
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h> // dx math

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib") // Shader Compiler

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

ZGraphics::ZGraphics(HWND hWnd, float winRatio, DWORD width, DWORD height)
{
    this->winRatio = winRatio;
    this->m_ClientWidth = width;
    this->m_ClientHeight = height;

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

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_FEATURE_LEVEL featureLevel;

    UINT creationFlags = 0;
#if defined(_DEBUG)
    // ����� ���忡���� D3D11 ����� ���̾ Ȱ��ȭ�մϴ�.
    // �̸� ���� API ��� ����, �޸� ���� �� �پ��� ����� ������ ���� �� �ֽ��ϴ�.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // D3D11 ��ġ(Device), ��ġ ���ؽ�Ʈ(Context), ���� ü��(Swap Chain)�� �����մϴ�.
    HRESULT hr;

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,						// pAdapter: �⺻ �����(�׷��� ī��)�� ����մϴ�.
        D3D_DRIVER_TYPE_HARDWARE,		// DriverType: �ϵ���� ������ ����մϴ�.
        nullptr,						// Software: �ϵ���� ����̹��� ����ϹǷ� NULL�Դϴ�.
        creationFlags,					// Flags: �߰����� ���� �÷��� (��: ����� ���)
        featureLevels,				// pFeatureLevels: ������ ��� ���� �迭�Դϴ�.
        numFeatureLevels,				// FeatureLevels: ��� ���� �迭�� ũ���Դϴ�.
        D3D11_SDK_VERSION,			// SDKVersion: �׻� D3D11_SDK_VERSION���� �����մϴ�.
        &sd,						// pSwapChainDesc: ������ ������ ���� ü�� ���� ����ü�Դϴ�.
        &pSwap,						// ppSwapChain: ������ ���� ü�� �������̽��� ���� �������Դϴ�.
        &pDevice,					// ppDevice: ������ ��ġ �������̽��� ���� �������Դϴ�.
        &featureLevel,				// pFeatureLevel: ������ ���õ� ��� ������ ���� �������Դϴ�.
        &pContext					// ppImmediateContext: ������ ��ġ ���ؽ�Ʈ�� ���� �������Դϴ�.
    ));

    if (SUCCEEDED(hr))
    {
        switch (featureLevel)
        {
        case D3D_FEATURE_LEVEL_11_1:
            std::cout << "D3D Feature Level: 11.1" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_11_0:
            std::cout << "D3D Feature Level: 11.0" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_10_1:
            std::cout << "D3D Feature Level: 10.1" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_10_0:
            std::cout << "D3D Feature Level: 10.0" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_9_3:
            std::cout << "D3D Feature Level: 9.3" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_9_2:
            std::cout << "D3D Feature Level: 9.2" << std::endl;
            break;
        case D3D_FEATURE_LEVEL_9_1:
            std::cout << "D3D Feature Level: 9.1" << std::endl;
            break;
        default:
            std::cout << "D3D Feature Level: Unknown or lower than 9.1" << std::endl;
            break;
        }
    }

    // [�ֿ� ����: GetBuffer�� ���� ü��]
    // ���� ü�����κ��� �ĸ� ����(Back Buffer)�� �����ϱ� ���� ���ҽ��� �����ɴϴ�.
    // - ù ��° ���� '0': ���� ü���� ù ��° ����, �� '�ĸ� ����'�� �ǹ��մϴ�. ��� �������� �� ���ۿ� �̷�����ϴ�.
    // - ���� ����(Front Buffer): ���� ȭ�鿡 ǥ�õǴ� ���۷�, GetBuffer()�� ���� ���� ������ �� �����ϴ�.
    // - Present() ȣ��: �ĸ� ������ ������ ���� ���۷� ���� ȭ�鿡 ǥ���ϴ� ������ �մϴ�. (�����δ� ������ ��ü(Flip) ����� �ַ� ����)
    // - Ʈ���� ���۸�: BufferCount�� 2 �̻����� �����ϸ�, �ε��� 1�� '�� ��° �ĸ� ����'�� �ǹ��մϴ�. �̴� V-Sync �� GPU�� ��� �ð��� �ٿ� ���� ���ϸ� ��ȭ�ϴ� �� ���˴ϴ�.
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));

    // �ĸ� ���ۿ� ���� RTV(Render Target View)�� �����մϴ�.
    // ������ ������������ ���� ����� �� ���� Ÿ�� �信 �������ϴ�.
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(
        pBackBuffer.Get(),		// pResource: ���� Ÿ������ ����� ���ҽ� (�ĸ� ����)
        nullptr,			// pDesc: ���� Ÿ�� �� ����, NULL�̸� ���ҽ��� �⺻ ������ �����ϴ�.
        &pTarget			// ppRTView: ������ ���� Ÿ�� �並 ���� �������Դϴ�.
    ));


    // depth ���� �ʱ�ȭ �� �� ����Ѵ�.
    // create depth stensil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // z������ ����� ���� �ٸ� ���� �������̵��Ѵ�.
    wrl::ComPtr<ID3D11DepthStencilState> pDSState;
    GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

    // bind depth state
    pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

    // create depth stensil texture
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = (UINT)m_ClientWidth;
    descDepth.Height = (UINT)m_ClientHeight;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

    // bind depth stensil view to OM
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
}

void ZGraphics::EndFrame()
{
    HRESULT hr;

#ifndef NDEBUG
    infoManager.Set();
#endif

    // �ĸ� ������ ������ ȭ�鿡 ǥ��(Present)�մϴ�.
    // ù ��° ����(SyncInterval): ���� ����ȭ �ɼ�. 1�� ���� ����ȭ�� �մϴ�.
    // �� ��° ����(Flags): �߰����� ���������̼� �ɼ�.
    if (FAILED(hr = pSwap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT(hr);
        }
    }
}

void ZGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
    // ���� Ÿ�� �並 ������ �������� �ʱ�ȭ�մϴ�.
    const float color[] = { red, green, blue, 1.0f }; // RGBA ����
    pContext->ClearRenderTargetView(pTarget.Get(), color);
    pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void ZGraphics::DrawTestTriangle()
{
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
        //struct
        //{
        //    //float r;
        //    //float g;
        //    //float b;
        //    uint8_t r;
        //    uint8_t g;
        //    uint8_t b;
        //    uint8_t a;
        //} color;
    };

    // NDC
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1)
    // create vertex buffer (1 2d triangle at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        //{ 0.0f, 0.5f, 255, 0, 0, 0 },
        //{ 0.5f, -0.5f, 0, 255, 0, 0 },
        //{ -0.5f, -0.5f, 0, 0, 255, 0 },

        // 2d float3
        //{ 0.0f, 0.5f, 1.0f, 0.0f, 0.0f },
        //{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        //{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },

        // 2d
        { 0.0f, 0.5f },
        { 0.5f, -0.5f },
        { -0.5f, -0.5f },
    };

    //Vertex vertices[] =
    //{
    //    { 0.0f, 0.5f, 255, 0, 0, 0 },
    //    { 0.5f, -0.5f, 0, 255, 0, 0 },
    //    { -0.5f, -0.5f, 0, 0, 255, 0 },

    //    { 0.0f, 0.5f, 255, 0, 0, 0 },
    //    { -0.5f, -0.5f, 0, 0, 255, 0 },
    //    { -0.3f, 0.3f, 0, 255, 0, 0 },

    //    { 0.0f, 0.5f, 255, 0, 0, 0 },
    //    { 0.3f, 0.3f, 0, 0, 255, 0 },
    //    { 0.5f, -0.5f, 0, 255, 0, 0 },

    //    { 0.0f, -0.8f, 255, 0, 0, 0 },
    //    { -0.5f, -0.5f, 0, 0, 255, 0 },
    //    { 0.5f, -0.5f, 0, 255, 0, 0 },
    //};

    //vertices[0].color.g = 255;

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_ClientWidth;
    vp.Height = (float)m_ClientHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
}

void ZGraphics::DrawIndexedTriangle()
{
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } color;
    };

    // NDC
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1)
    // create vertex buffer (1 2d triangle at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 255, 0, 0, 0 },
        { 0.5f, -0.5f, 0, 255, 0, 0 },
        { -0.5f, -0.5f, 0, 0, 255, 0 },
        { -0.3f, 0.3f, 0, 255, 0, 0 },
        { 0.3f, 0.3f, 0, 0, 255, 0 },
        { 0.0f, -0.8f, 255, 0, 0, 0 },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const uint16_t indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        2, 1, 5,
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(uint16_t);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    //vp.Width = 800 / 2;
    //vp.Height = 600 / 2;
    vp.Width = (float)m_ClientWidth / 2.0f;
    vp.Height = (float)m_ClientHeight / 2.0f;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 400;
    vp.TopLeftY = 300;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void ZGraphics::DrawConstantBuffer(float angle)
{
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } color;
    };

    // NDC
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1)
    // create vertex buffer (1 2d triangle at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 255, 0, 0, 0 },
        { 0.5f, -0.5f, 0, 255, 0, 0 },
        { -0.5f, -0.5f, 0, 0, 255, 0 },
        { -0.3f, 0.3f, 0, 255, 0, 0 },
        { 0.3f, 0.3f, 0, 0, 255, 0 },
        { 0.0f, -0.8f, 255, 0, 0, 0 },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const uint16_t indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        2, 1, 5,
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(uint16_t);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create constant buffer for transformation matrix
    struct ConstantBuffer
    {
        struct
        {
            float element[4][4];
        } transformation;
    };
    const ConstantBuffer cb =
    {
        {
            winRatio * std::cos(angle), std::sin(angle), 0.0f, 0.0f,
            winRatio * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    // GPU�� ��� ���۸� �����ϱ� ���� Transpose �ؾ��Ѵ�. (CPU : ����*��Ʈ����, GPU : ��Ʈ����*����)
    // ���⼭�� D3D ���¸� �״�� �ΰ� VertexShader���� row_major Ű���� ����Ѵ�.
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

    // bind constant buffer to vertex shader
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShaderConst.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_ClientWidth;
    vp.Height = (float)m_ClientHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void ZGraphics::DrawConstantBufferWithDXMath(float angle, float x, float y)
{
    // XMVECTOR �׽�Ʈ
    dx::XMVECTOR v = dx::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
    auto result = dx::XMVector3Transform(v, dx::XMMatrixScaling(1.5f, 1.0f, 1.0f));
    auto xx = dx::XMVectorGetX(result);
    auto yy = dx::XMVectorGetY(result);
    auto zz = dx::XMVectorGetZ(result);
    //std::cout << xx << " " << yy << " " << zz << std::endl;

    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } color;
    };

    // NDC
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1)
    // create vertex buffer (1 2d triangle at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 255, 0, 0, 0 },
        { 0.5f, -0.5f, 0, 255, 0, 0 },
        { -0.5f, -0.5f, 0, 0, 255, 0 },
        //{ -0.3f, 0.3f, 0, 255, 0, 0 },
        //{ 0.3f, 0.3f, 0, 0, 255, 0 },
        //{ 0.0f, -0.8f, 255, 0, 0, 0 },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const uint16_t indices[] =
    {
        0, 1, 2,
        //0, 2, 3,
        //0, 4, 1,
        //2, 1, 5,
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(uint16_t);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create constant buffer for transformation matrix
    struct ConstantBuffer
    {
        dx::XMMATRIX transform; // apply simd
    };
    const ConstantBuffer cb =
    {
        {
            // NDC(-1 ~ 1) : RST ��ȯ
            dx::XMMatrixTranspose(
                dx::XMMatrixRotationZ(angle) *
                dx::XMMatrixScaling(winRatio, 1.0f, 1.0f) *
                dx::XMMatrixTranslation(x, y, 0)
            )
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    // GPU�� ��� ���۸� �����ϱ� ���� Transpose �ؾ��Ѵ�. (CPU : ����*��Ʈ����, GPU : ��Ʈ����*����)
    // ���⼭�� D3D ���¸� �״�� �ΰ� VertexShader���� row_major Ű���� ����Ѵ�.
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

    // bind constant buffer to vertex shader
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShaderColMajor.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_ClientWidth;
    vp.Height = (float)m_ClientHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void ZGraphics::DrawCube(float angle, float x, float y)
{
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } color;
    };

    // NDC (Normalize Device Coordinate)
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1) Z(-1 ~ 1)
    // create vertex buffer (1 3d cube at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        { -1.0f, -1.0f, -1.0f   , 255, 0, 0 },
        { 1.0f, -1.0f, -1.0f    , 0, 255, 0 },
        { -1.0f, 1.0f, -1.0f    , 0, 0, 255 },
        { 1.0f, 1.0f, -1.0f     , 255, 255, 0 },
        { -1.0f, -1.0f, 1.0f    , 255, 0, 255 },
        { 1.0f, -1.0f, 1.0f     , 0, 255, 255 },
        { -1.0f, 1.0f, 1.0f     , 0, 0, 0 },
        { 1.0f, 1.0f, 1.0f      , 255, 255, 255 },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const uint16_t indices[] =
    {
        0, 2, 1,  2, 3, 1,
        1, 3, 5,  3, 7, 5,
        2, 6, 3,  3, 6, 7,
        4, 5, 7,  4, 7, 6,
        0, 4, 2,  2, 4, 6,
        0, 1, 4,  1, 5, 4,
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(uint16_t);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create constant buffer for transformation matrix
    struct ConstantBuffer
    {
        dx::XMMATRIX transform; // apply simd
    };
    const ConstantBuffer cb =
    {
        {
            // NDC(-1 ~ 1) : RST ��ȯ
            dx::XMMatrixTranspose(
                dx::XMMatrixRotationZ(angle) *
                dx::XMMatrixRotationY(angle) *
                // dx::XMMatrixScaling(winRatio, 1.0f, 1.0f) * // no need ratio adjustment
                dx::XMMatrixTranslation(x, y, 4) *
                dx::XMMatrixPerspectiveLH(1.0f, winRatio, 0.5f, 10.0f)
            )
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    // GPU�� ��� ���۸� �����ϱ� ���� Transpose �ؾ��Ѵ�. (CPU : ����*��Ʈ����, GPU : ��Ʈ����*����)
    // ���⼭�� D3D ���¸� �״�� �ΰ� VertexShader���� row_major Ű���� ����Ѵ�.
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

    // bind constant buffer to vertex shader
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShaderCube.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_ClientWidth;
    vp.Height = (float)m_ClientHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void ZGraphics::DrawCubeDepth(float angle, float x, float z)
{
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
    };

    // NDC (Normalize Device Coordinate)
    // ȭ�� ����� (0,0), X(-1 ~ 1), Y(-1 ~ 1) Z(-1 ~ 1)
    // create vertex buffer (1 3d cube at center of screen)
    // �޼���ǥ�� �ո� : CW (Clock Wise)
    Vertex vertices[] =
    {
        { -1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f  },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const uint16_t indices[] =
    {
        0, 2, 1,  2, 3, 1,
        1, 3, 5,  3, 7, 5,
        2, 6, 3,  3, 6, 7,
        4, 5, 7,  4, 7, 6,
        0, 4, 2,  2, 4, 6,
        0, 1, 4,  1, 5, 4,
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(uint16_t);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create constant buffer for transformation matrix
    struct ConstantBuffer
    {
        dx::XMMATRIX transform; // apply simd
    };
    const ConstantBuffer cb =
    {
        {
            // NDC(-1 ~ 1) : RST ��ȯ
            dx::XMMatrixTranspose(
                dx::XMMatrixRotationZ(angle) *
                dx::XMMatrixRotationY(angle) *
                // dx::XMMatrixScaling(winRatio, 1.0f, 1.0f) * // no need ratio adjustment
                dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
                dx::XMMatrixPerspectiveLH(1.0f, winRatio, 0.5f, 10.0f)
            )
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    // GPU�� ��� ���۸� �����ϱ� ���� Transpose �ؾ��Ѵ�. (CPU : ����*��Ʈ����, GPU : ��Ʈ����*����)
    // ���⼭�� D3D ���¸� �״�� �ΰ� VertexShader���� row_major Ű���� ����Ѵ�.
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

    // bind constant buffer to vertex shader
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    // �� �ﰢ���� ������ �÷��� �ٸ���. ���� ���� �������� ���� constant buffer �߰�
    struct ConstantBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };
    const ConstantBuffer2 cb2 =
    {
        {
            {1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f},
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
    D3D11_BUFFER_DESC cbd2;
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd2.Usage = D3D11_USAGE_DEFAULT;
    cbd2.CPUAccessFlags = 0u;
    cbd2.MiscFlags = 0u;
    cbd2.ByteWidth = sizeof(cb2);
    cbd2.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd2 = {};
    csd2.pSysMem = &cb2;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

    // bind constant buffer to pixel shader
    pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShaderCubeDepth.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShaderCubeDepth.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // VertexShader�� ����Ʈ�ڵ�(pBlob)�� �ʿ��ϱ� ������ PixelShader�� ���� �����Ѵ�.
    // input (vertex) layout (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    //pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr); // ZGraphics �����ڿ��� DepthStensil �ʱ�ȭ �ߴ�. ���⼭ �ּ�

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // CW ���� ����
    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); // CW ���� ����

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_ClientWidth;
    vp.Height = (float)m_ClientHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}



// Graphics exception stuff
ZGraphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hr(hr)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* ZGraphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* ZGraphics::HrException::GetType() const noexcept
{
    return "Chili Graphics Exception";
}

HRESULT ZGraphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string ZGraphics::HrException::GetErrorString() const noexcept
{
    // DXGetErrorString�� TCHAR*�� ��ȯ�ϹǷ�, std::string���� ��ȯ�ؾ� �մϴ�.
    const TCHAR* errorString = DXGetErrorString(hr);
#ifdef _UNICODE
    // �����ڵ� ȯ�濡���� wchar_t*�� char*�� ��ȯ�մϴ�.
    std::wstring w_str(errorString);
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w_str[0], (int)w_str.size(), NULL, 0, NULL, NULL);
    std::string str_to(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &w_str[0], (int)w_str.size(), &str_to[0], size_needed, NULL, NULL);
    return str_to;
#else
    // ��Ƽ����Ʈ ȯ�濡���� �ٷ� ��ȯ �����մϴ�.
    return std::string(errorString);
#endif
}

std::string ZGraphics::HrException::GetErrorDescription() const noexcept
{
    TCHAR buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf) / sizeof(TCHAR));
#ifdef _UNICODE
    // �����ڵ� ȯ�濡���� wchar_t*�� char*�� ��ȯ�մϴ�.
    std::wstring w_str(buf);
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w_str[0], (int)w_str.size(), NULL, 0, NULL, NULL);
    std::string str_to(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &w_str[0], (int)w_str.size(), &str_to[0], size_needed, NULL, NULL);
    return str_to;
#else
    // ��Ƽ����Ʈ ȯ�濡���� �ٷ� ��ȯ �����մϴ�.
    return std::string(buf);
#endif
}

std::string ZGraphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}


const char* ZGraphics::DeviceRemovedException::GetType() const noexcept
{
    return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
ZGraphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}


const char* ZGraphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* ZGraphics::InfoException::GetType() const noexcept
{
    return "Chili Graphics Info Exception";
}

std::string ZGraphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}
