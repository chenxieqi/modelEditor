//--------------------------------------------------------------------------------
// リジットボディー
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "component.h"
//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class Rigidbody:public Component 
{
public:
    Rigidbody(GameObject* game_object) :Component(game_object,"Rigidbody"),
    m_mass(1.0f),
    m_inverse_mass(1.0f),
    m_drag(0.95f),
    m_friction(0.0f),
    m_bounciness(0.5f),
    m_gravity_multiplier(5.0f),
    m_movement(D3DXVECTOR3(0.0f,0.0f,0.0f)),
    m_fixed_movement(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_velocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_acceleration(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_force_accum(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
    {}
    ~Rigidbody();
    //--------------------------------------------------------------------------------
    // 更新
    //--------------------------------------------------------------------------------
    void Update()override;
    void LateUpdate()override;
    //--------------------------------------------------------------------------------
    // 計算
    //--------------------------------------------------------------------------------
    void AddVelocity(D3DXVECTOR3 velocity) { m_velocity += velocity; }
    void AddFixedMovement(D3DXVECTOR3 fixed_movement) { m_fixed_movement += fixed_movement; }

    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    auto& GetMass(void)const { return m_mass; }
    auto& GetInverseMass(void)const { return m_inverse_mass; }
    auto& GetDrag(void)const { return m_drag; }
    auto& GetFriction(void)const { return m_friction; }
    auto& GetBounciness(void)const { return m_bounciness; }
    auto& GetVelocity(void)const { return m_velocity; }
    auto& GetAcceleration(void)const { return m_acceleration; }
    auto& GetMovement(void)const { return m_movement; }
    auto& GetGravityMultiplier(void)const { return m_gravity_multiplier; }
    
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    void SetMass(const float& mass) { m_mass = mass; }
    void SetDrag(const float& drag) { m_drag = drag; }
    void SetFriction(const float& friction) { m_friction = friction; }
    void SetBounciness(const float& bounciness) { m_bounciness = bounciness; }
    void SetGravityMultiplier(const float& gravity_multiplier) { m_gravity_multiplier = gravity_multiplier; }
    void SetVelocity(const D3DXVECTOR3& velocity) { m_velocity = velocity; }
    void SetMovement(const D3DXVECTOR3& movement) { m_movement = movement; }


private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    float       m_mass;                    // 質量
    float       m_inverse_mass;            // 質量分の一
    float       m_drag;                    // 抵抗係数
    float       m_friction;                // 摩擦係数
    float       m_bounciness;              // 跳ね返り係数
    float       m_gravity_multiplier;      // 重力係数
    D3DXVECTOR3 m_movement;                // 移動量
    D3DXVECTOR3 m_fixed_movement;          // 物理演算による補正
    D3DXVECTOR3 m_velocity;                // 速度
    D3DXVECTOR3 m_acceleration;            // 加速度
    D3DXVECTOR3 m_force_accum;             // 合わせた作用力
};

