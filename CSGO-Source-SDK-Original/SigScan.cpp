#include "pch.h"
#include "SigScan.h"

namespace SigScan
{
	using namespace std;

	unique_ptr<char[]> SigScan::FindSig(char* pattern, char* mask, char* begin, uintptr_t size)
	{
		intptr_t patternSize = strlen(mask);

		

		for (int j = 0; j < size; j++)
		{
			unique_ptr<char[]> dPattern(new char[patternSize]);
			bool isFound = true;
			for (int i = 0; i < patternSize; i++)
			{
				if (mask[i] != '?' && pattern[i] != begin[j + i])
				{
					isFound = false;
					break;
				}
				dPattern[i] = begin[i];
			}
			if (isFound)
				return dPattern;
		}
		return nullptr;
	}

	unique_ptr<char[]> SigScan::ScanInternal(char* pattern, char* mask, char* begin, uintptr_t size)
	{
		unique_ptr<char[]> match;
		MEMORY_BASIC_INFORMATION mbi;
		for (char* curr = begin; curr < begin + size; curr += mbi.RegionSize)
		{
			if (!VirtualQuery(curr, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				continue;
			match = FindSig(pattern, mask, curr, mbi.RegionSize);
			if (match != nullptr)
				break;
		}
		return match;
	}

	uintptr_t SigScan::FindOffset(char* pattern, char* mask, char* modBase, uintptr_t size, uintptr_t offset)
	{
		unique_ptr<char[]> rnOffset = ScanInternal(pattern, mask, modBase, size);
		
		return *(uintptr_t*)(&rnOffset + offset);

	}
}
