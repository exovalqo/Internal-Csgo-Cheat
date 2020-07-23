#include "Options.h"

namespace Options
{
    bool bAnyVisualFeaturesEnabled = false;
    bool bAnyAimbotFeaturesEnabled = false;
    bool bShowMenu = false;
    bool bVisuals = false;
    bool bAimbot = false;
    bool bSnapline = false;
    bool bHealthBar = false;
    bool bBoxEsp = false;
    bool bSmooth = false;
    float fAimbotFov = 5.f;
    float fSmoothAmount = .5f;
    bool bAimbotFov = false;
    float fRCSSmooth = 0.5f;
    bool bEndSceneUnhooked = false;
    float smoothMultiplier = 200.f;
    const char* bones[] = { "Head", "Body", "Legs" };
    int currBone = 0;
    bool bVisCheck = true;
}
