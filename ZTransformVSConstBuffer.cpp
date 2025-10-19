#include "ZD3D11.h"
#include "ZTransformVSConstBuffer.h"

namespace Bind
{
	ZTransformVSConstBuffer::ZTransformVSConstBuffer(ZGraphics& gfx, const ZRenderable& parent)
		:
		parent(parent)
	{
		if (!pVcbuf) // �� ���� �����Ѵ�.
		{
			pVcbuf = std::make_unique<Bind::VSConstBuffer<DirectX::XMMATRIX>>(gfx);
		}
	}

	void ZTransformVSConstBuffer::Bind(ZGraphics& gfx) noexcept
	{
		pVcbuf->Update(gfx,
			// ��ġ��� ��� ���� : D3D(�຤��*���) --> Shader(���*������) ��ġ��� �����
			DirectX::XMMatrixTranspose(
				parent.GetTransformXM() * gfx.GetProjection()
			)
		);
		pVcbuf->Bind(gfx);
	}

	std::unique_ptr<Bind::VSConstBuffer<DirectX::XMMATRIX>> ZTransformVSConstBuffer::pVcbuf;
}
