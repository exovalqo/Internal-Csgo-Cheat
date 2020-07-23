#pragma once
namespace Init
{
	void* GetInterface(const char* dll, const char* iFace);
	//bool InitHooks();
	bool StartInit();
	void Free();
};

