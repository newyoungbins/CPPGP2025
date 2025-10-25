#pragma once

#include "SpriteBatch.h"
#include "SimpleMath.h"
#include "ZBindable.h"

namespace Bind
{
	class ZTexture : public ZBindable
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>pTextureSRV;
		uint32_t width, height;
		TCHAR filename[MAX_PATH];

	public:
		ZTexture(ZGraphics& gfx, const TCHAR* filename);
		~ZTexture();

		const TCHAR* GetFileName();
		ID3D11ShaderResourceView* GetTextureSRV();
		BOOL Load(ZGraphics& gfx, const TCHAR* filename);
		void Bind(ZGraphics& gfx) noexcept;
		BOOL Free();
		
		BOOL IsLoaded();
		uint32_t GetWidth();
		uint32_t GetHeight();
		DXGI_FORMAT GetFormat();

		BOOL Blit(DirectX::SpriteBatch* spriteBatch,
			const DirectX::SimpleMath::Vector2& position,
			const RECT* srcRect = nullptr,
			DirectX::SimpleMath::Vector4 color = DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f),
			float rotation = 0.f,
			const DirectX::SimpleMath::Vector2& origin = DirectX::SimpleMath::Vector2(0.f, 0.f),
			float scale = 1.f);
	};
}