#pragma once
#include "pch.h"
#include "Utils/Vectors.h"

namespace Draw
{
	struct vertex
	{
		FLOAT x, y, z, rhw;
		DWORD color;
	};

	extern IDirect3DDevice9* pDevice;
	extern IDirect3DVertexBuffer9* g_pVB;
	extern IDirect3DIndexBuffer9* g_pIB;
	extern ID3DXLine* pLine;
	void BorderBox(float x, float y, float w, float h, float linewidth, D3DCOLOR outline, D3DCOLOR box);
	void Circle(int X, int Y, int radius, int numSides, float thickness, DWORD Color);
	void FilledBox(float x, float y, float w, float h, D3DCOLOR color);
	void Box(float x, float y, float w, float h, float linewidth, D3DCOLOR color);
	void Line(const Vector2D& pos1, const Vector2D& pos2, float thickness, D3DCOLOR color);
};

