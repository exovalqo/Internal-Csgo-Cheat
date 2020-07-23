#pragma once
#include "pch.h"

namespace SigScan
{
	std::unique_ptr<char[]> FindSig(char* pattern, char* mask, char* begin, uintptr_t size);
	std::unique_ptr<char[]> ScanInternal(char* pattern, char* mask, char* begin, uintptr_t size);
	uintptr_t FindOffset(char* pattern, char* mask, char* begin, uintptr_t size, uintptr_t offset);
};

