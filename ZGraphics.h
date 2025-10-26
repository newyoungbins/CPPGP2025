#pragma once
#include "ChiliException.h"
#include <wrl.h> // ComPtr
#include "DxgiInfoManager.h"
#include "ZConditionalNoExcept.h"
#include "SampleBox.h"
#include "SpriteBatch.h"
#include "ZTexture.h"



// D3D 11�� �ʱ�ȭ �� �ٽ� �������̽� ����

class ZGraphics
{
	friend class ZGraphicsResource;

private:
	DirectX::XMMATRIX projection;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;			// D3D11 ��ġ, ���ҽ� ���� �� ����
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;			// ���� ü��, �ĸ� ���ۿ� ȭ�� ����� ��ü
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;	// D3D11 ��ġ ���ؽ�Ʈ, ������ ����� GPU�� ����
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;	// ���� Ÿ�� ��, ������ ����� ����Ǵ� ��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;   // ���� ���� ����

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif

	float winRatio;
	DWORD m_ClientWidth;
	DWORD m_ClientHeight;

	std::unique_ptr<DirectX::SpriteBatch> pSpriteBatch;
	std::unique_ptr<Bind::ZTexture> pTexture;

public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

public:
	ZGraphics(HWND hWnd, float winRatio, DWORD width, DWORD height);

	// ���� �����ڿ� ���� �����ڸ� ������� �ʴ´�. (��ü ���� ����)
	// -- D3D ���ҽ� �����ϰ� ����
	ZGraphics(const ZGraphics&) = delete;
	ZGraphics& operator=(const ZGraphics&) = delete;

	~ZGraphics() = default;

	// ���� �������� �������� ������ �ĸ� ���۸� ȭ�鿡 ǥ���Ѵ�.
	void EndFrame();
	// �ĸ� ���۸� ������ �������� �ʱ�ȭ
	// red (0.0f ~ 1.0f)
	// green (0.0f ~ 1.0f)
	// blue (0.0f ~ 1.0f)
	void ClearBuffer(float red, float green, float blue) noexcept;
	void SetViewport() noexcept;
	void RenderIndexed(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	// Basic
	void DrawTriangle();
	void DrawIndexedTriangle();
	void DrawConstTriangle(float angle);
	void DrawDepthCube(float angle, float x, float y); // using face color

	void DrawTexture();
};
