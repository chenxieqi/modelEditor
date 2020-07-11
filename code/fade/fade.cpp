//--------------------------------------------------------------------------------
//  �t�F�C�h
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
// ������
//--------------------------------------------------------------------------------
void Fade::Init(void)
{

}
//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void Fade::Uninit(void)
{

}
//--------------------------------------------------------------------------------
// �X�V
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
// �t�F�C�h�C��
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
// �t�F�C�h�A�E�g
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
// ���̃��[�h�ɕϊ�
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
// �t�F�C�h�̃A���t�@�l�X�V
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
// ����
//--------------------------------------------------------------------------------
Fade* Fade::Create(void)
{
    Fade* fade = new Fade;
    return fade;
}