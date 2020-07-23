#pragma once
#include "pch.h"
#include "Vectors.h"
#include "../Offsets.h"

namespace Math
{
	Vector2D GetScreenResolution()
	{
		HWND window = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		RECT screen;
		GetClientRect(window, &screen);
		return Vector2D(screen.right, screen.bottom);
	}
	bool WorldToScreen(IN const Vector3& pos, OUT Vector2D& result)
	{
		uintptr_t clientBase = (uintptr_t)GetModuleHandleA("client_panorama.dll");
		float* matrix = (float*)(clientBase + Offsets::Signatures::dwViewMatrix);

		Vector2D clipCoords;
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
		clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
		//clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
		float w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

		if (w < 0.1f)
			return false;

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		Vector2D NDC;
		NDC.x = clipCoords.x / w;
		NDC.y = clipCoords.y / w;
		//NDC.z = clipCoords.z / w;

		Vector2D resolution = GetScreenResolution();
		float width = resolution.x;
		float height = resolution.y;
		result.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
		result.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);
		return true;

	}

}