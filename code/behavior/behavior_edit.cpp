//--------------------------------------------------------------------------------
// エディター
//--------------------------------------------------------------------------------
#include "behavior_edit.h"
#include "library\ImGui\imgui_impl_dx9.h"
#include "library\cereal\archives\binary.hpp"
#include <fstream>

#include "game_system.h"
#include "render_system.h"
#include "behavior_field_edit.h"
#include "behavior_model_edit.h"
#include "input_system.h"
#include "camera_manager.h"
#include "camera_edit.h"
#include "game_object_factory.h"
#include "gameobject.h"
#include "transform.h"

using namespace cereal;

BehaviorEdit::~BehaviorEdit()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void BehaviorEdit::Init() 
{
    m_player = GameObjectFactory::CreateModel("Kaduki.model", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 5.0f));
    m_stage_name.resize(MAX_PATH);
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void BehaviorEdit::Uninit() 
{

}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void BehaviorEdit::Update() 
{
    auto color = GameSystem::Instance().GetRenderSystem().GetBackgrounColor();
    bool filed_show = m_field_edit->GetShow();
    bool model_show = m_model_edit->GetShow();
    if (!ImGui::Begin("Edit"))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    if (ImGui::ColorEdit3("background color", (float*)&color))
    {
        GameSystem::Instance().GetRenderSystem().SetBackgroundColor(color);
    }
    if (ImGui::Button("Field Edit"))
    {
        filed_show = 1 ^ filed_show;
        m_field_edit->SetShow(filed_show);
    }
    if (ImGui::Button("Model Edit"))
    {
        model_show = 1 ^ model_show;
        m_model_edit->SetShow(model_show);
    }
    if (ImGui::Button("Camera Edit"))
    {
        m_show_camera_window = 1 ^ m_show_camera_window;
    }

    ImGui::InputText("Stage Name", &m_stage_name[0], MAX_PATH);

    if (ImGui::Button("Save"))
    {
        Save();
    }
    if (ImGui::Button("Load"))
    {
        Load();
    }

    ShowPositionWindow();

    ImGui::End();
    ShowCameraWindow();

}
void BehaviorEdit::LateUpdate() 
{

}
//--------------------------------------------------------------------------------
// セーブ
//--------------------------------------------------------------------------------
void BehaviorEdit::Save(void)
{
    auto size = m_stage_name.find_first_of('\0');
    string name = string(m_stage_name.begin(), m_stage_name.begin() + size);
    m_field_edit->SaveAsBinary(name);
    m_model_edit->SaveAsBinary(name);
    SaveAsBinary(name);
}
//--------------------------------------------------------------------------------
// ロード
//--------------------------------------------------------------------------------
void BehaviorEdit::Load(void)
{
    UINT id = MessageBox(NULL, "今のステージを破棄して新しいステージを読み込むますか？（笑）", "ok", MB_YESNO | MB_ICONWARNING);
    if (id != IDYES) { return; }
    auto size = m_stage_name.find_first_of('\0');
    string name = string(m_stage_name.begin(), m_stage_name.begin() + size);
    m_field_edit->LoadFrom(name);
    m_model_edit->LoadFrom(name);
    LoadFrom(name);
}
//--------------------------------------------------------------------------------
// プレイヤー情報セーブ
//--------------------------------------------------------------------------------
void BehaviorEdit::SaveAsBinary(const string& name)
{
    string path = "data/stage/" + name + ".player";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .player file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    // save position
    auto transform = static_cast<Transform*>(m_player->GetComponent("Transform"));
    D3DXVECTOR3 position = transform->GetPosition();
    archive.saveBinary(&position, sizeof(position));

    file.close();
    MessageBox(NULL, ".player file save complete", path.c_str(), MB_OK | MB_ICONWARNING);
}
//--------------------------------------------------------------------------------
// プレイヤー情報ロード
//--------------------------------------------------------------------------------
void BehaviorEdit::LoadFrom(const string& name)
{
    string path = "data/stage/" + name + ".player";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .player file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    // save position
    D3DXVECTOR3 position;
    archive.loadBinary(&position, sizeof(position));
    auto transform = static_cast<Transform*>(m_player->GetComponent("Transform"));
    transform->SetPosition(position);


    file.close();
    MessageBox(NULL, ".player file load complete", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
// カメラウィンドウ
//--------------------------------------------------------------------------------
void BehaviorEdit::ShowCameraWindow(void)
{
    if (!m_show_camera_window) { return; }
    if (!ImGui::Begin("Camera Edit",&m_show_camera_window))
    {
        ImGui::End();
        return;
    }
    auto camera =static_cast<CameraEdit*> (GameSystem::Instance().GetCameraManager().GetMainCamera());
    ImGui::InputFloat("Rotation Speed", &camera->m_rotation_speed);
    ImGui::InputFloat("Zoom Speed", &camera->m_zoom_speed);
    ImGui::InputFloat("Distance Min", &camera->m_distance_min);
    ImGui::InputFloat("Distance Max", &camera->m_distance_max);
    ImGui::End();

}
//--------------------------------------------------------------------------------
// 位置調整ウィンドウ
//--------------------------------------------------------------------------------
void BehaviorEdit::ShowPositionWindow(void)
{
    D3DXVECTOR3 previous_position = m_position;
    auto camera = static_cast<CameraEdit*> (GameSystem::Instance().GetCameraManager().GetMainCamera());
    auto& input = GameSystem::Instance().GetKeyboardSystem();

    D3DXVECTOR3 camera_forward = D3DXVECTOR3(camera->GetCameraForward().x, 0.0f, camera->GetCameraForward().z);
    D3DXVec3Normalize(&camera_forward, &camera_forward);
    D3DXVECTOR2 axis;
    axis.x = -static_cast<float>(input.GetKeyPress(DIK_A)) + static_cast<float>(input.GetKeyPress(DIK_D));
    axis.y = -static_cast<float>(input.GetKeyPress(DIK_S)) + static_cast<float>(input.GetKeyPress(DIK_W));
    D3DXVECTOR3 movement = camera->GetCameraRight()*axis.x*m_move_speed + camera_forward * axis.y*m_move_speed;
    m_position += movement;
    m_position = m_field_edit->AdjustPosition(m_position);

    // 操作
    ImGui::InputFloat("Move Speed", &m_move_speed);

    m_field_edit->SetPosition(m_position);
    m_model_edit->SetPosition(m_position);
   
    movement = m_position - previous_position;
    camera->Move(movement);

    if (ImGui::Button("Set Player"))
    {
        auto transform = static_cast<Transform*>(m_player->GetComponent("Transform"));
        transform->SetPosition(m_position);
    }
}