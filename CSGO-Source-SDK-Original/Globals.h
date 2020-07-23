#pragma once
class CGlobalVars;
class IEngineTrace;
class IVEngineClient;
class CUserCmd;

namespace Globals
{
	extern CGlobalVars* pGlobalVars;
	extern IEngineTrace* pEngineTrace;
	extern IVEngineClient* pEngineClient;
	extern CUserCmd* pCmd;
}

