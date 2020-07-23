#pragma once

#include "Utils/QAngle.h"
#include "Utils/GetFunction.h"

class IVEngineClient
{
public:
	IVEngineClient() {}
	void GetScreenSize(int& width, int& height)
	{
		return call_virtual<5, void>(this, width, height);
	}// 6

		// Get the entity index of the local player
	int	GetLocalPlayer(void)
	{
		return call_virtual<11, int>(this);
	}//12

	// Copy current view orientation into va
	void GetViewAngles(QAngle& va)
	{
		return call_virtual<18, void>(this, va);
	}//19

	// Set current view orientation from va
	void SetViewAngles(QAngle& va)
	{
		return call_virtual<19, void>(this, va);
	}//20

	// Get access to the world to screen transformation matrix
	const VMatrix& WorldToScreenMatrix()
	{
		return call_virtual<35, VMatrix&>(this);
	}//36
};
