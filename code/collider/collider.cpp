//--------------------------------------------------------------------------------
// コライダー
//--------------------------------------------------------------------------------
#include "collider.h"
#include "collision_system.h"
#include "game_system.h"
#include "gameobject.h"
#include "transform.h"
#include <cassert>

Collider::~Collider()
{
}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void Collider::Init(void)
{
    GameSystem::Instance().GetCollisionSystem().Register(this);
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void Collider::Uninit(void)
{
    GameSystem::Instance().GetCollisionSystem().DeRegister(this);
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));
    m_world= m_offset * transform->GetMatrix();
}
//--------------------------------------------------------------------------------
// 衝突タイプ
//--------------------------------------------------------------------------------
void Collider::OnTrigger(Collider& collider)
{

}

void Collider::OnCollision(Collider& collider_1, Collider& collider_2, Collision& collision)
{

}
//--------------------------------------------------------------------------------
// 登録
//--------------------------------------------------------------------------------
void Collider::RegisterBehaviorListeners(Behavior* behavior)
{
    assert(behavior);
    m_behavior_listeners.push_back(behavior);
}
//--------------------------------------------------------------------------------
// 登録解除
//--------------------------------------------------------------------------------
void Collider::DeRegisterBehaviorListeners(Behavior* behavior)
{
    assert(behavior);
    m_behavior_listeners.remove(behavior);
}