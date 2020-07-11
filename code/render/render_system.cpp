//--------------------------------------------------------------------------------
// レンダラーシステム
//--------------------------------------------------------------------------------
#include "render_system.h"
#include "game_system.h"
#include "mesh_manager.h"
#include "shader_manager.h"
#include "library\ImGui\imgui_impl_dx9.h"


RenderSystem::RenderSystem():
    m_D3D(nullptr), 
    m_D3DDevice(nullptr),
    m_vertex_declaration_2d(nullptr),
    m_vertex_declaration_3d(nullptr),
    m_background_color(D3DCOLOR_RGBA(0,200,255,255))
{

}

RenderSystem::~RenderSystem()
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------

void RenderSystem::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE    d3ddm;

    //Direct3Dインターフェースの作成
    m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

    //現在のディスプレイモードを
    m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
    //device presentation paramate
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferCount = 1;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.Windowed = bWindow;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    //デバイス作成制御
    m_D3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &m_D3DDevice            //出来たデバイスのアドレス
    );

    m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          // αブレンドを行う

    m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // αソースカラーの指定
    m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // αデスティネーションカラーの指定

    // ポリゴンとテクスチャをどうブレンドするか
    m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//ARG0 ---left
    m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//ARG1 ---right

    // テクスチャのピクセルに関する
    m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);  //U方向ー横 テクスチャアドレス(0-1)以外どう参照する
    m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //V方向ー縦
    m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  //フィルタリング（実際のサイズより小さくなったとき
    m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  //大きくなった時
    m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);  //MIPマップ

    InitVertexDeclaration();

    ImGui_ImplDX9_Init(hWnd, m_D3DDevice);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void RenderSystem::Uninit()
{
    ImGui_ImplDX9_Shutdown();

    if (m_D3DDevice != NULL)
    {//デバイスの解放
        m_D3DDevice->Release();
        m_D3DDevice = NULL;
    }

    if (m_D3D != NULL)
    {
        m_D3D->Release();
        m_D3D = NULL;
    }

}

//--------------------------------------------------------------------------------
//  描画
//--------------------------------------------------------------------------------
void RenderSystem::DrawBegin()
{
    //clear関数
    m_D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),m_background_color, 1.0f, 0);
    m_D3DDevice->BeginScene();
}

void RenderSystem::Draw()
{

    auto& shader_manager = GameSystem::Instance().GetShaderManager();
    for (int count_priority = 0; count_priority < static_cast<int>(PRIORITY_MAX); count_priority++)
    {
        for (int count_shader = 0; count_shader < static_cast<int>(SHADER_MAX); count_shader++)
        {
            SHADER_TYPE shader_type = static_cast<SHADER_TYPE>(count_shader);
            shader_manager.Set(shader_type);
            while (!m_renderers[count_priority][count_shader].empty())
            {
                auto iterator = m_renderers[count_priority][count_shader].front();
                m_renderers[count_priority][count_shader].pop();
                shader_manager.SetConstantTable(shader_type, *iterator);
                iterator->RenderBy(*this);
            }
            shader_manager.Reset(shader_type);
        }
    }
}

void RenderSystem::DrawEnd()
{
    m_D3DDevice->EndScene();
    //バックバッファとフロントバッファの入れ替え
    //pSourceRect            転送元矩形
    //pDestRect                転送先矩形
    //hDestWindowOverride    転送先ウインドウへのpointer
    //pDirtyRegion            通常はNULLで
    m_D3DDevice->Present(NULL, NULL, NULL, NULL);
}


//--------------------------------------------------------------------------------
//  描画処理(2D)
//--------------------------------------------------------------------------------
void RenderSystem::Render2dMesh(const string& mesh_name)const
{
    auto mesh = GameSystem::Instance().GetMeshManager().GetMesh(mesh_name);
    if (!mesh) { return; }

    m_D3DDevice->SetVertexDeclaration(m_vertex_declaration_2d);
    m_D3DDevice->SetStreamSource(0, mesh->vertex_buffer,0, sizeof(VERTEX_2D));
    m_D3DDevice->SetIndices(mesh->index_buffer);
    m_D3DDevice->DrawIndexedPrimitive(static_cast<_D3DPRIMITIVETYPE>(mesh->primitive_type), 0, 0, mesh->vertex_number, 0, mesh->polygon_number);

}

//--------------------------------------------------------------------------------
//  描画処理(3D)
//--------------------------------------------------------------------------------
void RenderSystem::Render3dMesh(const string& mesh_name)const
{
    auto mesh = GameSystem::Instance().GetMeshManager().GetMesh(mesh_name);
    if (!mesh) { return; }

    m_D3DDevice->SetVertexDeclaration(m_vertex_declaration_3d);
    m_D3DDevice->SetStreamSource(0, mesh->vertex_buffer, 0, sizeof(VERTEX_3D));
    m_D3DDevice->SetIndices(mesh->index_buffer);
    m_D3DDevice->DrawIndexedPrimitive(mesh->primitive_type, 0, 0, mesh->vertex_number, 0, mesh->polygon_number);

}


//--------------------------------------------------------------------------------
//  バーテックスデクラレーション初期化
//--------------------------------------------------------------------------------
void RenderSystem::InitVertexDeclaration(void)
{
    // 2D
    D3DVERTEXELEMENT9 element_2d[] =
    {
        {0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_POSITION,0},
        {0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_TEXCOORD,0},
        {0,20,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_COLOR,0 },
        D3DDECL_END()
    };
    m_D3DDevice->CreateVertexDeclaration(element_2d, &m_vertex_declaration_2d);

    // 3D
    D3DVERTEXELEMENT9 element_3d[] =
    {
        { 0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_POSITION,0 },
        { 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_NORMAL,0 },
        { 0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_TEXCOORD,0 },
        { 0,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_COLOR,0 },
         D3DDECL_END()
    };
    m_D3DDevice->CreateVertexDeclaration(element_3d, &m_vertex_declaration_3d);
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
RenderSystem* RenderSystem::Create(void)
{
    RenderSystem* rendersystem = new RenderSystem;
    return rendersystem;
}

//--------------------------------------------------------------------------------
//  登録
//--------------------------------------------------------------------------------
void RenderSystem::Register(Renderer* renderer)
{
    auto priority = renderer->GetRendererPriority();
    auto shader_type = renderer->GetShaderType();
    m_renderers[priority][shader_type].push(renderer);
}