//--------------------------------------------------------------------------------
// カメラマネージャー
//--------------------------------------------------------------------------------

#include "camera_manager.h"
#include "camera.h"
#include "main.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CameraManager::Init()
{

}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CameraManager::Update()
{
    for (auto iterator : m_cameras)
    {
        iterator->Update();
    }

}
void CameraManager::LateUpdate()
{
    for (auto iterator : m_cameras)
    {
        iterator->Set();
    }
}
//--------------------------------------------------------------------------------
// リリース
//--------------------------------------------------------------------------------
void CameraManager::Release()
{
    Clear();
    delete this;
}
//--------------------------------------------------------------------------------
// クリア
//--------------------------------------------------------------------------------
void CameraManager::Clear()
{
    for (auto iterator : m_cameras)
    {
        iterator->Uninit();
        delete iterator;
    }
    m_cameras.clear();
}
//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
CameraManager* CameraManager::Create()
{
    CameraManager* camera_manager = new CameraManager;
    return camera_manager;
}