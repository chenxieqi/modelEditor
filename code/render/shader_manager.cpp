//--------------------------------------------------------------------------------
// �V�F�[�_�[�}�l�[�W���[
//--------------------------------------------------------------------------------
#include "shader_manager.h"
#include "shader_default.h"
#include "shader_2d.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}
//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void ShaderManager::Init()
{
    m_shaders[SHADER_DEFAULT] = new DefaultShader;
    m_shaders[SHADER_DEFAULT]->Init();
    m_shaders[SHADER_2D] = new Shader2D;
    m_shaders[SHADER_2D]->Init();
}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void ShaderManager::Uninit()
{
    m_shaders[SHADER_DEFAULT]->Uninit();
    delete m_shaders[SHADER_DEFAULT];
    m_shaders[SHADER_2D]->Uninit();
    delete m_shaders[SHADER_2D];
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void ShaderManager::Update()
{

}
//--------------------------------------------------------------------------------
// �Z�b�g�V�F�[�_�[
//--------------------------------------------------------------------------------
void ShaderManager::Set(SHADER_TYPE& shader_type)
{
    m_shaders[shader_type]->Set();
}
//--------------------------------------------------------------------------------
// ���Z�b�g�V�F�[�_�[
//--------------------------------------------------------------------------------
void ShaderManager::Reset(SHADER_TYPE& shader_type)
{
    m_shaders[shader_type]->Reset();
}
//--------------------------------------------------------------------------------
// �Z�b�g�V�F�[�_�[�R���X�^���g�e�[�u��
//--------------------------------------------------------------------------------
void ShaderManager::SetConstantTable(SHADER_TYPE& shader_type, const Renderer& renderer)
{
    m_shaders[shader_type]->SetConstantTable(renderer);
}
//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
ShaderManager* ShaderManager::Create()
{
    ShaderManager* shadermanager = new ShaderManager;
    return shadermanager;
}