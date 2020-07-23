#include "pch.h"
#include "Globals.h"
#include "Utils/CGlobalVars.h"
#include "IEngineTrace.h"
#include "IVEngineClient.h"
#include "CUserCmd.h"

namespace Globals
{
	CGlobalVars* pGlobalVars = nullptr;
	IEngineTrace* pEngineTrace = nullptr;
	IVEngineClient* pEngineClient = nullptr;
	CUserCmd* pCmd = nullptr;
}