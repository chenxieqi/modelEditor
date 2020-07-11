//--------------------------------------------------------------------------------
// ゲームオブジェクトファクトリー
//--------------------------------------------------------------------------------
#include "game_object_factory.h"
#include "game_system.h"
#include "material_manager.h"
#include "gameobject.h"
#include "component.h"
#include "transform.h"
#include "rigidbody.h"
#include "render3D.h"
#include "render2D.h"
#include "collider.h"
#include "collider_sphere.h"
#include "collider_field.h"
#include "behavior.h"
#include "behavior_edit.h"
#include "behavior_field_edit.h"
#include "behavior_model_edit.h"
#include "mesh_manager.h"
#include <fstream>
#include <cassert>
//--------------------------------------------------------------------------------
// フィールド生成
//--------------------------------------------------------------------------------
void GameObjectFactory::CreateField(const string& name)
{
    GameObject* game_object = new GameObject;
    ColliderField* collider_field = new ColliderField(game_object, name);
    game_object->AddComponent(collider_field);
    game_object->Init();

}
//--------------------------------------------------------------------------------
// スカイボックス生成
//--------------------------------------------------------------------------------
void GameObjectFactory::CreateSkyBox(void)
{
    GameObject* game_object = new GameObject;
    game_object->SetName("Barrel");
    Render3D* render3D = new Render3D(game_object);
    game_object->AddComponent(render3D);
    GameSystem::Instance().GetMaterialManager().Use("Barrel","Barrel.jpg", "Barrel.jpg");
    render3D->SetMaterialName("Barrel");
    render3D->SetMeshName("Barrel_0.mesh");
    render3D->SetShaderType(SHADER_DEFAULT);
    Transform* transform = static_cast<Transform*>(game_object->GetComponent("Transform"));
    transform->SetPosition(0.0f, -1.0f, -2.0f);
    transform->SetScaling(1.0f, 1.0f, 1.0f);
    game_object->Init();
}
//--------------------------------------------------------------------------------
// エディター生成
//--------------------------------------------------------------------------------
void GameObjectFactory::CreateEdit(void)
{
    GameObject* game_object = new GameObject;
    BehaviorEdit* edit = new BehaviorEdit(game_object);
    BehaviorFieldEdit* field_edit = new BehaviorFieldEdit(game_object);
    BehaviorModelEdit* model_edit = new BehaviorModelEdit(game_object);
    edit->SetFieldEdit(field_edit);
    edit->SetModelEdit(model_edit);
    game_object->AddComponent(edit);
    game_object->AddComponent(model_edit);
    game_object->AddComponent(field_edit);
    game_object->Init();

    Render3D* render3D = new Render3D(game_object);
    render3D->SetMeshName("FieldEdit");
    GameSystem::Instance().GetMaterialManager().Use("Grass", "field.jpg", "field.jpg");
    render3D->SetMaterialName("Grass");
    render3D->SetShaderType(SHADER_DEFAULT);
    game_object->AddComponent(render3D);
    render3D->Init();
}
//--------------------------------------------------------------------------------
// モデル生成
//--------------------------------------------------------------------------------
GameObject* GameObjectFactory::CreateModel(const string& name, const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scaling)
{
    string path = "data/model/" + name ;
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return NULL;
    }

    BinaryInputArchive archive(file);

    auto result = CreateChild(nullptr, archive);

    result->SetName(name);

    Transform* transform = static_cast<Transform*>(result->GetComponent("Transform"));
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScaling(scaling);

    //Rigidbody* rigidbody = new Rigidbody(result);
    //result->AddComponent(rigidbody);

    file.close();

    result->Init();
    return result;
}
//--------------------------------------------------------------------------------
// 子供生成
//--------------------------------------------------------------------------------
GameObject* GameObjectFactory::CreateChild(Transform* parent, BinaryInputArchive& archive)
{
    auto result = new GameObject;

    int name_size;
    archive.loadBinary(&name_size, sizeof(name_size));
    string name;
    name.resize(name_size);
    archive.loadBinary(&name[0], name_size);
    result->SetName(string(name.begin(), name.end()));

    // offset
    Transform* transform=static_cast<Transform*>(result->GetComponent("Transform"));
    D3DXVECTOR3 position;
    D3DXQUATERNION rotation;
    D3DXVECTOR3 scaling;
    archive.loadBinary(&position, sizeof(position));
    archive.loadBinary(&rotation, sizeof(rotation));
    archive.loadBinary(&scaling, sizeof(scaling));
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScaling(scaling);
    if (parent)
    {
        transform->SetParent(parent);

    }

    int collider_number = 0;
    archive.loadBinary(&collider_number, sizeof(collider_number));

    for (int count = 0; count < collider_number; count++)
    {
        int collider_type = 0;
        D3DXVECTOR3 collider_position;
        D3DXVECTOR3 collider_rotation;
        D3DXVECTOR3 collider_scaling;
        bool istrigger;
        archive.loadBinary(&collider_type, sizeof(collider_type));
        archive.loadBinary(&collider_position, sizeof(collider_position));
        archive.loadBinary(&collider_rotation, sizeof(collider_rotation));
        archive.loadBinary(&collider_scaling, sizeof(collider_scaling));

        D3DXMATRIX collider_offset_mtx, collider_rotation_mtx, collider_scaling_mtx;
        D3DXMatrixTranslation(&collider_offset_mtx, collider_position.x, collider_position.y, collider_position.z);
        D3DXMatrixRotationYawPitchRoll(&collider_rotation_mtx, collider_rotation.y, collider_rotation.x, collider_rotation.z);
        D3DXMatrixScaling(&collider_scaling_mtx, collider_scaling.x, collider_scaling.y, collider_scaling.z);
        collider_offset_mtx = collider_offset_mtx * collider_rotation_mtx*collider_scaling_mtx;

        archive.loadBinary(&istrigger, sizeof(istrigger));
        Collider* collider=NULL;
        switch (collider_type) {
        case COLLIDER_SPHERE:
            collider = new ColliderSphere(result);
            break;
        default:
            break;
        }
        if (collider == NULL) { continue; }
        collider->SetIsTrigger(istrigger);
        collider->SetOffset(collider_offset_mtx);
        result->AddComponent(collider);
    }

    //Mesh
    int mesh_number = 0;
    archive.loadBinary(&mesh_number, sizeof(mesh_number));
    for (int count = 0; count < mesh_number; count++)
    {
        // Mesh Name
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        string mesh_name = string(name.begin(), name.end());

        // Material Name
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        string material_name = string(name.begin(), name.end());

        // Render Priority
        RENDERER_PRIORITY render_priority;
        archive.loadBinary(&render_priority, sizeof(render_priority));

        // Shader Type
        SHADER_TYPE shader_type;
        archive.loadBinary(&shader_type, sizeof(shader_type));

        //CastShadow
        bool cast_shadow;
        archive.loadBinary(&cast_shadow, sizeof(cast_shadow));

        //BoundingSpherePosition
        D3DXVECTOR3 bounding_sphere_position;
        float bounding_sphere_radius;
        archive.loadBinary(&bounding_sphere_position, sizeof(bounding_sphere_position));
        archive.loadBinary(&bounding_sphere_radius, sizeof(bounding_sphere_radius));

        // Mesh Type
        MeshType mesh_type;
        archive.loadBinary(&mesh_type, sizeof(mesh_type));

        mesh_type = TYPE_3DMESH;
        if (mesh_type == TYPE_3DMESH)
        {
            auto renderer = new Render3D(result);
            renderer->SetMeshName(mesh_name+".mesh");
            renderer->SetMaterialName(material_name);
            renderer->SetRendererPriority(render_priority);
            renderer->SetShaderType(shader_type);
            result->AddComponent(renderer);
        }
        //else if (mesh_type == TYPE_3DSKIN)
        //{
        //}
        else {
            MessageBox(NULL, "Error Mesh Type", "GameObjectFactory::CreateChild", MB_OK | MB_ICONWARNING);
        }

    }

    // Child
    int child_number;
    archive.loadBinary(&child_number, sizeof(child_number));
    for (int count = 0; count < child_number; count++)
    {
        auto child = CreateChild(transform, archive);
    }
    return result;
    
}