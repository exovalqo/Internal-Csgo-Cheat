#include "pch.h"
#include "Init.h"
#include "Hook.h"
#include "PatternScan/GHDumper.h"
#include "PatternScan/SrcDumper.h"
#include "Globals.h"
#include "Offsets.h"

HWND window;
namespace Init
{
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
	{
		DWORD wndProcId;
		GetWindowThreadProcessId(handle, &wndProcId);

		if (GetCurrentProcessId() != wndProcId)
			return TRUE; // skip to next window

		window = handle;
		return FALSE; // window found abort search
	}
	HWND GetProcessWindow()
	{
		window = NULL;
		EnumWindows(EnumWindowsCallback, NULL);
		return window;
	}
	void* GetInterface(const char* dll, const char* iFace)
	{
		using tCreateInterface = void* (__cdecl*)(const char*, int*);

		static tCreateInterface engineInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(dll), "CreateInterface");

		int returnCode = 0;
		void* rnInterface = engineInterface(iFace, &returnCode);

		return rnInterface;

	}
	bool InitHooks()
	{
		if (!Hook::HookEndScene(GetProcessWindow()))
			return false;
		if (!Hook::HookCreateMove())
			return false;
		return true;
	}
	void InitOffsets()
	{
		jsonxx::Object jsonConfig = ParseConfig();
		SrcDumper dumper(&jsonConfig);
		dumper.Dump();
		
	}
	void InitGlobals()
	{
		Globals::pGlobalVars = (CGlobalVars*)((uintptr_t)GetModuleHandleA("engine.dll") + Offsets::Signatures::dwGlobalVars);
		Globals::pEngineTrace = (IEngineTrace*)GetInterface("engine.dll", "EngineTraceClient004");
		Globals::pEngineClient = (IVEngineClient*)GetInterface("engine.dll", "Engine")
	}
	bool StartInit()
	{
		//Get Offsets
		InitOffsets();

		
		InitGlobals();
		// Setup Hooks
		if (!InitHooks())
			return false;
		
		return true;

	}
	void Free()
	{
		Hook::UnHook();
	}
}