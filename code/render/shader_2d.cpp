//--------------------------------------------------------------------------------
// ２Dシェーダー
//--------------------------------------------------------------------------------
#include "shader_2d.h"
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

Shader2D::Shader2D()
{
}

Shader2D::~Shader2D()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void Shader2D::Init()
{
    CompileShaderFrom("vertex_shader_2d", "pixel_shader_2d");
}
//--------------------------------------------------------------------------------
// セット
//--------------------------------------------------------------------------------
void Shader2D::Set()
{
    Shader::Set();
}
void Shader2D::Reset()
{
    
}
//--------------------------------------------------------------------------------
// シェーダーコンスタントテーブル情報
//--------------------------------------------------------------------------------
void Shader2D::SetConstantTable(const Renderer& renderer)
{
    auto& game_system = GameSystem::Instance();
    auto device = game_system.GetRenderSystem().GetD3DDevice();
    auto material = game_system.GetMaterialManager().GetMaterial(renderer.GetMaterialName());   
    auto transform = static_cast<Transform*>(renderer.GetGameObject()->GetComponent("Transform"));
    auto& word_matrix = transform->GetMatrix();

    D3DMATRIX projection = {
        2.0f / static_cast<float>(SCREEN_WIDTH), 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f / static_cast<float>(SCREEN_HEIGHT), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f };
    D3DXVECTOR2 offset(static_cast<float>(SCREEN_WIDTH) * 0.5f,static_cast<float>(SCREEN_HEIGHT) * 0.5f);
    m_vs_const_table->SetValue(device, "word", &word_matrix, sizeof(word_matrix));
    m_vs_const_table->SetValue(device, "projection", &projection, sizeof(projection));
    m_vs_const_table->SetValue(device, "offset", &offset, sizeof(offset));

   
    if (material)
    {
        m_ps_const_table->SetValue(device, "material_diffuse", &material->m_diffuse, sizeof(material->m_diffuse));
        auto texture = game_system.GetTextureManager().GetTexture(material->m_color_texture);
        UINT index = m_ps_const_table->GetSamplerIndex("color_texture");
        device->SetTexture(index, texture);
    }
}