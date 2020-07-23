#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include "patternscan.h"
#include "jsonxx\jsonxx.h"
#include "SigData.h"

jsonxx::Object ParseConfig();

class Dumper
{
public:
	jsonxx::Object* jsonConfig{ nullptr };
	std::vector<SigData> signatures;

	Dumper();
	Dumper(jsonxx::Object* json);

	void ProcessSignatures();

	void GenerateHeaderOuput();

	virtual void Dump();
};