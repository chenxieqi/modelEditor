//--------------------------------------------------------------------------------
// ÉÇÅ[Éh
//--------------------------------------------------------------------------------
#include "mode.h"
#include "game_system.h"
#include "gameobject_manager.h"
#include "camera_manager.h"
//--------------------------------------------------------------------------------
// ê∂ê¨
//--------------------------------------------------------------------------------
Mode* Mode::Create()
{
    Mode* mode = new Mode;
    return mode;
}
//--------------------------------------------------------------------------------
// èIóπ
//--------------------------------------------------------------------------------
void Mode::Uninit(void)
{
    auto& game_system = GameSystem::Instance();
    auto& game_object_manager = game_system.GetGameobjectManager();
    auto& camera_manager = game_system.GetCameraManager();
    game_object_manager.Clear();
    camera_manager.Clear();
}                