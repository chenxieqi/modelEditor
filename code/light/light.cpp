//--------------------------------------------------------------------------------
// ƒ‰ƒCƒg
//--------------------------------------------------------------------------------
#include "light.h"
#include "game_system.h"
#include "light_manager.h"

Light::Light() :m_ambient(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f)),
m_diffuse(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f)),
m_direction(D3DXVECTOR3(1.0f, -1.0f, 0.0f))
{
    GameSystem::Instance().GetLightManager().Regiester(this);
};

Light::~Light()
{
}