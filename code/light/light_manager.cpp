//--------------------------------------------------------------------------------
// ���C�g�}�l�[�W���[
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
// ����
//--------------------------------------------------------------------------------
LightManager* LightManager::Create()
{
    LightManager* light_manager = new LightManager;
    return light_manager;
}
