//--------------------------------------------------------------------------------
// ���W�b�g�{�f�B�[
//--------------------------------------------------------------------------------
#include "rigidbody.h"
#include "gameobject.h"
#include "transform.h"
#include "timer.h"
Rigidbody::~Rigidbody()
{
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void Rigidbody::Update()
{
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));

    //�d�͉����x
    m_acceleration += D3DXVECTOR3(0.0f,-9.8f,0.0f) * m_gravity_multiplier;

    //�͂�������x���v�Z����
    m_acceleration += m_force_accum * m_inverse_mass;
    
    //���x
    m_velocity += m_acceleration * Timer::Instance().GetDeltaTime();
    
    //�ʒu�X�V
    m_movement += m_velocity * Timer::Instance().GetDeltaTime();
    transform->SetPosition(transform->GetPosition() + m_movement);
}

void Rigidbody::LateUpdate()
{
    // �������Z���ʒu�␳
    auto transform = static_cast<Transform*>(m_game_object->GetComponent("Transform"));

    transform->SetPosition(transform->GetPosition() + m_fixed_movement);

    //��������
    m_velocity *= m_drag;
    m_movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fixed_movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_force_accum= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_acceleration= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}