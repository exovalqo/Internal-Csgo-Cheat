#include "pch.h"
#include "SrcDumper.h"
#include "../Offsets.h"
#include "jsonxx/jsonxx.h"

SrcDumper::SrcDumper() {}
SrcDumper::SrcDumper(jsonxx::Object* json)
{
	jsonConfig = json;
}

intptr_t SrcDumper::GetdwGetAllClassesAddr()
{
	for (auto s : signatures)
	{
		if (s.name == "dwGetAllClasses")
		{
			return s.result;
		}
	}
	return 0;
}


intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName)
{
	for (int i = 0; i < table->m_nProps; i++)
	{
		RecvProp prop = table->m_pProps[i];

		if (!_stricmp(prop.m_pVarName, netvarName))
		{
			return prop.m_Offset;
		}

		if (prop.m_pDataTable)
		{
			intptr_t offset = GetOffset(prop.m_pDataTable, tableName, netvarName);

			if (offset)
			{
				return offset + prop.m_Offset;
			}
		}
	}
	return 0;
}

intptr_t SrcDumper::GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass)
{
	ClientClass* currNode = clientClass;

	for (auto currNode = clientClass; currNode; currNode = currNode->m_pNext)
	{
		if (!_stricmp(tableName, currNode->m_pRecvTable->m_pNetTableName))
		{
			return GetOffset(currNode->m_pRecvTable, tableName, netvarName);
		}
	}

	return 0;
}

void SrcDumper::ProcessNetvars()
{
	jsonxx::Array netvars = jsonConfig->get<jsonxx::Array>("netvars");

	//Loop through JSON netvar array
	for (size_t i = 0; i < netvars.size(); i++)
	{
		jsonxx::Object curr = netvars.get<jsonxx::Object>(i);
		NetvarData currData;

		currData.name = curr.get<jsonxx::String>("name");
		currData.prop = curr.get<jsonxx::String>("prop");
		currData.table = curr.get<jsonxx::String>("table");

		//dump offsets from json into vector
		if (curr.has<jsonxx::Number>("offset"))
		{
			jsonxx::Number offset = curr.get<jsonxx::Number>("offset");
			currData.offset = (int)offset;
		}

		Netvars.push_back(currData);
	}

	//^ we parsed the json into Netvars

	//now we can locally load client_panorama.dll and get the netvar offsets internally
	HMODULE hMod = GetModuleHandleA("client.dll");

	//Get First ClientClass in the linked list
	ClientClass* dwGetallClassesAddr = (ClientClass*)((intptr_t)hMod + GetdwGetAllClassesAddr());
	
	//for each netvar in netvars, get the offset
	
	for (NetvarData& n : Netvars)
	{
		n.result = GetNetVarOffset(n.table.c_str(), n.prop.c_str(), dwGetallClassesAddr);

		if (n.offset)
		{
			n.result += n.offset;
		}
	}
}

