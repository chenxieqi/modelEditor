//--------------------------------------------------------------------------------
// コリジョンシステム
//--------------------------------------------------------------------------------
#include "collision_system.h"
#include "collider.h"
#include "collider_sphere.h"
#include "collider_field.h"
#include "game_system.h"
#include "gameobject.h"
#include "physics_system.h"
#include "rigidbody.h"

const float CollisionSystem::MAX_SLOPE_COS = cosf(MAX_SLOPE);

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
bool CollisionSystem::Init()
{
    return true;
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CollisionSystem::Uninit()
{

}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CollisionSystem::Update()
{
    for (auto iterator_1 = m_collider_sphere[COLLIDER_DYNAMIC].begin(); iterator_1 != m_collider_sphere[COLLIDER_DYNAMIC].end(); ++iterator_1)
    {
        for (auto iterator_2 = iterator_1; iterator_2 != m_collider_sphere[COLLIDER_DYNAMIC].end(); ++iterator_2)
        {
            if ((*iterator_1)->GetGameObject() != (*iterator_2)->GetGameObject())
            {
                SphereDetect((*static_cast<ColliderSphere*>(*iterator_1)), (*static_cast<ColliderSphere*>(*iterator_2)));
            }
        }
        for (auto iterator_static = m_collider_sphere[COLLIDER_STATIC].begin(); iterator_static != m_collider_sphere[COLLIDER_STATIC].end(); ++iterator_static)
        {
            SphereDetect((*static_cast<ColliderSphere*>(*iterator_1)), (*static_cast<ColliderSphere*>(*iterator_static)));
        }

        SphereFieldDetect(*static_cast<ColliderSphere*>(*iterator_1));
    }
}
//--------------------------------------------------------------------------------
// スフィアあたり判定
//--------------------------------------------------------------------------------
void CollisionSystem::SphereDetect(ColliderSphere& sphere_1, ColliderSphere& sphere_2)
{
    Rigidbody* sphere_1_rigidbody = static_cast<Rigidbody*>(sphere_1.GetGameObject()->GetComponent("Rigidbody"));
    Rigidbody* sphere_2_rigidbody = static_cast<Rigidbody*>(sphere_2.GetGameObject()->GetComponent("Rigidbody"));

    if (sphere_1_rigidbody || sphere_2_rigidbody)
    {
        D3DXVECTOR3 sphere_1_position = D3DXVECTOR3(sphere_1.GetWorld()._41, sphere_1.GetWorld()._42, sphere_1.GetWorld()._43);
        D3DXVECTOR3 sphere_2_position = D3DXVECTOR3(sphere_2.GetWorld()._41, sphere_2.GetWorld()._42, sphere_2.GetWorld()._43);
        float distance_square = D3DXVec3LengthSq(&(sphere_2_position - sphere_1_position));
        float radius_square = (sphere_2.GetRadius() + sphere_1.GetRadius())*(sphere_2.GetRadius() + sphere_1.GetRadius());
        if (radius_square >= distance_square)
        {
            if (sphere_1.GetIsTrigger() || sphere_2.GetIsTrigger())
            {
                sphere_1.OnTrigger(sphere_2);
                sphere_2.OnTrigger(sphere_1);
            }
            else
            {
                auto& physics_system = GameSystem::Instance().GetPhysicsSystem();
                Collision* collision = new Collision;
                D3DXVECTOR3 direction= sphere_1_position - sphere_2_position;

                // 衝突点
                collision->m_point = sphere_2_position + direction*0.5f;
                float distance = sqrtf(distance_square);

                // 衝突深度
                collision->m_penetration = (sphere_2.GetRadius() + sphere_1.GetRadius()) - distance;

                // 衝突法線
                collision->m_normal = direction / distance;

                if (sphere_1_rigidbody==NULL)
                {
                    collision->m_rigidbody_1 = sphere_2_rigidbody;
                    collision->m_normal *= -1;
                }
                else {
                    collision->m_rigidbody_1 = sphere_1_rigidbody;
                    collision->m_rigidbody_2 = sphere_2_rigidbody;
                }
                physics_system.Register(collision);

                sphere_1.OnCollision(sphere_1,sphere_2,*collision);
                sphere_2.OnCollision(sphere_2,sphere_1,*collision);
            }
        }
    }
}

//--------------------------------------------------------------------------------
// スフィアと地面の当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::SphereFieldDetect(ColliderSphere& sphere)
{
    D3DXVECTOR3 sphere_position = D3DXVECTOR3(sphere.GetWorld()._41, sphere.GetWorld()._42, sphere.GetWorld()._43);
    
    float sphere_scale;
    sphere_scale = D3DXVec3Length(&D3DXVECTOR3(sphere.GetWorld()._11, sphere.GetWorld()._12, sphere.GetWorld()._13));

    float sphere_radius = sphere_scale * sphere.GetRadius();
    Collision* collision = Vec3FieldDetect(sphere_position);
    if (collision == NULL) { return; }
    float penetration = collision->m_penetration + sphere_radius;
    if (penetration <= 0) {
        delete collision;
        return;
    }
    if (sphere.GetIsTrigger())
    {
        sphere.OnTrigger(*m_collider_field);
        return;
    }

    collision->m_rigidbody_1 = static_cast<Rigidbody*>(sphere.GetGameObject()->GetComponent("Rigidbody"));
    if (collision->m_rigidbody_1 == NULL)
    {
        delete collision;
        return;
    }
    collision->m_rigidbody_2 = NULL;
    float dot = D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &collision->m_normal);// スフィアと地面の角度
    if (dot < MAX_SLOPE_COS)// 登れない
    {
        collision->m_penetration = D3DXVec3Dot(&collision->m_normal, &(collision->m_rigidbody_1->GetMovement()*(-1.0f)));
    }
    else {// 登れる
        collision->m_normal = D3DXVECTOR3(0, 1, 0);
        collision->m_penetration = penetration;
    }
    auto& physics_system = GameSystem::Instance().GetPhysicsSystem();
    physics_system.Register(collision);
    sphere.OnCollision(sphere,*m_collider_field ,*collision);
}
//--------------------------------------------------------------------------------
// 点と地面の当たり判定
//--------------------------------------------------------------------------------
Collision* CollisionSystem::Vec3FieldDetect(D3DXVECTOR3& vec3)
{
    PolygonInfo* polygon_info = static_cast<ColliderField*>(m_collider_field)->GetPolygon(vec3);
    if (polygon_info == NULL) { return NULL; }
    float point_y_on_field = polygon_info->side.y - ((vec3.x - polygon_info->side.x) * polygon_info->normal.x + (vec3.z - polygon_info->side.z) * polygon_info->normal.z) / polygon_info->normal.y;
    Collision* collision = new Collision;
    collision->m_point = D3DXVECTOR3(vec3.x, point_y_on_field, vec3.z);
    collision->m_normal = polygon_info->normal;
    collision->m_penetration = point_y_on_field - vec3.y;
    delete polygon_info;
    return collision;
}
//--------------------------------------------------------------------------------
// 登録
//--------------------------------------------------------------------------------
void CollisionSystem::Register(Collider* collider)
{
    if (collider->GetType() == COLLIDER_FIELD)
    {
        m_collider_field = collider;
    }
    else {
        auto rigidbody = static_cast<Rigidbody*>(collider->GetGameObject()->GetComponent("Rigidbody"));
        if (rigidbody == NULL)
        {
            m_collider_sphere[COLLIDER_STATIC].push_back(collider);
        }
        else {
            m_collider_sphere[COLLIDER_DYNAMIC].push_back(collider);
        }
    }
}
//--------------------------------------------------------------------------------
// 登録解除
//--------------------------------------------------------------------------------
void CollisionSystem::DeRegister(Collider* collider)
{
    if (collider->GetType() == COLLIDER_FIELD)
    {
        m_collider_field = NULL;
    }
    else {
        auto rigidbody = static_cast<Rigidbody*>(collider->GetGameObject()->GetComponent("Rigidbody"));
        if (rigidbody == NULL)
        {
            m_collider_sphere[COLLIDER_STATIC].remove(collider);
        }
        else {
            m_collider_sphere[COLLIDER_DYNAMIC].remove(collider);
        }
    }

}
//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
CollisionSystem* CollisionSystem::Create()
{
    CollisionSystem* collision = new CollisionSystem;
    return collision;
}