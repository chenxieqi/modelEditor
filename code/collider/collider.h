//--------------------------------------------------------------------------------
// コライダー
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "component.h"
#include <list>

using namespace std;

class Behavior;
class Collision;
//--------------------------------------------------------------------------------
// コライダータイプ
//--------------------------------------------------------------------------------
enum COLLIDERTYPE
{
    COLLIDER_SPHERE=0,
    COLLIDER_FIELD,
    COLLIDER_MAX
};
//--------------------------------------------------------------------------------
// クラス
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
    // 基本関数
    //--------------------------------------------------------------------------------
    void Init(void)override;
    void Uninit(void)override;
    void Update(void)override;
    //--------------------------------------------------------------------------------
    // 衝突タイプ
    //--------------------------------------------------------------------------------
    void OnTrigger(Collider& collider);
    void OnCollision(Collider& collider_1, Collider& collider_2,Collision& collision);
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    auto SetIsTrigger(const bool istrigger) { m_istrigger = istrigger; }
    auto SetOffset(const D3DXMATRIX& offset) { m_offset = offset; }
    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    auto& GetIsTrigger()const { return m_istrigger; }
    auto& GetOffset()   const { return m_offset; }
    auto& GetWorld()    const { return m_world; }
    auto& GetType()     const { return m_type; }
    //--------------------------------------------------------------------------------
    // 登録
    //--------------------------------------------------------------------------------
    void RegisterBehaviorListeners(Behavior* behavior);
    //--------------------------------------------------------------------------------
    // 登録解除
    //--------------------------------------------------------------------------------
    void DeRegisterBehaviorListeners(Behavior* behavior);

protected:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    bool            m_istrigger;
    D3DXMATRIX      m_offset;
    D3DXMATRIX      m_world;
    list<Behavior*> m_behavior_listeners;
    COLLIDERTYPE    m_type;
};

