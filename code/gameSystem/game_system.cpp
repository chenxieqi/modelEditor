//--------------------------------------------------------------------------------
// ゲームシステム
//--------------------------------------------------------------------------------

#include <Windows.h>
#include "game_system.h"
#include "collision_system.h"
#include "game_system.h"
#include "gameobject.h"
#include "input_system.h"
#include "main.h"
#include "material_manager.h"
#include "mesh_manager.h"
#include "physics_system.h"
#include "render_system.h"
#include "shader_manager.h"
#include "sound_system.h"
#include "texture_manager.h"
#include "gameobject_manager.h"
#include "camera_manager.h"
#include "light_manager.h"
#include "mode.h"
#include "mode_title.h"
#include "library\ImGui\imgui_impl_dx9.h"

GameSystem *GameSystem::m_gamesystem = NULL;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------
// メッセージ
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return GameSystem::Instance().MsgProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
GameSystem::GameSystem(HINSTANCE hInstance)
{
    if (m_gamesystem == NULL)
    {
        m_gamesystem = this;
    }
    else {
        delete this;
    }
    m_hinstance = hInstance;
}
//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
GameSystem::~GameSystem()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void GameSystem::Init()
{
    InitWindow();

    m_render_system = RenderSystem::Create();
    m_render_system->Init(m_hinstance,m_hWnd,true);

    m_collision_system = CollisionSystem::Create();
    m_collision_system->Init();

    m_physics_system = PhysicsSystem::Create();
    m_physics_system->Init();

    m_keyboard_system = InputKeyboard::Create();
    m_keyboard_system->Init(m_hinstance,m_hWnd);

    m_mouse_system = InputMouse::Create();
    m_mouse_system->Init(m_hinstance, m_hWnd);

    m_texture_manager = TextureManager::Create();
    m_texture_manager->Init();

    m_material_manager = MaterialManager::Create();
    m_material_manager->Init();

    m_mesh_manager = MeshManager::Create();
    m_mesh_manager->Init();

    m_shader_manager = ShaderManager::Create();
    m_shader_manager->Init();

    m_sound_system = SoundSystem::Create();
    m_sound_system->Init();

    m_game_object_manager = GameobjectManager::Create();
    m_game_object_manager->Init();

    m_camera_manager = CameraManager::Create();
    m_camera_manager->Init();

    m_light_manager = LightManager::Create();
    m_light_manager->Init();


    ChangeMode(new ModeTitle);

}
//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void GameSystem::Uninit()
{

    m_game_object_manager->Uninit();
    delete m_game_object_manager;

    m_collision_system->Uninit();
    delete m_collision_system;

    m_physics_system->Uninit();
    delete m_physics_system;

    m_keyboard_system->Uninit();
    delete m_keyboard_system;

    m_mouse_system->Uninit();
    delete m_mouse_system;

    m_material_manager->Uninit();
    delete m_material_manager;

    m_mesh_manager->Uninit();
    delete m_mesh_manager;

    m_shader_manager->Uninit();
    delete m_shader_manager;

    m_sound_system->Uninit();
    delete m_sound_system;

    m_texture_manager->Uninit();
    delete m_texture_manager;

    m_render_system->Uninit();
    delete m_render_system;

    m_camera_manager->Release();

    m_light_manager->Uninit();
    delete m_light_manager;

    m_mode->Uninit();
    delete m_mode;
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void GameSystem::Update()
{
    ImGui_ImplDX9_NewFrame();
    m_keyboard_system->Update();
    m_mouse_system->Update();
    m_game_object_manager->Update();
    m_mode->Update();
    m_camera_manager->Update();
    m_collision_system->Update();
    m_physics_system->Update();
    m_game_object_manager->LateUpdate();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void GameSystem::Draw()
{
    m_camera_manager->LateUpdate();
    m_render_system->DrawBegin();
    m_render_system->Draw();
    ImGui::Render();
    m_render_system->DrawEnd();
}

//--------------------------------------------------------------------------------
// モードを変える
//--------------------------------------------------------------------------------
void GameSystem::ChangeMode(Mode* mode)
{
    SAFE_RELEASE(m_mode);
    m_mode = mode;
    if (m_mode)
    {
        m_mode->Init();
    }
}


//--------------------------------------------------------------------------------
// ウインドウ初期化
//--------------------------------------------------------------------------------
void GameSystem::InitWindow()
{
    //    構造体
    WNDCLASSEX wcex = {
        sizeof(WNDCLASSEX),
        CS_VREDRAW | CS_HREDRAW,
        WndProc,
        0,
        0,
        m_hinstance,
        NULL,
        LoadCursor(NULL,IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        m_class_name,
        NULL,
    };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_VREDRAW | CS_HREDRAW;

    //ウインドウクラスの登録
    RegisterClassEx(&wcex);

    RECT wr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW^WS_MAXIMIZEBOX^WS_THICKFRAME, FALSE);
    int windowWidth = wr.right - wr.left;
    int windowHeight = wr.bottom - wr.top;

    RECT dr;
    GetWindowRect(GetDesktopWindow(), &dr);

    HWND hWnd = CreateWindowEx(
        0,
        m_class_name,
        m_window_name,
        WS_OVERLAPPEDWINDOW^WS_MAXIMIZEBOX^WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowWidth,
        windowHeight,
        NULL,
        NULL,
        m_hinstance,
        NULL
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    m_hWnd = hWnd;

}

//--------------------------------------------------------------------------------
// メッセージ処理
//--------------------------------------------------------------------------------
LRESULT GameSystem::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam);

    int nID;
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);    //WM_QUITメッセージの送信(もしセーブしない場合出る)
        break;
    case WM_KEYDOWN://何のキーが押された
        switch (wParam)
        {
        case VK_ESCAPE://「ESC」が押されたら
            nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);
            if (nID == IDYES)
            {
                DestroyWindow(hWnd);    //ウィンドウを破壊する
            }
            break;
        }
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
