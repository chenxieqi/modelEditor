//--------------------------------------------------------------------------------
//  物理システム
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
//  初期化
//--------------------------------------------------------------------------------
void PhysicsSystem::Init()
{

}
//--------------------------------------------------------------------------------
//  終了
//--------------------------------------------------------------------------------
void PhysicsSystem::Uninit()
{

}
//--------------------------------------------------------------------------------
//  更新
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
//  登録
//--------------------------------------------------------------------------------
void PhysicsSystem::Register(Collision* collision)
{
    m_collisions.push(collision);
}
//--------------------------------------------------------------------------------
//  クリア
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
//  生成
//--------------------------------------------------------------------------------
PhysicsSystem* PhysicsSystem::Create()
{
    PhysicsSystem* physicssystem = new PhysicsSystem;
    return physicssystem;
}

//--------------------------------------------------------------------------------
// 衝突問題解決
//--------------------------------------------------------------------------------
void PhysicsSystem::Resolve(Collision& collision)
{
    ResolveVelocity(collision);
    ResolveInterpenetration(collision);
}
//--------------------------------------------------------------------------------
// 衝突速度計算
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
    
    // 跳ね返り速度＋分離速度
    float delta_velocity = bounciness_velocity - separating_velocity;

    // 逆質量
    float inverse_mass = collision.m_rigidbody_1->GetMass();
    if (collision.m_rigidbody_2 != NULL)
    {
        inverse_mass += collision.m_rigidbody_2->GetMass();
    }
    if (inverse_mass <= 0.0f) { return; }

    // 衝突力
    float inpulse = delta_velocity / inverse_mass;
    
    // 逆質量の単位衝突力
    D3DXVECTOR3 vector3_inpulse = inpulse * collision.m_normal;

    // 速度計算
    collision.m_rigidbody_1->AddVelocity(vector3_inpulse*collision.m_rigidbody_1->GetInverseMass());
    if (collision.m_rigidbody_2 != NULL)
    {
        collision.m_rigidbody_2->AddVelocity(vector3_inpulse*collision.m_rigidbody_2->GetInverseMass()*-1.0f);
    }
    
}
//--------------------------------------------------------------------------------
// 衝突位置解決
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
// 衝突分離速度計算
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