void SrcDumper::GenerateOffsets()
{
	//Netvars
	using namespace Offsets::NetVars;
	int count = 0;

	m_ArmorValue = Netvars[count++].result;
	 m_Collision = Netvars[count++].result;
	 m_CollisionGroup = Netvars[count++].result;
	 m_Local = Netvars[count++].result;
	 m_MoveType = Netvars[count++].result;
	 m_OriginalOwnerXuidHigh = Netvars[count++].result;
	 m_OriginalOwnerXuidLow = Netvars[count++].result;
	 m_aimPunchAngle = Netvars[count++].result;
	 m_aimPunchAngleVel = Netvars[count++].result;
	 m_bGunGameImmunity = Netvars[count++].result;
	 m_bHasDefuser = Netvars[count++].result;
	 m_bHasHelmet = Netvars[count++].result;
	 m_bInReload = Netvars[count++].result;
	 m_bIsDefusing = Netvars[count++].result;
	 m_bIsScoped = Netvars[count++].result;
	 m_bSpotted = Netvars[count++].result;
	 m_bSpottedByMask = Netvars[count++].result;
	 m_dwBoneMatrix = Netvars[count++].result;
	 m_fAccuracyPenalty = Netvars[count++].result;
	 m_fFlags = Netvars[count++].result;
	 m_flFallbackWear = Netvars[count++].result;
	 m_flFlashDuration = Netvars[count++].result;
	 m_flFlashMaxAlpha = Netvars[count++].result;
	 m_flNextPrimaryAttack = Netvars[count++].result;
	 m_hActiveWeapon = Netvars[count++].result;
	 m_hMyWeapons = Netvars[count++].result;
	 m_hObserverTarget = Netvars[count++].result;
	 m_hOwner = Netvars[count++].result;
	 m_hOwnerEntity = Netvars[count++].result;
	 m_iAccountID = Netvars[count++].result;
	 m_iClip1 = Netvars[count++].result;
	 m_iCompetitiveRanking = Netvars[count++].result;
	 m_iCompetitiveWins = Netvars[count++].result;
	 m_iCrosshairId = Netvars[count++].result;
	 m_iEntityQuality = Netvars[count++].result;
	 m_iFOVStart = Netvars[count++].result;
	 m_iFOV = Netvars[count++].result;
	 m_iGlowIndex = Netvars[count++].result;
	 m_iHealth = Netvars[count++].result;
	 m_iItemDefinitionIndex = Netvars[count++].result;
	 m_iItemIDHigh = Netvars[count++].result;
	 m_iObserverMode = Netvars[count++].result;
	 m_iShotsFired = Netvars[count++].result;
	 m_iState = Netvars[count++].result;
	 m_iTeamNum = Netvars[count++].result;
	 m_lifeState = Netvars[count++].result;
	 m_nFallbackPaintKit = Netvars[count++].result;
	 m_nFallbackSeed = Netvars[count++].result;
	 m_nFallbackStatTrak = Netvars[count++].result;
	 m_nForceBone = Netvars[count++].result;
	 m_nTickBase = Netvars[count++].result;
	 m_rgflCoordinateFrame = Netvars[count++].result;
	 m_szCustomName = Netvars[count++].result;
	 m_szLastPlaceName = Netvars[count++].result;
	 m_vecOrigin = Netvars[count++].result;
	 m_vecVelocity = Netvars[count++].result;
	 m_vecViewOffset = Netvars[count++].result;
	 m_viewPunchAngle = Netvars[count++].result;
	 m_thirdPersonViewAngles = Netvars[count++].result;
	 m_clrRender = Netvars[count++].result;
	 m_flC4Blow = Netvars[count++].result;
	 m_flTimerLength = Netvars[count++].result;
	 m_flDefuseLength = Netvars[count++].result;
	 m_flDefuseCountDown = Netvars[count++].result;
	 cs_gamerules_data = Netvars[count++].result;
	 m_SurvivalRules = Netvars[count++].result;
	 m_SurvivalGameRuleDecisionTypes = Netvars[count++].result;
	 m_bIsValveDS = Netvars[count++].result;
	 m_bFreezePeriod = Netvars[count++].result;
	 m_bBombPlanted = Netvars[count++].result;
	 m_bIsQueuedMatchmaking = Netvars[count++].result;
	 m_flSimulationTime = Netvars[count++].result;
	 m_flLowerBodyYawTarget = Netvars[count++].result;
	 m_angEyeAnglesX = Netvars[count++].result;
	 m_angEyeAnglesY = Netvars[count++].result;
	 m_flNextAttack = Netvars[count++].result;
	 m_iMostRecentModelBoneCounter = Netvars[count++].result;
	 m_flLastBoneSetupTime = Netvars[count++].result;
	 m_bStartedArming = Netvars[count++].result;
	 m_bUseCustomBloomScale = Netvars[count++].result;
	 m_bUseCustomAutoExposureMin = Netvars[count++].result;
	 m_bUseCustomAutoExposureMax = Netvars[count++].result;
	 m_flCustomBloomScale = Netvars[count++].result;
	 m_flCustomAutoExposureMin = Netvars[count++].result;
	 m_flCustomAutoExposureMax = Netvars[count++].result;
	

	// Signatures
	using namespace Offsets::Signatures;
	count = 0;

	 dwClientState = signatures[count++].result;
	 dwClientState_GetLocalPlayer = signatures[count++].result;
	 dwClientState_IsHLTV = signatures[count++].result;
	 dwClientState_Map = signatures[count++].result;
	 dwClientState_MapDirectory = signatures[count++].result;
	 dwClientState_MaxPlayer = signatures[count++].result;
	 dwClientState_PlayerInfo = signatures[count++].result;
	 dwClientState_State = signatures[count++].result;
	 dwClientState_ViewAngles = signatures[count++].result;
	 clientstate_delta_ticks = signatures[count++].result;
	 clientstate_last_outgoing_command = signatures[count++].result;
	 clientstate_choked_commands = signatures[count++].result;
	 clientstate_net_channel = signatures[count++].result;
	 dwEntityList = signatures[count++].result;
	 dwForceAttack = signatures[count++].result;
	 dwForceAttack2 = signatures[count++].result;
	 dwForceBackward = signatures[count++].result;
	 dwForceForwar = signatures[count++].result;
	 dwForceJump = signatures[count++].result;
	 dwForceLeft = signatures[count++].result;
	 dwForceRight = signatures[count++].result;
	 dwGameDir = signatures[count++].result;
	 dwGameRulesProxy = signatures[count++].result;
	 dwGetAllClasses = signatures[count++].result;
	 dwGlobalVars = signatures[count++].result;
	 dwGlowObjectManager = signatures[count++].result;
	 dwInput = signatures[count++].result;
	 dwInterfaceLinkList = signatures[count++].result;
	 dwLocalPlayer = signatures[count++].result;
	 dwMouseEnable = signatures[count++].result;
	 dwMouseEnablePtr = signatures[count++].result;
	 dwPlayerResource = signatures[count++].result;
	 dwRadarBase = signatures[count++].result;
	 dwSensitivity = signatures[count++].result;
	 dwSensitivityPtr = signatures[count++].result;
	 dwSetClanTag = signatures[count++].result;
	 dwViewMatrix = signatures[count++].result;
	 dwWeaponTable = signatures[count++].result;
	 dwWeaponTableIndex = signatures[count++].result;
	 dwYawPtr = signatures[count++].result;
	 dwZoomSensitivityRatioPtr = signatures[count++].result;
	 dwbSendPackets = signatures[count++].result;
	 dwppDirect3DDevice9 = signatures[count++].result;
	 m_pStudioHdr = signatures[count++].result;
	 m_yawClassPtr = signatures[count++].result;
	 m_pitchClassPtr = signatures[count++].result;
	 interface_engine_cvar = signatures[count++].result;
	 convar_name_hash_table = signatures[count++].result;
	 m_bDormant = signatures[count++].result;
	 model_ambient_min = signatures[count++].result;
	 set_abs_angles = signatures[count++].result;
	 set_abs_origin = signatures[count++].result;
	 is_c4_owner = signatures[count++].result;
	 force_update_spectator_glow = signatures[count++].result;
	 anim_overlays = signatures[count++].result;
	 m_flSpawnTime = signatures[count++].result;
	 find_hud_element = signatures[count++].result;
}


