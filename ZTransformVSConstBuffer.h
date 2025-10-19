#pragma once
#include "ZConstBuffer.h"
#include "ZRenderable.h"
#include <DirectXMath.h>

namespace Bind
{
	class ZTransformVSConstBuffer : public ZBindable
	{
	private:
		// VSConstBuffer�� �� ������ ���� �ٽ� ����ϱ� ������ ��Ȱ���Ѵ�.
		static std::unique_ptr<Bind::VSConstBuffer<DirectX::XMMATRIX>> pVcbuf;
		const ZRenderable& parent;

	public:
		ZTransformVSConstBuffer(ZGraphics& gfx, const ZRenderable& parent);
		void Bind(ZGraphics& gfx) noexcept override;
	};
}