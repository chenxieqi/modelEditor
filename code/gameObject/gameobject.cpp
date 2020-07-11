//--------------------------------------------------------------------------------
// �I�u�W�F�N�g
//--------------------------------------------------------------------------------
#include "gameobject.h"
#include "game_system.h"
#include "gameobject_manager.h"
#include "component.h"
#include "transform.h"
#include <cassert>

GameObject::GameObject()
    :m_alive(true),
    m_active(true)
{
    AddComponent(new Transform(this));
}

GameObject::~GameObject()
{
}
//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void GameObject::Init()
{
    GameSystem::Instance().GetGameobjectManager().AddGameObject(this);
    for (auto component : m_component)
    {
        component.second->Init();
    }

    Transform* transform = static_cast<Transform*>(this->GetComponent("Transform"));
    for (auto& pair : transform->GetChild())
    {
        pair.second->GetGameObject()->Init();
    }
}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void GameObject::Uninit()
{
    for (auto component : m_component)
    {
        component.second->Uninit();
        delete component.second;
    }
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void GameObject::Update()
{
    if (!m_active) { return; }
    for (auto component : m_component)
    {
        component.second->Update();
    }
}

void GameObject::LateUpdate()
{
    if (!m_active) { return; }
    for (auto component : m_component)
    {
        component.second->LateUpdate();
    }
}
//--------------------------------------------------------------------------------
// �p�[�c�ǉ�
//--------------------------------------------------------------------------------
void GameObject::AddComponent(Component* component)
{
    assert(component);
    m_component.emplace(component->GetName(),component);
}
//--------------------------------------------------------------------------------
// �Z�b�g�A�N�e�B�u
//--------------------------------------------------------------------------------
void GameObject::SetActive(const bool& active)
{
    if (m_active == active) { return; }
    m_active = active;
    auto transform = static_cast<Transform*>(GetComponent("Transform"));
    for (auto& child : transform->GetChild())
    {
        child.second->GetGameObject()->SetActive(active);
    }
}
//--------------------------------------------------------------------------------
// �Z�b�g�A���C�u
//--------------------------------------------------------------------------------
void GameObject::SetAlive(const bool& alive) 
{ 
    m_alive = alive;
    auto transform = static_cast<Transform*>(GetComponent("Transform"));
    for (auto& child : transform->GetChild())
    {
        child.second->GetGameObject()->SetAlive(alive);
    }
}
//--------------------------------------------------------------------------------
// �Q�b�^�[
//--------------------------------------------------------------------------------
Component* GameObject::GetComponent(std::string name)
{ 
    auto iterator = m_component.find(name); 
    if (iterator == m_component.end()) { return NULL; }
    return m_component.find(name)->second; 
}