#pragma once
#include "pch.h"
#include "Draw.h"

typedef HRESULT(WINAPI* tEndScene)(IDirect3DDevice9* pDevice);
typedef bool(__thiscall* tCreateMove)(void* ecx, int flInputSampletime, void* cmd);

namespace HookUtils
{
	bool Detour(char* src, char* dst, SIZE_T len);
	bool TrampHook(char* src, char* dst, SIZE_T len, uintptr_t &rnHook);
	bool Patch(char* src, char* dst, SIZE_T len);
	
}
namespace HookFunctions
{
	HRESULT WINAPI hkEndScene(IDirect3DDevice9* pDevice);
}
namespace Hook
{
	extern tEndScene oEndScene;
	extern tCreateMove oCreateMove;

	extern void* pTable[119];
	bool GetD3DVTable(void** pTable, SIZE_T size, HWND wnd);
	bool HookCreateMove();
	bool HookEndScene(HWND wnd);
	void UnHook();
};

