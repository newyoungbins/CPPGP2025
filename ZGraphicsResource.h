#pragma once
#pragma once

class ZGraphics;
class DxgiInfoManager;

class ZGraphicsResource
{
protected: // ZGraphics friend 立辟 力距 格利
    static ID3D11DeviceContext* GetContext(ZGraphics& gfx) noexcept;
    static ID3D11Device* GetDevice(ZGraphics& gfx) noexcept;
    static ID3D11RenderTargetView* GetTarget(ZGraphics& gfx) noexcept;
    static DxgiInfoManager& GetInfoManager(ZGraphics& gfx);
    static DWORD GetClientWidth(ZGraphics& gfx);
    static DWORD GetClientHeight(ZGraphics& gfx);
};