void SrcDumper::Dump()
{
	ProcessSignatures();

	ProcessNetvars();

	//Store Offsets In Offsets.h (Namespace Offsets)
	GenerateOffsets();

	/*for (SigData s : signatures)
		std::cout << s.name << ": " << std::hex << s.result << std::endl;
	for (NetvarData n : Netvars)
		std::cout << n.name << ": " << std::hex << n.result << std::endl;
		*/
}

std::string SrcDumper::GetSigBase(SigData sigdata)
{
	std::string sigBase;

	if (sigdata.name.find("clientstate_") != std::string::npos)
	{
		sigBase = "[" + sigdata.module + "dwClientState]";
	}

	else
	{
		sigBase = sigdata.module;
	}

	return sigBase;
	//does sig.module define the base module of the offset as well as signature?  think so
}
namespace Offsets
{
	namespace NetVars
	{
		 ptrdiff_t m_ArmorValue;
		 ptrdiff_t m_Collision;
		 ptrdiff_t m_CollisionGroup;
		 ptrdiff_t m_Local;
		 ptrdiff_t m_MoveType;
		 ptrdiff_t m_OriginalOwnerXuidHigh;
		 ptrdiff_t m_OriginalOwnerXuidLow;
		 ptrdiff_t m_aimPunchAngle;
		 ptrdiff_t m_aimPunchAngleVel;
		 ptrdiff_t m_bGunGameImmunity;
		 ptrdiff_t m_bHasDefuser;
		 ptrdiff_t m_bHasHelmet;
		 ptrdiff_t m_bInReload;
		 ptrdiff_t m_bIsDefusing;
		 ptrdiff_t m_bIsScoped;
		 ptrdiff_t m_bSpotted;
		 ptrdiff_t m_bSpottedByMask;
		 ptrdiff_t m_dwBoneMatrix;
		 ptrdiff_t m_fAccuracyPenalty;
		 ptrdiff_t m_fFlags;
		 ptrdiff_t m_flFallbackWear;
		 ptrdiff_t m_flFlashDuration;
		 ptrdiff_t m_flFlashMaxAlpha;
		 ptrdiff_t m_flNextPrimaryAttack;
		 ptrdiff_t m_hActiveWeapon;
		 ptrdiff_t m_hMyWeapons;
		 ptrdiff_t m_hObserverTarget;
		 ptrdiff_t m_hOwner;
		 ptrdiff_t m_hOwnerEntity;
		 ptrdiff_t m_iAccountID;
		 ptrdiff_t m_iClip1;
		 ptrdiff_t m_iCompetitiveRanking;
		 ptrdiff_t m_iCompetitiveWins;
		 ptrdiff_t m_iCrosshairId;
		 ptrdiff_t m_iEntityQuality;
		 ptrdiff_t m_iFOVStart;
		 ptrdiff_t m_iFOV;
		 ptrdiff_t m_iGlowIndex;
		 ptrdiff_t m_iHealth;
		 ptrdiff_t m_iItemDefinitionIndex;
		 ptrdiff_t m_iItemIDHigh;
		 ptrdiff_t m_iObserverMode;
		 ptrdiff_t m_iShotsFired;
		 ptrdiff_t m_iState;
		 ptrdiff_t m_iTeamNum;
		 ptrdiff_t m_lifeState;
		 ptrdiff_t m_nFallbackPaintKit;
		 ptrdiff_t m_nFallbackSeed;
		 ptrdiff_t m_nFallbackStatTrak;
		 ptrdiff_t m_nForceBone;
		 ptrdiff_t m_nTickBase;
		 ptrdiff_t m_rgflCoordinateFrame;
		 ptrdiff_t m_szCustomName;
		 ptrdiff_t m_szLastPlaceName;
		 ptrdiff_t m_vecOrigin;
		 ptrdiff_t m_vecVelocity;
		 ptrdiff_t m_vecViewOffset;
		 ptrdiff_t m_viewPunchAngle;
		 ptrdiff_t m_thirdPersonViewAngles;
		 ptrdiff_t m_clrRender;
		 ptrdiff_t m_flC4Blow;
		 ptrdiff_t m_flTimerLength;
		 ptrdiff_t m_flDefuseLength;
		 ptrdiff_t m_flDefuseCountDown;
		 ptrdiff_t cs_gamerules_data;
		 ptrdiff_t m_SurvivalRules;
		 ptrdiff_t m_SurvivalGameRuleDecisionTypes;
		 ptrdiff_t m_bIsValveDS;
		 ptrdiff_t m_bFreezePeriod;
		 ptrdiff_t m_bBombPlanted;
		 ptrdiff_t m_bIsQueuedMatchmaking;
		 ptrdiff_t m_flSimulationTime;
		 ptrdiff_t m_flLowerBodyYawTarget;
		 ptrdiff_t m_angEyeAnglesX;
		 ptrdiff_t m_angEyeAnglesY;
		 ptrdiff_t m_flNextAttack;
		 ptrdiff_t m_iMostRecentModelBoneCounter;
		 ptrdiff_t m_flLastBoneSetupTime;
		 ptrdiff_t m_bStartedArming;
		 ptrdiff_t m_bUseCustomBloomScale;
		 ptrdiff_t m_bUseCustomAutoExposureMin;
		 ptrdiff_t m_bUseCustomAutoExposureMax;
		 ptrdiff_t m_flCustomBloomScale;
		 ptrdiff_t m_flCustomAutoExposureMin;
		 ptrdiff_t m_flCustomAutoExposureMax;
	}

