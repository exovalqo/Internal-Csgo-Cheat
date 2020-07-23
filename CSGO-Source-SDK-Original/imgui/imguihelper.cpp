#include "imguihelper.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "d3d9.h"
#include "Options.h"



IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace imguihelper
{
    //Function Declarations
    LRESULT WINAPI WndProcHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    WNDPROC oWndProc = nullptr;
    TAB tab = VISUALS;

    LRESULT WINAPI WndProcHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (Options::bShowMenu)
        {
            ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
            //return TRUE;
        }
        return CallWindowProc(oWndProc, hWnd, message, wParam, lParam);
    }
    void InitImGui(IDirect3DDevice9* pDevice)
    {
        /*D3DDEVICE_CREATION_PARAMETERS CP;
        pDevice->GetCreationParameters(&CP);
        window = CP.hFocusWindow;*/

        HWND window = FindWindowA(NULL, "Counter-Strike: Global Offensive");
        ImGui::CreateContext();

        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.IniFilename = NULL;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //io.Fonts->AddFontDefault();

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX9_Init(pDevice);
        InitWndProcHook();
        return;
    }
    void InitWndProcHook()
    {
        HWND window = FindWindowA(NULL, "Counter-Strike: Global Offensive");
        oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProcHook);
    }
    void UnhookWndProc()
    {
        HWND window = FindWindowA(NULL, "Counter-Strike: Global Offensive");
        SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)oWndProc);
    }
    void RenderTabs(TAB& tab)
    {
        if (ImGui::Button("Visuals", ImVec2(0, 25.f)))
            tab = VISUALS;
        ImGui::SameLine();
        if (ImGui::Button("Aimbot", ImVec2(0, 25.f)))
            tab = AIMBOT;
    }
    void RenderVisuals()
    {
        ImGui::Checkbox("Enable", &Options::bVisuals);
        {
            ImGui::Checkbox("Box ESP", &Options::bBoxEsp);
            ImGui::Checkbox("Snapline", &Options::bSnapline);
            ImGui::Checkbox("Health Bar", &Options::bHealthBar);
        }
        if (!Options::bBoxEsp && !Options::bSnapline && !Options::bHealthBar && !Options::bAimbotFov)
            Options::bAnyVisualFeaturesEnabled = false;
        else //This if statement checks if any features are enabled so don't uselessly render
            Options::bAnyVisualFeaturesEnabled = true;
    }
    void RenderAimbot()
    {
        ImGui::Checkbox("Enable", &Options::bAimbot);
        {
            ImGui::SliderFloat("Smooth Amount", &Options::fSmoothAmount, 0, 1);
            ImGui::Checkbox("Show FOV", &Options::bAimbotFov);
            ImGui::SliderFloat("FOV Range", &Options::fAimbotFov, 0, 45);
            ImGui::SliderFloat("RCS Smoothing", &Options::fRCSSmooth, 0, 2);
            ImGui::Combo("Bone", &Options::currBone, Options::bones, IM_ARRAYSIZE(Options::bones), 3);
            ImGui::Checkbox("Visual Check", &Options::bVisCheck);
        }
        
        Options::bAnyAimbotFeaturesEnabled = true; //Change later. Right now only have one feature
        /*if (!Options::bSmooth)
            Options::bAnyAimbotFeaturesEnabled = false;
        else //This if statement checks if any features are enabled so don't uselessly render
            Options::bAnyAimbotFeaturesEnabled = true;*/
    }

    void RenderMenu()
    {

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("ExoCheats - Dev Build", nullptr, ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
        {
            RenderTabs(tab);
            switch (tab)
            {
            case(VISUALS):
                RenderVisuals();
                break;
            case(AIMBOT):
                RenderAimbot();
                break;
            default:
                break;
            }

        }

        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    }

}