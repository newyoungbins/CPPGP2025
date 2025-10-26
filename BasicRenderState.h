#pragma once
#include "GameState.h"

class BasicRenderState : public GameState
{
public:
	BasicRenderState();
	~BasicRenderState();

	void Enter(ZGraphics& gfx) override;
	void Exit() override;
	void Update(float elapsedTime) override;
	void Render(ZGraphics& gfx) override;
	void OnKeyDown(WPARAM wParam) override;
	void OnkeyUp(WPARAM wParam) override;
	void OnMouseDown(int x, int y, int button) override;
	void OnMouseUp(int x, int y, int button) override;
	void OnMouseMove(int x, int y) override;

	void DrawTriangle(ZGraphics& gfx);
	void DrawIndexedTriangle(ZGraphics& gfx);
};