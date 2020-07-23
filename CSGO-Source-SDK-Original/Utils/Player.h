#pragma once

class Vector3;
class Player
{

private:
	Player();

public:
	static int GetMaxPlayer();
	static Player* GetPlayer(int index);

	int GetHealth();
	bool IsDormant();
	int GetTeam();
	bool IsVisible();
	Vector3 GetEyePos();
	Vector3 GetOrigin();
	Vector3 GetVelocity();
	Vector3 GetViewOffset();
	Vector3 GetBonePos(int boneID);
	bool IsValidPlayer();
};
