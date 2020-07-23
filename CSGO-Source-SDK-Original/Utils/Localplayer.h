#pragma once

class Vector3;
class QAngle;

class LocalPlayer
{

private:

	LocalPlayer();

public:

	static LocalPlayer* Get();

	Vector3 GetOrigin();
	Vector3 GetViewOffset();
	Vector3 GetEyePos();
	QAngle GetAimPunchAngle();
	int GetFov();
	int GetShotsFired();
	bool IsValid();
	int GetHealth();
	int GetTeam();
	QAngle GetAnglesFromVector(Vector3* target);
	float GetDistance(Vector3*);

};