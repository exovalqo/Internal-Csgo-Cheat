#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>


enum class DataType
{
	DT_BYTE, DT_FLOAT, DT_INT, DT_INT_HEX
};

struct SigData
{
	//json config vars
	std::string name;
	int extra{ 0 };
	bool relative{ false };
	std::string module;
	std::vector<int> offsets;
	std::string comboPattern;

	uintptr_t result;

	//extra vars for output
	DataType dataType;
	std::string offsetBase;

	//Perform the pattern scan
	void Scan();
};