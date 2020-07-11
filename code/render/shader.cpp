//--------------------------------------------------------------------------------
// シェーダー
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
// 終了
//--------------------------------------------------------------------------------
void Shader::Uninit()
{
    SAFE_RELEASE(m_vertex_shader);
    SAFE_RELEASE(m_vs_const_table);
    SAFE_RELEASE(m_pixel_shader);
    SAFE_RELEASE(m_ps_const_table);
}
//--------------------------------------------------------------------------------
// セット
//--------------------------------------------------------------------------------
void Shader::Set()
{
    auto device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    device->SetVertexShader(m_vertex_shader);
    device->SetPixelShader(m_pixel_shader);
}
//--------------------------------------------------------------------------------
// コンパイル
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
        "main",	// FunctionNameエントリー関数名
        "vs_3_0", // シェーダーモデル
        0,
        &codeBuffer, // バイナリコード
        &errorBuffer, // コンパイルが無事済んだらnull、失敗したらエラーメッセージが入ってる
        &m_vs_const_table);

    if (FAILED(hr))
    {
        MessageBox(NULL, (char*)errorBuffer->GetBufferPointer(), "エラー", MB_OK | MB_ICONWARNING);
        errorBuffer->Release();
    }

    auto device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    hr = device->CreateVertexShader(
        (LPDWORD)codeBuffer->GetBufferPointer(),
        &m_vertex_shader // シェーダー管理してくるところ
    );

    if (codeBuffer) codeBuffer->Release(); codeBuffer = nullptr;
    if (errorBuffer) errorBuffer->Release(); errorBuffer = nullptr;

    file_path = "data/shader/" + pixel_shader + ".hlsl";
    // Pixel Shader
    hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
        NULL,
        NULL,
        "main",	// FunctionNameエントリー関数名
        "ps_3_0", // シェーダーモデル
        0,
        &codeBuffer, // バイナリコード
        &errorBuffer, // コンパイルが無事済んだらnull、失敗したらエラーメッセージが入ってる
        &m_ps_const_table);

    if (FAILED(hr))
    {
        MessageBox(NULL, (char*)errorBuffer->GetBufferPointer(), "エラー", MB_OK | MB_ICONWARNING);
        errorBuffer->Release();
    }

    hr = device->CreatePixelShader(
        (LPDWORD)codeBuffer->GetBufferPointer(),
        &m_pixel_shader // シェーダー管理してくるところ
    );

    if (codeBuffer) codeBuffer->Release();
    if (errorBuffer) errorBuffer->Release();
}