#include "pch.h"
#include "patternscan.h"
#include <Psapi.h>


//Splits combo pattern into mask/pattern, accepts all wildcard variations
void Pattern::Parse(char* combo, char* pattern, char* mask)
{
	char lastChar = ' ';
	int j = 0;

	for (unsigned int i = 0; i < strlen(combo); i++)
	{
		if ((combo[i] == '?' || combo[i] == '*') && (lastChar != '?' && lastChar != '*'))
		{
			pattern[j] = mask[j] = '?';
			j++;
		}

		else if (isspace(lastChar))
		{
			pattern[j] = lastChar = (char)strtol(&combo[i], 0, 16);
			mask[j] = 'x';
			j++;
		}
		lastChar = combo[i];
	}
	pattern[j] = mask[j] = '\0';
}

//Basic Scan
char* Pattern::ScanBasic(char* pattern, char* mask, char* begin, intptr_t size)
{
	intptr_t patternLen = strlen(mask);

	for (int i = 0; i < size; i++)
	{
		bool found = true;
		for (int j = 0; j < patternLen; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(char*)((intptr_t)begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return nullptr;
}

char* Pattern::ScanInternal(char* combo, char* begin)
{
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)begin, &modInfo, sizeof(modInfo));

	char* pattern = new char[strlen(combo)];
	char* mask = new char[strlen(combo)];
	Parse(combo, pattern, mask);

	char* match = nullptr;
	MEMORY_BASIC_INFORMATION mbi;
	for (char* curr = begin; curr < begin + modInfo.SizeOfImage; curr += mbi.RegionSize)
	{
		if (!VirtualQuery(curr, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;
		match = ScanBasic(pattern, mask, curr, mbi.RegionSize);
		if (match != nullptr)
			break;
	}
	delete[] pattern;
	delete[] mask;

	return match;
}
