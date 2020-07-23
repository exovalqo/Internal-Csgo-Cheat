#include "pch.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "Draw.h"
#include "Utils/Vectors.h"
#include <vector>

#pragma comment(lib, "d3dx9.lib")
#define PI 3.14159265

namespace Draw
{
    IDirect3DDevice9* pDevice = nullptr;
    IDirect3DVertexBuffer9* g_pVB = nullptr;
    IDirect3DIndexBuffer9* g_pIB = nullptr;
    ID3DXLine* pLine = nullptr;

    void Line(const Vector2D& pos1, const Vector2D& pos2, float thickness, D3DCOLOR color)
    {
        if (!pLine)
            D3DXCreateLine(pDevice, &pLine);
        
        D3DXVECTOR2 Line[2];
        Line[0] = D3DXVECTOR2(pos1.x, pos1.y);
        Line[1] = D3DXVECTOR2(pos2.x, pos2.y);
        pLine->SetWidth(thickness);
        pLine->Draw(Line, 2, color);
    }
    void Circle(int X, int Y, int radius, int numSides, float thickness, DWORD Color)
    {
        if (!pLine)
            D3DXCreateLine(pDevice, &pLine);

        D3DXVECTOR2 Line[128];
        float Step = PI * 2.0 / numSides;
        int Count = 0;
        for (float a = 0; a < PI * 2.0; a += Step)
        {
            float X1 = radius * cos(a) + X;
            float Y1 = radius * sin(a) + Y;
            float X2 = radius * cos(a + Step) + X;
            float Y2 = radius * sin(a + Step) + Y;
            Line[Count].x = X1;
            Line[Count].y = Y1;
            Line[Count + 1].x = X2;
            Line[Count + 1].y = Y2;
            Count += 2;
        }
        pLine->SetWidth(thickness);
        pLine->Begin();
        pLine->Draw(Line, Count, Color);
        pLine->End();
    }

    void BorderBox(float x, float y, float w, float h, float linewidth, D3DCOLOR outline, D3DCOLOR box)
    {
        if (w <= 1)
            return;
        Box(x, y, w, h, linewidth, outline);
        FilledBox(x+linewidth, y+linewidth, w-(2*linewidth), h-(2*linewidth), box);
    }
    void Box(float x, float y, float w, float h, float linewidth, D3DCOLOR color)
    {
        if (linewidth == 0 || linewidth == 1)
        {
            FilledBox(x, y, w, 1, color);             // Top
            FilledBox(x, y + h - 1, w, 1, color);         // Bottom
            FilledBox(x, y + 1, 1, h - 2 * 1, color);       // Left
            FilledBox(x + w - 1, y + 1, 1, h - 2 * 1, color);   // Right
        }
        else
        {
            FilledBox(x, y, w, linewidth, color);                                     // Top
            FilledBox(x, y + h - linewidth, w, linewidth, color);                         // Bottom
            FilledBox(x, y + linewidth, linewidth, h - 2 * linewidth, color);               // Left
            FilledBox(x + w - linewidth, y + linewidth, linewidth, h - 2 * linewidth, color);   // Right
        }
    }

    void FilledBox(float x, float y, float w, float h, D3DCOLOR color)
    {
        vertex V[4];

        V[0].color = V[1].color = V[2].color = V[3].color = color;

        V[0].z = V[1].z = V[2].z = V[3].z = 0;
        V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

        V[0].x = x;
        V[0].y = y;
        V[1].x = x + w;
        V[1].y = y;
        V[2].x = x + w;
        V[2].y = y + h;
        V[3].x = x;
        V[3].y = y + h;

        unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };
        
 
        pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
        pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

        VOID* pVertices;
        g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
        memcpy(pVertices, V, sizeof(V));
        g_pVB->Unlock();

        VOID* pIndex;
        g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
        memcpy(pIndex, indexes, sizeof(indexes));
        g_pIB->Unlock();

        pDevice->SetTexture(0, NULL);
        pDevice->SetPixelShader(NULL);
        pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
        pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        pDevice->SetIndices(g_pIB);

        pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

        g_pVB->Release();
        g_pIB->Release();
       
    }
}