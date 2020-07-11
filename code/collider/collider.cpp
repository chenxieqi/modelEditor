//--------------------------------------------------------------------------------
// �R���C�_�[
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
// ������
//--------------------------------------------------------------------------------
void Collider::Init(void)
{
    GameSystem::Instance().GetCollisionSystem().Register(this);
}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void Collider::Uninit(void)
{
    GameSystem::Instance().GetCollisionSystem().DeRegister(this);
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));
    m_world= m_offset * transform->GetMatrix();
}
//--------------------------------------------------------------------------------
// �Փ˃^�C�v
//--------------------------------------------------------------------------------
void Collider::OnTrigger(Collider& collider)
{

}

void Collider::OnCollision(Collider& collider_1, Collider& collider_2, Collision& collision)
{

}
//--------------------------------------------------------------------------------
// �o�^
//--------------------------------------------------------------------------------
void Collider::RegisterBehaviorListeners(Behavior* behavior)
{
    assert(behavior);
    m_behavior_listeners.push_back(behavior);
}
//--------------------------------------------------------------------------------
// �o�^����
//--------------------------------------------------------------------------------
void Collider::DeRegisterBehaviorListeners(Behavior* behavior)
{
    assert(behavior);
    m_behavior_listeners.remove(behavior);
}