//--------------------------------------------------------------------------------
// デフォルトシェーダー
//--------------------------------------------------------------------------------
#include "shader_default.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "renderer.h"
#include "transform.h"
#include "game_system.h"
#include "render_system.h"
#include "camera.h"
#include "light_manager.h"
#include "light.h"
#include "gameobject.h"

DefaultShader::DefaultShader()
{
}

DefaultShader::~DefaultShader()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void DefaultShader::Init()
{
    CompileShaderFrom("default_vertex_shader", "default_pixel_shader");
}
//--------------------------------------------------------------------------------
// セット
//--------------------------------------------------------------------------------
void DefaultShader::Set()
{
    Shader::Set();
}
void DefaultShader::Reset()
{
    
}
//--------------------------------------------------------------------------------
// シェーダーコンスタントテーブル情報
//--------------------------------------------------------------------------------
void DefaultShader::SetConstantTable(const Renderer& renderer)
{
    auto& game_system = GameSystem::Instance();
    auto device = game_system.GetRenderSystem().GetD3DDevice();
    auto camera = game_system.GetCameraManager().GetMainCamera();
    auto light_direction = game_system.GetLightManager().GetMainLight()->GetDirection();
    auto material = game_system.GetMaterialManager().GetMaterial(renderer.GetMaterialName());   
    auto transform = static_cast<Transform*>(renderer.GetGameObject()->GetComponent("Transform"));
    auto word_matrix = transform->GetMatrix();
    
    D3DXMATRIX world_inverse;
    D3DXMatrixInverse(&world_inverse, nullptr, &word_matrix);

    D3DXMATRIX world_inverse_transpose;
    D3DXMatrixTranspose(&world_inverse_transpose, &world_inverse);

    D3DXMATRIX word_view_projection = word_matrix * camera->GetCameraViewMatrix()*camera->GetCameraProjectMatrix();
    D3DXVECTOR3 camera_position = camera->GetCameraAt();
    m_vs_const_table->SetValue(device, "world_view_projection", &word_view_projection, sizeof(word_view_projection));
    m_vs_const_table->SetMatrix(device, "world_inverse_transpose", &world_inverse_transpose);
    
    m_ps_const_table->SetValue(device, "light_direction_local", &light_direction, sizeof(light_direction));
    m_ps_const_table->SetValue(device, "camera_position_local", &camera_position, sizeof(camera_position));

    if (material)
    {
        auto texture = game_system.GetTextureManager().GetTexture(material->m_color_texture);
        UINT index = m_ps_const_table->GetSamplerIndex("color_texture");
        device->SetTexture(index, texture);
    }
}