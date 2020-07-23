#include "pch.h"
#include "SigData.h"
#include "patternscan.h"
#include "jsonxx\jsonxx.h"

void SigData::Scan()
{
	//Scan for the pattern
	result = (uintptr_t)Pattern::ScanInternal((char*)comboPattern.c_str(), (char*)GetModuleHandleA(module.c_str()));
	

	//first offset is relative to pattern location, different than FindDMAAddy, you must add offset first, then RPM
	if (offsets.size() != 0)
	{
		for (auto o : offsets)
		{
			result = *(uintptr_t*)(result + o);
		}
	}
	//offset into the resulting address, if necessary
	if (extra)
	{
		result += extra;
	}
	//if a relative offset, get the relative offset
	if (relative)
	{
		result -= (uintptr_t)GetModuleHandleA(module.c_str());
	}
}