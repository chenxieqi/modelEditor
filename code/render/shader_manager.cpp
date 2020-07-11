//--------------------------------------------------------------------------------
// シェーダーマネージャー
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
// 初期化
//--------------------------------------------------------------------------------
void ShaderManager::Init()
{
    m_shaders[SHADER_DEFAULT] = new DefaultShader;
    m_shaders[SHADER_DEFAULT]->Init();
    m_shaders[SHADER_2D] = new Shader2D;
    m_shaders[SHADER_2D]->Init();
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void ShaderManager::Uninit()
{
    m_shaders[SHADER_DEFAULT]->Uninit();
    delete m_shaders[SHADER_DEFAULT];
    m_shaders[SHADER_2D]->Uninit();
    delete m_shaders[SHADER_2D];
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void ShaderManager::Update()
{

}
//--------------------------------------------------------------------------------
// セットシェーダー
//--------------------------------------------------------------------------------
void ShaderManager::Set(SHADER_TYPE& shader_type)
{
    m_shaders[shader_type]->Set();
}
//--------------------------------------------------------------------------------
// リセットシェーダー
//--------------------------------------------------------------------------------
void ShaderManager::Reset(SHADER_TYPE& shader_type)
{
    m_shaders[shader_type]->Reset();
}
//--------------------------------------------------------------------------------
// セットシェーダーコンスタントテーブル
//--------------------------------------------------------------------------------
void ShaderManager::SetConstantTable(SHADER_TYPE& shader_type, const Renderer& renderer)
{
    m_shaders[shader_type]->SetConstantTable(renderer);
}
//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
ShaderManager* ShaderManager::Create()
{
    ShaderManager* shadermanager = new ShaderManager;
    return shadermanager;
}