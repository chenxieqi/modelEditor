//--------------------------------------------------------------------------------
// レンダラー
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "game_system.h"
#include "mesh_manager.h"
#include "material_manager.h"


Renderer::~Renderer()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void Renderer::Init()
{

}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void Renderer::Uninit()
{
    auto& mesh_manager = GameSystem::Instance().GetMeshManager();
    mesh_manager.DisUse(m_mesh_name);
    auto& material_manager = GameSystem::Instance().GetMaterialManager();
    material_manager.DisUse(m_material_name);
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void Renderer::Update()
{

}
//--------------------------------------------------------------------------------
// セットメッシュネーム
//--------------------------------------------------------------------------------
void Renderer::SetMeshName(const string& mesh_name) 
{
    auto& mesh_manager = GameSystem::Instance().GetMeshManager();
    mesh_manager.DisUse(m_mesh_name);
    m_mesh_name = mesh_name;
    mesh_manager.Use(m_mesh_name);
}
//--------------------------------------------------------------------------------
// セットマテリアルネーム
//--------------------------------------------------------------------------------
void Renderer::SetMaterialName(const string& material_name) 
{ 
    auto& material_manager = GameSystem::Instance().GetMaterialManager();
    material_manager.DisUse(m_material_name);
    m_material_name = material_name; 
    material_manager.Use(m_material_name);
}