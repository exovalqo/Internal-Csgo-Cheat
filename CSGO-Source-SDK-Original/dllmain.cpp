// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Init.h"
#include "Offsets.h"
#include "Utils/xor.h"
#include "imgui/Options.h"
#include <iostream>
#include <Windows.h>

#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9.lib")


void AttachConsole(FILE* f)
{
    AllocConsole();
    freopen_s(&f,"CONOUT$", "w", stdout);
}

INT WINAPI OnAttach(HMODULE hModule)
{
#ifdef _DEBUG
    FILE* pConsole = stdout;
    AttachConsole(pConsole);
#endif
    if (Init::StartInit())
    {
    #ifdef _DEBUG
            std::cout << xorstr_("Successfully Initialized!") << std::endl;
    #endif
        while (!GetAsyncKeyState(VK_DOWN) & 1)
        {
            Sleep(100);
        }
        Init::Free();
    }
#ifdef _DEBUG
    std::cout << xorstr_("Detached Cheat") << std::endl;
#endif
    Sleep(500);
    fclose(pConsole);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)OnAttach, hModule, NULL, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

