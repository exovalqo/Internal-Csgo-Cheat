#pragma once
#include "d3d9.h"

namespace imguihelper
{
    enum TAB
    {
        VISUALS = 0,
        AIMBOT = 1
    };
    void InitWndProcHook();
    void InitImGui(IDirect3DDevice9* pDevice);
    void UnhookWndProc();
    void RenderTabs(TAB& tab);
    void RenderVisuals();
    void RenderMenu();
    void RenderAimbot();
}