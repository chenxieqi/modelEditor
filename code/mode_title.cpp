//--------------------------------------------------------------------------------
// タイトルモード
//--------------------------------------------------------------------------------
#include "mode_title.h"
#include "game_object_factory.h"
#include "light.h"
#include "camera.h"
#include "camera_edit.h"

void ModeTitle::Init()
{
   // GameObjectFactory::CreateSkyBox();
    GameObjectFactory::CreateField("demo");
    GameObjectFactory::CreateEdit();
    Light* light = new Light;
    CameraEdit* camera = new CameraEdit;
    camera->Init();
}
void ModeTitle::Uninit()
{

}
void ModeTitle::Update()
{

}