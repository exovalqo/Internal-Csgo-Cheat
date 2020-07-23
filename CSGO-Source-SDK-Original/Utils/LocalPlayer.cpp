#include "pch.h"
#include "Localplayer.h"
#include "../Offsets.h"
#include "Vectors.h"
#include "QAngle.h"
#include <cstdint>
#include <Windows.h>
#include <math.h>

LocalPlayer* LocalPlayer::Get()
{
	static uint32_t clientModule = (uint32_t)(GetModuleHandleA("client_panorama.dll"));
	static LocalPlayer* localPlayer = (LocalPlayer*)(clientModule + Offsets::Signatures::dwLocalPlayer);

	return localPlayer;
}
Vector3 LocalPlayer::GetEyePos()
{
	return GetOrigin() + GetViewOffset();
}
int LocalPlayer::GetFov()
{
	return *(int*)(*(uintptr_t*)this + Offsets::NetVars::m_iFOVStart);
}
QAngle LocalPlayer::GetAimPunchAngle()
{
	return *(QAngle*)(*(uintptr_t*)this + Offsets::NetVars::m_aimPunchAngle);
}
int LocalPlayer::GetShotsFired()
{
	return *(int*)(*(uintptr_t*)this + Offsets::NetVars::m_iShotsFired);
}
bool LocalPlayer::IsValid()
{
	if (this == nullptr) return false;
	if (*(uintptr_t*)this == NULL) return false;
	return true;
}
Vector3 LocalPlayer::GetOrigin()
{
	return *(Vector3*)(*(uintptr_t*)this +  Offsets::NetVars::m_vecOrigin);
}

Vector3 LocalPlayer::GetViewOffset()
{
	return *(Vector3*)(*(uintptr_t*)this + Offsets::NetVars::m_vecViewOffset);
}

int LocalPlayer::GetHealth()
{
	return *(int*)(*(uintptr_t*)this + Offsets::NetVars::m_iHealth);
}

int LocalPlayer::GetTeam()
{
	return *(int*)(*(uintptr_t*)this + Offsets::NetVars::m_iTeamNum);
}

float LocalPlayer::GetDistance(Vector3* other)
{
	Vector3 myPos = GetOrigin();
	Vector3 delta = Vector3(other->x - myPos.x, other->y - myPos.y, other->z - myPos.z);

	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

double PI = 3.14159265358;

QAngle LocalPlayer::GetAnglesFromVector(Vector3* target)
{
	Vector3 origin = GetOrigin();
	Vector3 viewOffset = GetViewOffset();
	Vector3* myPos = &(origin + viewOffset);

	Vector3 deltaVec = { target->x - myPos->x, target->y - myPos->y, target->z - myPos->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);

	return QAngle(pitch, yaw, 0);
}