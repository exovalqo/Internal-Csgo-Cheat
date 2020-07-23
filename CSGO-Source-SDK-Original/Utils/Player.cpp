#include "pch.h"
#include "Player.h"
#include "Localplayer.h"
#include "Vectors.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../IEngineTrace.h"

#include <Windows.h>
#include <cstdint>

Player* Player::GetPlayer(int index)
{
	static uint32_t moduleBase = (uint32_t)GetModuleHandleA("client_panorama.dll");
	static uint32_t entityList = moduleBase + Offsets::Signatures::dwEntityList;

	return (Player*)(entityList + index * 0x10);
}
bool Player::IsValidPlayer()
{
	if (this == nullptr) return false;
	if (*(uintptr_t*)this == NULL) return false;
	LocalPlayer* localPlayer = LocalPlayer::Get();
	if (!localPlayer->IsValid()) return false;
	if (localPlayer->GetTeam() == GetTeam()) return false;
	if (IsDormant()) return false;
	if (GetHealth() < 1) return false;
	return true;
}
Vector3 Player::GetVelocity()
{
	return *(Vector3*)(*(uintptr_t*)this + Offsets::NetVars::m_vecVelocity);
}
bool Player::IsDormant()
{
	return *(bool*)(*(uintptr_t*)this + Offsets::Signatures::m_bDormant);
}
int Player::GetMaxPlayer()
{
	static uint32_t moduleBase = (uintptr_t)GetModuleHandleA("engine.dll");
	return *(int*)(*(uint32_t*)(moduleBase + Offsets::Signatures::dwClientState) + Offsets::Signatures::dwClientState_MaxPlayer);
}

Vector3 Player::GetOrigin()
{
	return *(Vector3*)(*(uintptr_t*)this + Offsets::NetVars::m_vecOrigin);
}

Vector3 Player::GetViewOffset()
{
	return *(Vector3*)(*(uintptr_t*)this + Offsets::NetVars::m_vecViewOffset);
}
Vector3 Player::GetEyePos()
{
	return GetOrigin() + GetViewOffset();
}
Vector3 Player::GetBonePos(int boneID)
{
	uint32_t boneMatrix = *(uint32_t*)(*(uint32_t*)this + Offsets::NetVars::m_dwBoneMatrix);
	Vector3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
	return bonePos;
}
bool Player::IsVisible()
{
	Ray_t ray;
	trace_t trace;
	ray.Init(LocalPlayer::Get()->GetEyePos(), GetEyePos());
	CTraceFilter traceFilter;
	traceFilter.pSkip = (void*)(*(uintptr_t*)LocalPlayer::Get()); //dereference localplayer pointer to get actual localplayer address
	Globals::pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &traceFilter, &trace);
	if (trace.m_pEnt == (Player*)(*(uintptr_t*)this) || trace.fraction == 1.0f) //dereference player pointer to get actual player address
		return true;
	return false;
}
int Player::GetHealth()
{
	return *(int*)(*(uint32_t*)this + Offsets::NetVars::m_iHealth);
}

int Player::GetTeam()
{
	return *(int*)(*(uint32_t*)this + Offsets::NetVars::m_iTeamNum);
}