	namespace Signatures
	{
		 ptrdiff_t dwClientState;
		 ptrdiff_t dwClientState_GetLocalPlayer;
		 ptrdiff_t dwClientState_IsHLTV;
		 ptrdiff_t dwClientState_Map;
		 ptrdiff_t dwClientState_MapDirectory;
		 ptrdiff_t dwClientState_MaxPlayer;
		 ptrdiff_t dwClientState_PlayerInfo;
		 ptrdiff_t dwClientState_State;
		 ptrdiff_t dwClientState_ViewAngles;
		 ptrdiff_t clientstate_delta_ticks;
		 ptrdiff_t clientstate_last_outgoing_command;
		 ptrdiff_t clientstate_choked_commands;
		 ptrdiff_t clientstate_net_channel;
		 ptrdiff_t dwEntityList;
		 ptrdiff_t dwForceAttack;
		 ptrdiff_t dwForceAttack2;
		 ptrdiff_t dwForceBackward;
		 ptrdiff_t dwForceForwar;
		 ptrdiff_t dwForceJump;
		 ptrdiff_t dwForceLeft;
		 ptrdiff_t dwForceRight;
		 ptrdiff_t dwGameDir;
		 ptrdiff_t dwGameRulesProxy;
		 ptrdiff_t dwGetAllClasses;
		 ptrdiff_t dwGlobalVars;
		 ptrdiff_t dwGlowObjectManager;
		 ptrdiff_t dwInput;
		 ptrdiff_t dwInterfaceLinkList;
		 ptrdiff_t dwLocalPlayer;
		 ptrdiff_t dwMouseEnable;
		 ptrdiff_t dwMouseEnablePtr;
		 ptrdiff_t dwPlayerResource;
		 ptrdiff_t dwRadarBase;
		 ptrdiff_t dwSensitivity;
		 ptrdiff_t dwSensitivityPtr;
		 ptrdiff_t dwSetClanTag;
		 ptrdiff_t dwViewMatrix;
		 ptrdiff_t dwWeaponTable;
		 ptrdiff_t dwWeaponTableIndex;
		 ptrdiff_t dwYawPtr;
		 ptrdiff_t dwZoomSensitivityRatioPtr;
		 ptrdiff_t dwbSendPackets;
		 ptrdiff_t dwppDirect3DDevice9;
		 ptrdiff_t m_pStudioHdr;
		 ptrdiff_t m_yawClassPtr;
		 ptrdiff_t m_pitchClassPtr;
		 ptrdiff_t interface_engine_cvar;
		 ptrdiff_t convar_name_hash_table;
		 ptrdiff_t m_bDormant;
		 ptrdiff_t model_ambient_min;
		 ptrdiff_t set_abs_angles;
		 ptrdiff_t set_abs_origin;
		 ptrdiff_t is_c4_owner;
		 ptrdiff_t force_update_spectator_glow;
		 ptrdiff_t anim_overlays;
		 ptrdiff_t m_flSpawnTime;
		 ptrdiff_t find_hud_element;

	}
}