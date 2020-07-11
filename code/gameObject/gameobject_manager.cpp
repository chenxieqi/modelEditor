//--------------------------------------------------------------------------------
//  ゲームオブジェクトマネージャー
//--------------------------------------------------------------------------------

#include "gameobject_manager.h"
#include "gameobject.h"

#include <cassert>



GameobjectManager::GameobjectManager()
{
}

GameobjectManager::~GameobjectManager()
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void GameobjectManager::Init()
{

}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void GameobjectManager::Uninit()
{
    Clear();
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void GameobjectManager::Update()
{
    for (auto iterator = m_gameobject.begin(); iterator != m_gameobject.end();)
    {
        if (iterator->second->IsAlive() == false)
        {
            iterator->second->Uninit();
            delete iterator->second;
            iterator = m_gameobject.erase(iterator);
        }
        else {
            iterator++;
        }
    }

    for (auto gameobject : m_gameobject)
    {
        gameobject.second->Update();
    }
}

void GameobjectManager::LateUpdate()
{
    for (auto gameobject : m_gameobject)
    {
        gameobject.second->LateUpdate();
    }
}

void GameobjectManager::AddGameObject(GameObject* gameobject)
{
    assert(gameobject);
    m_gameobject.emplace(gameobject->GetName(), gameobject);

}

//--------------------------------------------------------------------------------
//  クリア
//--------------------------------------------------------------------------------
void GameobjectManager::Clear()
{

    for (auto gameobject : m_gameobject)
    {
        gameobject.second->Uninit();
        delete gameobject.second;
    }
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
GameobjectManager* GameobjectManager::Create()
{
    GameobjectManager* gameobject_manager = new GameobjectManager;
    return gameobject_manager;
}