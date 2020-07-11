//--------------------------------------------------------------------------------
// �V�F�[�_�[
//--------------------------------------------------------------------------------
#include "shader.h"
#include "game_system.h"
#include "render_system.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void Shader::Uninit()
{
    SAFE_RELEASE(m_vertex_shader);
    SAFE_RELEASE(m_vs_const_table);
    SAFE_RELEASE(m_pixel_shader);
    SAFE_RELEASE(m_ps_const_table);
}
//--------------------------------------------------------------------------------
// �Z�b�g
//--------------------------------------------------------------------------------
void Shader::Set()
{
    auto device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    device->SetVertexShader(m_vertex_shader);
    device->SetPixelShader(m_pixel_shader);
}
//--------------------------------------------------------------------------------
// �R���p�C��
//--------------------------------------------------------------------------------
void Shader::CompileShaderFrom(const string& vertex_Shader, const string& pixel_shader)
{
    string file_path = "data/shader/" + vertex_Shader + ".hlsl";
    // Vertex Shader
    LPD3DXBUFFER errorBuffer = nullptr;
    LPD3DXBUFFER codeBuffer = nullptr;
    HRESULT hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
        NULL,
        NULL,
        "main",	// FunctionName�G���g���[�֐���
        "vs_3_0", // �V�F�[�_�[���f��
        0,
        &codeBuffer, // �o�C�i���R�[�h
        &errorBuffer, // �R���p�C���������ς񂾂�null�A���s������G���[���b�Z�[�W�������Ă�
        &m_vs_const_table);

    if (FAILED(hr))
    {
        MessageBox(NULL, (char*)errorBuffer->GetBufferPointer(), "�G���[", MB_OK | MB_ICONWARNING);
        errorBuffer->Release();
    }

    auto device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    hr = device->CreateVertexShader(
        (LPDWORD)codeBuffer->GetBufferPointer(),
        &m_vertex_shader // �V�F�[�_�[�Ǘ����Ă���Ƃ���
    );

    if (codeBuffer) codeBuffer->Release(); codeBuffer = nullptr;
    if (errorBuffer) errorBuffer->Release(); errorBuffer = nullptr;

    file_path = "data/shader/" + pixel_shader + ".hlsl";
    // Pixel Shader
    hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
        NULL,
        NULL,
        "main",	// FunctionName�G���g���[�֐���
        "ps_3_0", // �V�F�[�_�[���f��
        0,
        &codeBuffer, // �o�C�i���R�[�h
        &errorBuffer, // �R���p�C���������ς񂾂�null�A���s������G���[���b�Z�[�W�������Ă�
        &m_ps_const_table);

    if (FAILED(hr))
    {
        MessageBox(NULL, (char*)errorBuffer->GetBufferPointer(), "�G���[", MB_OK | MB_ICONWARNING);
        errorBuffer->Release();
    }

    hr = device->CreatePixelShader(
        (LPDWORD)codeBuffer->GetBufferPointer(),
        &m_pixel_shader // �V�F�[�_�[�Ǘ����Ă���Ƃ���
    );

    if (codeBuffer) codeBuffer->Release();
    if (errorBuffer) errorBuffer->Release();
}