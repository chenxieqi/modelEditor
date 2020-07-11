//--------------------------------------------------------------------------------
// レンダラー
//--------------------------------------------------------------------------------
#pragma once

#include <string>

#include "component.h"
#include "shader_manager.h"

class RenderSystem;

using namespace std;
//--------------------------------------------------------------------------------
// 構造体
//--------------------------------------------------------------------------------
enum RENDERER_TYPE
{
    RENDERER2D=0,
    RENDERER3D
};

enum RENDERER_PRIORITY
{
    PRIORITY_DEFAULT=0,
    PEIORITY_ALPHATEST,
    PRIORITY_2D,
    PRIORITY_MAX
};
//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class Renderer:public Component
{
public:
    Renderer(GameObject* game_object, std::string name) :Component(game_object, name) {}
    ~Renderer();
    //--------------------------------------------------------------------------------
    // 基本関数
    //--------------------------------------------------------------------------------
    virtual void Init()override;
    virtual void Uninit()override;
    virtual void Update()override;
    //--------------------------------------------------------------------------------
    // レンダーバイ
    //--------------------------------------------------------------------------------
    virtual void RenderBy(const RenderSystem& render_system)=0;
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    void SetMeshName(const string& mesh_name);
    void SetMaterialName(const string& material_name);
    void SetRendererPriority(const RENDERER_PRIORITY& priority) { m_priority = priority; }
    void SetRendererType(const RENDERER_TYPE& type) { m_type = type; }
    void SetShaderType(const SHADER_TYPE& shader_type) { m_shader_type = shader_type; }
    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    auto& GetMeshName(void)const { return m_mesh_name; }
    auto& GetMaterialName(void)const { return m_material_name; }
    auto& GetRendererPriority(void)const { return m_priority; }
    auto& GetType(void)const { return m_type; }
    auto& GetShaderType(void)const { return m_shader_type; }

protected:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    string              m_mesh_name;
    string              m_material_name;
    RENDERER_PRIORITY   m_priority;
    RENDERER_TYPE       m_type;
    SHADER_TYPE         m_shader_type;
};

