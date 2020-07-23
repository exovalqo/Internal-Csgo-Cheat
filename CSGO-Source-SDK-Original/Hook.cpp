#include "pch.h"
#include "Hook.h"
#include "PatternScan/patternscan.h"
#include "Features/Features.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imguihelper.h"
#include "imgui/Options.h"
#include "Globals.h"
#include "IVEngineClient.h"
#include "CUserCmd.h"

namespace HookUtils
{
	bool HookUtils::Detour(char* src, char* dst, SIZE_T len)
	{
		if (len < 5)
			return false;
		DWORD oldProt = 0;

		if (VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oldProt))
		{
			memset(dst, 0x90, len);
			SIZE_T delta = (uintptr_t)src - (uintptr_t)dst - 5;

			*dst = 0xE9;
			*(int*)(dst + 1) = delta;
			if (VirtualProtect(dst, len, oldProt, &oldProt))
				return true;
		}
#ifdef _DEBUG
		std::cout << "Error: " << GetLastError() << std::endl;
#endif
		return false;
	}
	bool HookUtils::TrampHook(char* src, char* dst, SIZE_T len, uintptr_t &rnHook)
	{
		if (len < 5)
			return false;

		// Alloc Memory To Store Stolen Bytes
		char* alloc = (char*)VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#ifdef _DEBUG
		std::cout << "Address of Trampoline Hook: " << std::hex << (uintptr_t)alloc << std::endl;
#endif
		//Copy Bytes into new memory
		memcpy_s(alloc, len, dst, len);
		// Find Delta - 5
		size_t delta = (uintptr_t)dst - (uintptr_t)alloc - 5;
		// write replace
		*(alloc + len) = 0xE9;
		*(int*)(alloc + len + 1) = delta;

		//Call Detour Function
		if (!Detour(src, dst, len))
			return false;

		rnHook = (uintptr_t)alloc;
		return true;
	}
	bool HookUtils::Patch(char* src, char* dst, SIZE_T len)
	{
		DWORD oProtect;
		if (VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oProtect))
		{
			memcpy_s(dst, len, src, len);
			if (VirtualProtect(dst, len, oProtect, &oProtect))
				return true;
		}
		return false;
	}
}

namespace HookFunctions
{
	char EndSceneBytes[7];
	char CreateMoveBytes[9];
	bool IsImguiInit = false;
	

	HRESULT WINAPI hkEndScene(IDirect3DDevice9* pDevice)
	{
		//save state
		IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
		pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
		pDevice->GetVertexDeclaration(&vertDec);
		pDevice->GetVertexShader(&vertShader);

		if (!Draw::pDevice)
			Draw::pDevice = pDevice;
		if (!IsImguiInit)
		{
			imguihelper::InitImGui(pDevice);

			IsImguiInit = true;
			//imguihelper::bShowMenu = fal;
		}
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			Options::bShowMenu = !Options::bShowMenu;
		}

		if (Options::bShowMenu)
		{
			imguihelper::RenderMenu();
		}

		if(Options::bVisuals)
			if(Options::bAnyVisualFeaturesEnabled)
				Features::Visuals::Draw(); //Visual Drawing

		//restore state
		pixel_state->Apply();
		pixel_state->Release();
		pDevice->SetVertexDeclaration(vertDec);
		pDevice->SetVertexShader(vertShader);

		return Hook::oEndScene(pDevice);

	}
	bool __fastcall hkCreateMove(void* ecx, void* edx, int flInputSampletime, CUserCmd* cmd)
	{
		Globals::pEngineClient->SetViewAngles(cmd->viewangles);
		if(Options::bAimbot)
			if(Options::bAnyAimbotFeaturesEnabled)
				Features::Aimbot::Run();
		Hook::oCreateMove(ecx, flInputSampletime, cmd);
		return false;
	}

}

namespace Hook
{
	tEndScene oEndScene = nullptr;
	tCreateMove oCreateMove = nullptr;
	uintptr_t oCreateMoveAddress = 0;
	void* pTable[119];

	bool Hook::HookCreateMove()
	{
		//CreateMove Pattern
		std::string pattern = "0x55 0x8B 0xEC 0x8B 0x0D ? ? ? ? 0x85 0xC9 0x75 0x06 0xB0 0x01 0x5D 0xC2 0x08 0x00";
		oCreateMoveAddress = (uintptr_t)Pattern::ScanInternal((char*)pattern.c_str(), (char*)GetModuleHandleA("client.dll"));
#ifdef _DEBUG
		std::cout << xorstr_("Address of CreateMove: ") << std::hex << oCreateMoveAddress << std::endl;
#endif
		uintptr_t rnAddy = 0; //Return address of tramp hook
		if (!HookUtils::TrampHook((char*)HookFunctions::hkCreateMove, (char*)oCreateMoveAddress,sizeof(HookFunctions::CreateMoveBytes), rnAddy))
			return false;
		oCreateMove = (tCreateMove)rnAddy;

		//Copy stolen bytes for patching when unhooking
		memcpy(HookFunctions::CreateMoveBytes, oCreateMove, sizeof(HookFunctions::CreateMoveBytes));
		return true;
	}
	bool Hook::HookEndScene(HWND wnd)
	{
		if (!GetD3DVTable(pTable, sizeof(pTable), wnd))
			return false;
#ifdef _DEBUG
				std::cout << xorstr_("EndScene Address: ") << std::hex << (uintptr_t)pTable[42] << std::endl;
				std::cout << xorstr_("HkEndScene Function: ") << std::hex << HookFunctions::hkEndScene << std::endl;
#endif
		uintptr_t hkAddy = 0;
		if (!HookUtils::TrampHook((char*)HookFunctions::hkEndScene, (char*)pTable[42],
			sizeof(HookFunctions::EndSceneBytes), hkAddy))
			return false;

		oEndScene = (tEndScene)hkAddy;

		memcpy(HookFunctions::EndSceneBytes, oEndScene, sizeof(HookFunctions::EndSceneBytes));
#ifdef _DEBUG
		std::cout << xorstr_("oEndScene Address: ") << std::hex << oEndScene << std::endl;
		std::cout << xorstr_("EndSceneBytes Address: ") << std::hex << &HookFunctions::EndSceneBytes << std::endl;
#endif
		
	}

	
	void Hook::UnHook()
	{
		//Disable Drawing To Prevent Weird Crashing
		Options::bVisuals = false;
		Options::bAimbot = false;
		Options::bShowMenu = false;

		HookUtils::Patch(HookFunctions::EndSceneBytes, (char*)pTable[42], sizeof(HookFunctions::EndSceneBytes));
		HookUtils::Patch(HookFunctions::CreateMoveBytes, (char*)oCreateMoveAddress, sizeof(HookFunctions::CreateMoveBytes));
		imguihelper::UnhookWndProc();
	}


	bool Hook::GetD3DVTable(void** pTable, SIZE_T size, HWND wnd)
	{
		IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		if (!pD3D)
			return false;

		IDirect3DDevice9* pDummyDevice = nullptr;

		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = wnd;

		HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
		if (result != S_OK)
		{
			d3dpp.Windowed = FALSE;
			result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
			if (result != S_OK)
			{
				pD3D->Release();
				return false;
			}
		}
		memcpy(pTable, *(void***)(pDummyDevice), size);


		pDummyDevice->Release();
		pD3D->Release();

		return true;
	}
}