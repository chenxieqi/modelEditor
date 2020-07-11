//--------------------------------------------------------------------------------
// リジットボディー
//--------------------------------------------------------------------------------
#include "rigidbody.h"
#include "gameobject.h"
#include "transform.h"
#include "timer.h"
Rigidbody::~Rigidbody()
{
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void Rigidbody::Update()
{
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));

    //重力加速度
    m_acceleration += D3DXVECTOR3(0.0f,-9.8f,0.0f) * m_gravity_multiplier;

    //力から加速度を計算する
    m_acceleration += m_force_accum * m_inverse_mass;
    
    //速度
    m_velocity += m_acceleration * Timer::Instance().GetDeltaTime();
    
    //位置更新
    m_movement += m_velocity * Timer::Instance().GetDeltaTime();
    transform->SetPosition(transform->GetPosition() + m_movement);
}

void Rigidbody::LateUpdate()
{
    // 物理演算より位置補正
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));

    transform->SetPosition(transform->GetPosition() + m_fixed_movement);

    //処理完了
    m_velocity *= m_drag;
    m_movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fixed_movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_force_accum= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_acceleration= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}