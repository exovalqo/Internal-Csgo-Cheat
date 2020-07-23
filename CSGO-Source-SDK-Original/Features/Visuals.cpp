#include "pch.h"
#include "Features.h"
#include "../Utils/Vectors.h"
#include "../Utils/Player.h"
#include "../Utils/Localplayer.h"
#include "../Draw.h"
#include "../Utils/Math.h"
#include "../Utils/QAngle.h"
#include "../imgui/Options.h"

#pragma region COLORS
#define ABS(x) (x<0) ? (-x) : (x)
#define GREY(alpha) D3DCOLOR_ARGB(alpha, 94, 86, 86)
#define LIGHTRED(alpha) D3DCOLOR_ARGB(alpha, 226, 43, 43)
#define BLACK(alpha) D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define LIGHTGREEN(alpha) D3DCOLOR_ARGB(alpha, 32, 199, 38)
#define RED(alpha) D3DCOLOR_ARGB(alpha, 186, 36, 36)
#define GREEN(alpha) D3DCOLOR_ARGB(alpha, 34, 139, 34)
#pragma endregion

#define PI 3.141592

namespace Features
{
	namespace Visuals
	{
        void DrawHealthBar(float width, float height, const Vector2D& origin, const Vector2D& headPos, Player* player)
        {
            float healthPercent = player->GetHealth() / 100.0f;
            float halfWidth = width / 2;
            float healthEnd = headPos.x - halfWidth + (healthPercent * width); //End Of Current Health //Multiplcation to normalize within range of width
            float hbEnd = headPos.x + halfWidth; //End Of Actually Bar 
            float hbHeight = height/10;
            float hbY = headPos.y - hbHeight;
            float thickness = hbHeight/8;

            //Vector2D barEnd = { healthBarEnd, healthBarY };
            Vector2D barStart = { headPos.x - halfWidth, hbY };
            Vector2D emptyStart = { healthEnd, hbY };
            //Vector2D emptyEnd = { headPos.x + halfWidth, hbY };

            //Draw::Box(barStart.x, healthBarY, width, healthBarHeight, thickness, BLACK(150));
            //Draw::FilledBox(barStart.x-thickness, healthBarY-thickness, width-(2*thickness), healthBarHeight-(2*thickness), LIGHTGREEN(150));

            Draw::BorderBox(barStart.x, barStart.y, healthEnd - barStart.x, hbHeight, thickness, BLACK(150), LIGHTGREEN(150));
            
            Draw::BorderBox(emptyStart.x, emptyStart.y, hbEnd - healthEnd, hbHeight, thickness, BLACK(150), BLACK(150));
            
            //Draw::Line(barStart, barEnd, healthBarHeight, LIGHTGREEN(100));
            //Draw::Line(emptyStart, emptyEnd, healthBarHeight, BLACK(100));
        }
        void DrawSnapline(const Vector2D& screenCoordinates)
        {
            Vector2D screen = Math::GetScreenResolution();
            Vector2D bottom = { screen.x / 2, screen.y }; //Bottom middle of screen

            Draw::Line(bottom, screenCoordinates, 1.5, RED(230));

        }
        void DrawBox(float width, float height, const Vector2D &origin, const Vector2D& headPos , bool IsVisible)
        {
            /*Vector2 bl, br, tr, tl;
            bl = Vector2(origin.x - width, origin.y);
            br = Vector2(origin.x + width, origin.y);
            tr = Vector2(headPos.x + width, headPos.y);*/

            Vector2D tl = Vector2D(headPos.x - width/2, headPos.y);
            float lineWidth = height / 65;

            if (IsVisible)
               Draw::BorderBox(tl.x, tl.y, width, height, lineWidth, GREEN(230), GREY(90));
            else
               Draw::BorderBox(tl.x, tl.y, width, height, lineWidth, RED(230), GREY(90));

             //Draw::Box(tl.x, tl.y, width, height, 1, RED(230));
            //Draw::FilledBox(tl.x + 2, tl.y + 2, width-2, height-2, GREY(75));
        }
        void DrawFovCircle()
        {
            static LocalPlayer* lp = LocalPlayer::Get();
            Vector2D screen = Math::GetScreenResolution();
            float width = screen.x; float height = screen.y;
            float radAimbotFov = Options::fAimbotFov * (PI / 180.0f); 
            float radViewFov = (106.3 /2.0f) *(PI / 180.0f);
            float tanAim = tan(radAimbotFov);
            float tanView = tan(radViewFov);
            float radius = tanAim / tanView * (width/2.f);
            Draw::Circle(width/2, height/2 , radius, 20, 2, BLACK(220));
        }
        void Draw()
        {
            if (Options::bAimbotFov) DrawFovCircle();
            for (int i = 1; i < Player::GetMaxPlayer(); i++)
            {
                bool IsVisible = false;
                Player* currentPlayer = Player::GetPlayer(i);
                if (!currentPlayer->IsValidPlayer())
                    continue;
                if (Options::bVisCheck) //Check If Visible
                {
                    if (currentPlayer->IsVisible())
                        IsVisible = true;
                }

                Vector2D origin2D;
                if (!Math::WorldToScreen(currentPlayer->GetOrigin(), origin2D))
                {
                    continue;
                }
                Vector2D headPos2D;
                if(!Math::WorldToScreen(currentPlayer->GetBonePos(8), headPos2D))
                {
                    continue;
                }

                float diff = (headPos2D.y - origin2D.y);
                float enemyHeight = ABS(diff);
                float enemyWidth = enemyHeight / 2;

                float additonalHeight = enemyHeight / 8;
                headPos2D.y -= additonalHeight; //Get Box Above Head
                enemyHeight += additonalHeight;

                if(Options::bBoxEsp) DrawBox(enemyWidth, enemyHeight, origin2D, headPos2D ,IsVisible);
                if (Options::bSnapline) DrawSnapline(origin2D);
                if (Options::bHealthBar) DrawHealthBar(enemyWidth, enemyHeight, origin2D, headPos2D, currentPlayer);
            }
            
        }
	}
}

