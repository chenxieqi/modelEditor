//--------------------------------------------------------------------------------
// �����_���[
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "game_system.h"
#include "mesh_manager.h"
#include "material_manager.h"


Renderer::~Renderer()
{
}
//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void Renderer::Init()
{

}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void Renderer::Uninit()
{
    auto& mesh_manager = GameSystem::Instance().GetMeshManager();
    mesh_manager.DisUse(m_mesh_name);
    auto& material_manager = GameSystem::Instance().GetMaterialManager();
    material_manager.DisUse(m_material_name);
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void Renderer::Update()
{

}
//--------------------------------------------------------------------------------
// �Z�b�g���b�V���l�[��
//--------------------------------------------------------------------------------
void Renderer::SetMeshName(const string& mesh_name) 
{
    auto& mesh_manager = GameSystem::Instance().GetMeshManager();
    mesh_manager.DisUse(m_mesh_name);
    m_mesh_name = mesh_name;
    mesh_manager.Use(m_mesh_name);
}
//--------------------------------------------------------------------------------
// �Z�b�g�}�e���A���l�[��
//--------------------------------------------------------------------------------
void Renderer::SetMaterialName(const string& material_name) 
{ 
    auto& material_manager = GameSystem::Instance().GetMaterialManager();
    material_manager.DisUse(m_material_name);
    m_material_name = material_name; 
    material_manager.Use(m_material_name);
}