//--------------------------------------------------------------------------------
//  フェイド
//--------------------------------------------------------------------------------
#include "fade.h"
#include "timer.h"
#include "game_system.h"
#include "material_manager.h"

Fade::Fade():m_fade_status(FADE_NONE),m_mode(nullptr)
{
}


Fade::~Fade()
{
}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void Fade::Init(void)
{

}
//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void Fade::Uninit(void)
{

}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void Fade::Update(void)
{
    switch (m_fade_status)
    {
    case FADE_NONE:
        break;
    case FADE_IN:
        FadeIn();
        break;
    case FADE_OUT:
        FadeOut();
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------------
// フェイドイン
//--------------------------------------------------------------------------------
void Fade::FadeIn(void)
{
    if (m_time_count <= 0.0f)
    {
        m_time_count = 0.0f;
        m_fade_status = FADE_NONE;
    }
    m_time_count -= Timer::Instance().GetDeltaTime();
    UpdateAlpha("fade", m_time_count / m_fade_time);
}

//--------------------------------------------------------------------------------
// フェイドアウト
//--------------------------------------------------------------------------------
void Fade::FadeOut(void)
{
    if (m_time_count>=m_fade_time)
    {
        m_time_count = m_fade_time;
        m_fade_status = FADE_IN;
        GameSystem::Instance().ChangeMode(m_mode);
    }
    m_time_count += Timer::Instance().GetDeltaTime();
    UpdateAlpha("fade", m_time_count / m_fade_time);
}

//--------------------------------------------------------------------------------
// 次のモードに変換
//--------------------------------------------------------------------------------
void Fade::FadeTo(Mode* next_mode)
{
    if (m_fade_status == FADE_OUT)
    {
        delete next_mode;
        return;
    }
    m_fade_status = FADE_OUT;
    m_time_count = 0.0f;
    m_mode = next_mode;
}

//--------------------------------------------------------------------------------
// フェイドのアルファ値更新
//--------------------------------------------------------------------------------
void Fade::UpdateAlpha(const string& material_name, const float& alpha)
{
    auto material = GameSystem::Instance().GetMaterialManager().GetMaterial(material_name);
    if (material)
    {
        material->m_diffuse.a = alpha;
    }
}

//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
Fade* Fade::Create(void)
{
    Fade* fade = new Fade;
    return fade;
}