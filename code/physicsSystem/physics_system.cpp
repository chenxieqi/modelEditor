//--------------------------------------------------------------------------------
//  �����V�X�e��
//--------------------------------------------------------------------------------

#include "physics_system.h"
#include "collision_system.h"
#include "rigidbody.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
void PhysicsSystem::Init()
{

}
//--------------------------------------------------------------------------------
//  �I��
//--------------------------------------------------------------------------------
void PhysicsSystem::Uninit()
{

}
//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void PhysicsSystem::Update()
{
    while (!m_collisions.empty())
    {
        Resolve(*m_collisions.front());
        m_collisions.pop();
    }
}
//--------------------------------------------------------------------------------
//  �o�^
//--------------------------------------------------------------------------------
void PhysicsSystem::Register(Collision* collision)
{
    m_collisions.push(collision);
}
//--------------------------------------------------------------------------------
//  �N���A
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
    while (!m_collisions.empty())
    {
        delete m_collisions.front();
        m_collisions.pop();
    }
}
//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
PhysicsSystem* PhysicsSystem::Create()
{
    PhysicsSystem* physicssystem = new PhysicsSystem;
    return physicssystem;
}

//--------------------------------------------------------------------------------
// �Փ˖�����
//--------------------------------------------------------------------------------
void PhysicsSystem::Resolve(Collision& collision)
{
    ResolveVelocity(collision);
    ResolveInterpenetration(collision);
}
//--------------------------------------------------------------------------------
// �Փˑ��x�v�Z
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveVelocity(Collision& collision)
{
    float separating_velocity = CalculateSeparatingVelocity(collision);
    if (separating_velocity >= 0.0f)
    {
        return;
    }

    float bounciness = collision.m_rigidbody_1->GetBounciness();
    if (collision.m_rigidbody_2 != NULL)
    {
        bounciness = (collision.m_rigidbody_2->GetBounciness() + bounciness)*0.5f;
    }
    float bounciness_velocity = -separating_velocity * bounciness;
    
    // ���˕Ԃ葬�x�{�������x
    float delta_velocity = bounciness_velocity - separating_velocity;

    // �t����
    float inverse_mass = collision.m_rigidbody_1->GetMass();
    if (collision.m_rigidbody_2 != NULL)
    {
        inverse_mass += collision.m_rigidbody_2->GetMass();
    }
    if (inverse_mass <= 0.0f) { return; }

    // �Փ˗�
    float inpulse = delta_velocity / inverse_mass;
    
    // �t���ʂ̒P�ʏՓ˗�
    D3DXVECTOR3 vector3_inpulse = inpulse * collision.m_normal;

    // ���x�v�Z
    collision.m_rigidbody_1->AddVelocity(vector3_inpulse*collision.m_rigidbody_1->GetInverseMass());
    if (collision.m_rigidbody_2 != NULL)
    {
        collision.m_rigidbody_2->AddVelocity(vector3_inpulse*collision.m_rigidbody_2->GetInverseMass()*-1.0f);
    }
    
}
//--------------------------------------------------------------------------------
// �Փˈʒu����
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveInterpenetration(Collision& collision)
{
    if(collision.m_penetration<=0)
    {
        return;
    }
    float inverse_mass = collision.m_rigidbody_1->GetMass();
    if (collision.m_rigidbody_2 != NULL)
    {
        inverse_mass += collision.m_rigidbody_2->GetMass();
    }
    if (inverse_mass <= 0.0f) {
        return;
    }

    D3DXVECTOR3 movement_per_inverse_mass = collision.m_normal*collision.m_penetration / inverse_mass;
    collision.m_rigidbody_1->AddFixedMovement(movement_per_inverse_mass*collision.m_rigidbody_1->GetInverseMass());
    if (collision.m_rigidbody_2 != NULL)
    {
        collision.m_rigidbody_2->AddFixedMovement(movement_per_inverse_mass*collision.m_rigidbody_2->GetInverseMass()*-1.0f);
    }

}
//--------------------------------------------------------------------------------
// �Փ˕������x�v�Z
//--------------------------------------------------------------------------------
float PhysicsSystem::CalculateSeparatingVelocity(Collision& collision)
{
    D3DXVECTOR3 velocity = collision.m_rigidbody_1->GetVelocity();
    if (collision.m_rigidbody_2 != NULL)
    {
        velocity -= collision.m_rigidbody_2->GetVelocity();
    }
    float separating_velocity = D3DXVec3Dot(&velocity, &collision.m_normal);
    return separating_velocity;
}