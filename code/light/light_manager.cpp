//--------------------------------------------------------------------------------
// ライトマネージャー
//--------------------------------------------------------------------------------
#include "light_manager.h"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::Init()
{

}
void LightManager::Uninit()
{

}
//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
LightManager* LightManager::Create()
{
    LightManager* light_manager = new LightManager;
    return light_manager;
}
