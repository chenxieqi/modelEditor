//--------------------------------------------------------------------------------
//  ゲームシステム
//--------------------------------------------------------------------------------
#pragma once
#include <Windows.h>
//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class RenderSystem;
class CollisionSystem;
class InputKeyboard;
class InputMouse;
class PhysicsSystem;
class SoundSystem;
class MaterialManager;
class MeshManager;
class ShaderManager;
class TextureManager;
class GameobjectManager;
class CameraManager;
class LightManager;
class Mode;
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class GameSystem
{
public:
    GameSystem(HINSTANCE hInstance);
    ~GameSystem();
    //--------------------------------------------------------------------------------
    //  基本関数
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update();
    void Draw();
    //--------------------------------------------------------------------------------
    // モードを変える
    //--------------------------------------------------------------------------------
    void ChangeMode(Mode* mode);
    //--------------------------------------------------------------------------------
    // ウインドウ関数
    //--------------------------------------------------------------------------------
    void InitWindow();
    LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    static GameSystem& Instance() { return *m_gamesystem; }
    auto& GetRenderSystem(void)const { return *m_render_system; }
    auto& GetCollisionSystem(void)const { return *m_collision_system; }
    auto& GetKeyboardSystem(void)const { return *m_keyboard_system; }
    auto& GetMouseSystem(void)const { return *m_mouse_system; }
    auto& GetPhysicsSystem(void)const { return *m_physics_system; }
    auto& GetSoundSystem(void)const { return *m_sound_system; }
    auto& GetMaterialManager(void)const { return *m_material_manager; }
    auto& GetMeshManager(void)const { return *m_mesh_manager; }
    auto& GetShaderManager(void)const { return *m_shader_manager; }
    auto& GetTextureManager(void)const { return *m_texture_manager; }
    auto& GetGameobjectManager(void)const { return *m_game_object_manager; }
    auto& GetCameraManager(void)const { return *m_camera_manager; }
    auto& GetLightManager(void)const { return *m_light_manager; }
    auto& GetMode(void)const { return *m_mode; }
private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    char m_class_name[5]= "game";
    char m_window_name[7] = "Island";
    HINSTANCE m_hinstance;
    HWND      m_hWnd;

    static GameSystem*  m_gamesystem;

    RenderSystem*       m_render_system;
    CollisionSystem*    m_collision_system;
    InputKeyboard*      m_keyboard_system;
    InputMouse*         m_mouse_system;
    PhysicsSystem*      m_physics_system;
    SoundSystem*        m_sound_system;
    MaterialManager*    m_material_manager;
    MeshManager*        m_mesh_manager;
    ShaderManager*      m_shader_manager;
    TextureManager*     m_texture_manager;
    GameobjectManager*  m_game_object_manager;
    CameraManager*      m_camera_manager;
    LightManager*       m_light_manager;
    Mode*               m_mode;
};