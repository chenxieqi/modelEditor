//--------------------------------------------------------------------------------
// �R���C�_�[
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "component.h"
#include <list>

using namespace std;

class Behavior;
class Collision;
//--------------------------------------------------------------------------------
// �R���C�_�[�^�C�v
//--------------------------------------------------------------------------------
enum COLLIDERTYPE
{
    COLLIDER_SPHERE=0,
    COLLIDER_FIELD,
    COLLIDER_MAX
};
//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class Collider:public Component
{
public:
    Collider(GameObject* game_object,std::string name,COLLIDERTYPE collider_type) :Component(game_object,name) 
    {
        m_istrigger = false;
        D3DXMatrixIdentity(&m_offset);
        D3DXMatrixIdentity(&m_world);
        m_type = collider_type;
    }
    ~Collider();
    //--------------------------------------------------------------------------------
    // ��{�֐�
    //--------------------------------------------------------------------------------
    void Init(void)override;
    void Uninit(void)override;
    void Update(void)override;
    //--------------------------------------------------------------------------------
    // �Փ˃^�C�v
    //--------------------------------------------------------------------------------
    void OnTrigger(Collider& collider);
    void OnCollision(Collider& collider_1, Collider& collider_2,Collision& collision);
    //--------------------------------------------------------------------------------
    // �Z�b�^�[
    //--------------------------------------------------------------------------------
    auto SetIsTrigger(const bool istrigger) { m_istrigger = istrigger; }
    auto SetOffset(const D3DXMATRIX& offset) { m_offset = offset; }
    //--------------------------------------------------------------------------------
    // �Q�b�^�[
    //--------------------------------------------------------------------------------
    auto& GetIsTrigger()const { return m_istrigger; }
    auto& GetOffset()   const { return m_offset; }
    auto& GetWorld()    const { return m_world; }
    auto& GetType()     const { return m_type; }
    //--------------------------------------------------------------------------------
    // �o�^
    //--------------------------------------------------------------------------------
    void RegisterBehaviorListeners(Behavior* behavior);
    //--------------------------------------------------------------------------------
    // �o�^����
    //--------------------------------------------------------------------------------
    void DeRegisterBehaviorListeners(Behavior* behavior);

protected:
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    bool            m_istrigger;
    D3DXMATRIX      m_offset;
    D3DXMATRIX      m_world;
    list<Behavior*> m_behavior_listeners;
    COLLIDERTYPE    m_type;
};

