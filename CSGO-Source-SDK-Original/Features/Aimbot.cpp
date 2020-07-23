#include "pch.h"
#include "Features.h"
#include "../Offsets.h"
#include "../Utils/Player.h"
#include "../Utils/LocalPlayer.h"
#include "../Utils/Vectors.h"
#include "../Utils/QAngle.h"
#include "../imgui/Options.h"
#include "../Globals.h"
#include "../Utils/CGlobalVars.h"
#include <Windows.h>
#include <iostream>

uint32_t engineModule;
QAngle* viewAngles;
QAngle oldPunchAngle = { 0, 0, 0 };

namespace Features
{
	namespace Aimbot
	{
		void Clamp(QAngle& angle)
		{
			if (angle.x > 180) angle.x = 180;
			else if (angle.x < -180) angle.x = -180;
			if (angle.y > 89) angle.y = 89;
			else if (angle.y < -89) angle.y = -89;
		}
		void Normalize(QAngle& angle)
		{
			if (angle.x > 180) angle.x -= 360.0f;
			if (angle.x < -180) angle.x += 360.0f;
			if (angle.y > 89) angle.y -= 179.0f;
			if (angle.y < -89) angle.y += 179.0f;
		}
		void AimAt(QAngle& angle)
		{
			viewAngles->y = angle.y; //pitch
			viewAngles->x = angle.x; //yaw
		}
		bool InFov(QAngle& angle)
		{
			LocalPlayer* localplayer = LocalPlayer::Get();
			QAngle delta = angle - *viewAngles;
			Normalize(delta); //Fixes crossing border for 180 to -180

			float length = sqrt(pow(delta.y, 2) + pow(delta.x, 2));
			if (length <= Options::fAimbotFov)
				return true;
			//if (delta.yaw > -5 && delta.yaw < 5)
				//if(delta.pitch > -5 && delta.pitch < 5)
					//return true;
			return false;

		}
		void AddRCSControl(QAngle& angle)
		{
			LocalPlayer* localPlayer = LocalPlayer::Get();
			if (localPlayer->GetShotsFired() > 1)
			{
				QAngle currPunchAngles = localPlayer->GetAimPunchAngle();
				//QAngle deltaPunchAngles = currPunchAngles - oldPunchAngle;
				angle -= currPunchAngles * Options::fRCSSmooth;
				oldPunchAngle = currPunchAngles;
			}
			else
			{
				oldPunchAngle.y = 0;
				oldPunchAngle.x = 0;
			}
		}
		void VelcotiyExtrapolate(Player* player, Vector3& pos)
		{
			pos += (player->GetVelocity() * Globals::pGlobalVars->interval_per_tick);
		}
		void SmoothAngle(QAngle& angle)
		{
			float smoothAmount = Options::fSmoothAmount * Options::smoothMultiplier;
			
			QAngle delta = angle - *viewAngles;
			delta /= smoothAmount;
			Clamp(delta);
			angle = *viewAngles + delta;
		}
		Vector3 GetBoneVec(Player* player)
		{
			switch (Options::currBone)
			{
			case 0:
				return player->GetBonePos(8);
			case 1:
				return player->GetBonePos(4);
			case 2:
				return player->GetBonePos(73);
			default:
				return {};
			}
		}
		Player* ClosestPlayer()
		{
			Player* ClosestPlayer = nullptr;
			QAngle cpDelta;

			for (int i = 1; i < Player::GetMaxPlayer(); i++)
			{
				Player* currentPlayer = Player::GetPlayer(i);
				if (!currentPlayer->IsValidPlayer())
					continue;
				if (Options::bVisCheck)
					if (!currentPlayer->IsVisible())
						continue;
				QAngle currViewAngles = *viewAngles;
				if (ClosestPlayer == nullptr)
				{
					ClosestPlayer = currentPlayer;
					QAngle angle = LocalPlayer::Get()->GetAnglesFromVector(&currentPlayer->GetBonePos(8));
					cpDelta = currViewAngles - angle;
					Clamp(cpDelta);
					continue;
				}
				QAngle angle = LocalPlayer::Get()->GetAnglesFromVector(&currentPlayer->GetBonePos(8));
				QAngle delta = currViewAngles - angle;
				Clamp(delta);

				double deltaLength = sqrt(pow(delta.y, 2) + pow(delta.x, 2));
				double cpDeltaLength = sqrt(pow(cpDelta.y, 2) + pow(cpDelta.x, 2));

				if (deltaLength < cpDeltaLength)
				{
					ClosestPlayer = currentPlayer;
					cpDelta = delta;
				}
			}
			return ClosestPlayer;
		}
		void Run()
		{
			if (!(GetKeyState(VK_LBUTTON) >> 15))
				return;

			if(engineModule == 0)
				engineModule = (uintptr_t)GetModuleHandleA("engine.dll");
			if(viewAngles == nullptr)
				viewAngles = (QAngle*)(*(uintptr_t*)(engineModule + Offsets::Signatures::dwClientState) + Offsets::Signatures::dwClientState_ViewAngles);

			LocalPlayer* localPlayer = LocalPlayer::Get();
			if (!localPlayer->IsValid()) //Check if our player is in game and valid
				return;

			Player* currentPlayer = ClosestPlayer(); //Get closest player to crosshair
			if (!currentPlayer->IsValidPlayer()) 
				return;
			
			Vector3 vecAim = GetBoneVec(currentPlayer);
			VelcotiyExtrapolate(currentPlayer, vecAim);
			QAngle angleToEnemy = localPlayer->GetAnglesFromVector(&vecAim);

			if (!InFov(angleToEnemy))
				return;

			AddRCSControl(angleToEnemy);
			SmoothAngle(angleToEnemy);
			Normalize(angleToEnemy);
			Clamp(angleToEnemy);
			AimAt(angleToEnemy);

		}

	}

}