#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>


namespace Pattern
{
	//Split combo pattern into mask/pattern
	void Parse(char* combo, char* pattern, char* mask);

	//Pattern Scan
	char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);

	char* ScanInternal(char* combo, char* begin);
}