#include "ZD3D11.h"
#include "ZTexture.h"
#include "ZGraphics.h"
#include "WICTextureLoader.h" // From DirextX Tool Kit
#include "GraphicsThrowMacros.h"

namespace Bind
{
	ZTexture::ZTexture(ZGraphics& gfx, const TCHAR* filename)
	{
		pTextureSRV = nullptr;
		width = height = 0;
		this -> filename[0] = _T('\0'); // Initialize the string

		Load(gfx, filename);

	}

	ZTexture::~ZTexture()
	{
		Free();
	}

	const TCHAR* ZTexture::GetFileName()
	{
		return filename;
	}

	ID3D11ShaderResourceView* ZTexture::GetTextureSRV()
	{
		return pTextureSRV.Get();
	}

	BOOL ZTexture::Load(ZGraphics& gfx, const TCHAR* filename)
	{
		Free();

		if (GetDevice(gfx) == NULL)
			return FALSE;
		if (filename == NULL)
			return FALSE;

		INFOMAN(gfx);

		_tcscpy_s(this->filename, MAX_PATH, filename);

		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(
			GetDevice(gfx),
			filename,
			resource.GetAddressOf(),
			pTextureSRV.GetAddressOf()
		));

		// Get texture dimensions
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		hr = resource.As(&texture2D);
		if (SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			texture2D->GetDesc(&desc);
			width = desc.Width;
			height = desc.Height;
		}

		return TRUE;
	}
	void ZTexture::Bind(ZGraphics& gfx)noexcept
	{
		GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureSRV.GetAddressOf());
	}

	BOOL ZTexture::Free()
	{
		pTextureSRV.Reset();
		width = height = 0;
		filename[0] = _T('\0');

		return TRUE;
	}

	BOOL ZTexture::IsLoaded()
	{
		return (pTextureSRV != nullptr);
	}

	uint32_t ZTexture::GetWidth()
	{
		return width;
	}

	uint32_t ZTexture::GetHeight()
	{
		return height;
	}

	DXGI_FORMAT ZTexture::GetFormat()
	{
		if (!pTextureSRV)
			return DXGI_FORMAT_UNKNOWN;

		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		pTextureSRV->GetResource(resource.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D > texture2D;
		if (FAILED(resource.As(&texture2D)))
			return DXGI_FORMAT_UNKNOWN;

		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);


		return desc.Format;
	}
	BOOL ZTexture::Blit(DirectX::SpriteBatch* spriteBatch, const DirectX::SimpleMath::Vector2& position, const RECT* srcRect, DirectX::SimpleMath::Vector4 color, float rotation, const DirectX::SimpleMath::Vector2& origin, float scale)
	{
		if (pTextureSRV == nullptr || spriteBatch == nullptr)
			return FALSE;

		spriteBatch->Draw(pTextureSRV.Get(),
			position,
			srcRect,
			color,
			rotation,
			origin,
			scale);

		return TRUE;
	}
}
