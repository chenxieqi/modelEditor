//--------------------------------------------------------------------------------
// モデルエディター
//--------------------------------------------------------------------------------
#include "behavior_model_edit.h"
#include "library\ImGui\imgui_impl_dx9.h"
#include "library\cereal\archives\binary.hpp"
#include <fstream>

#include "transform.h"
#include "game_object_factory.h"
#include "gameobject.h"

using namespace cereal;
//--------------------------------------------------------------------------------
// IMGUI設定
//--------------------------------------------------------------------------------
namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& values = *static_cast<vector<string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(values.size())) { return false; }
        *out_text = values.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, vector<string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, vector<string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

}

BehaviorModelEdit::~BehaviorModelEdit()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void BehaviorModelEdit::Init()
{
    string folder_path = GetProjectPath() + "data\\model";
    m_mode_names = GetFilesFromFolder(folder_path, "model");

    size_t model_number = m_mode_names.size();
    m_model_infos.resize(model_number);
    for (size_t count = 0; count < model_number; count++)
    {
        auto game_object = GameObjectFactory::CreateModel(m_mode_names[count], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
        game_object->SetActive(false);
        m_model_infos[count].m_transform = static_cast<Transform*>(game_object->GetComponent("Transform"));
    }
    m_created_model_infos.resize(model_number);
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void BehaviorModelEdit::Uninit()
{
    //ClearList();
    m_model_infos.clear();
    m_mode_names.clear();
    m_current_model_number = 0;
    m_show_list = false;
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void BehaviorModelEdit::Update()
{
    if (!m_show)
    {
        for (auto info : m_model_infos)
        {
            info.m_transform->GetGameObject()->SetActive(false);
        }
        return;
    }
    ShowMainWindow();
    ShowCreatedList();
}
//--------------------------------------------------------------------------------
// モデル情報セーブ
//--------------------------------------------------------------------------------
void BehaviorModelEdit::SaveAsBinary(const string& name)
{
    string path = "data/stage/" + name + ".stage";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .stage file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    size_t model_number = m_mode_names.size();
    archive.saveBinary(&model_number, sizeof(model_number));

    for (size_t count = 0; count < model_number; count++)
    {
        size_t name_size = name.size();
        archive.saveBinary(&name_size, sizeof(name_size));
        archive.saveBinary(&name[0], name_size);

        size_t created_model_number = m_created_model_infos[count].size();
        archive.saveBinary(&created_model_number, sizeof(created_model_number));

        for (auto& info : m_created_model_infos[count])
        {
            auto& position = info.m_transform->GetPosition();
            archive.saveBinary(&position, sizeof(position));
            auto& rotation = info.m_transform->GetRotation();
            archive.saveBinary(&rotation, sizeof(rotation));
            auto& scale = info.m_transform->GetScalling();
            archive.saveBinary(&scale, sizeof(scale));
        }
    }

    file.close();
    MessageBox(NULL, " .stage file save complete", path.c_str(), MB_OK | MB_ICONWARNING);

}
//--------------------------------------------------------------------------------
// モデル情報ロード
//--------------------------------------------------------------------------------
void BehaviorModelEdit::LoadFrom(const string& name)
{
    string path = "data/stage/" + name + ".stage";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .stage file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    ClearList();

    size_t model_number;
    archive.loadBinary(&model_number, sizeof(model_number));

    for (size_t count = 0; count < model_number; count++)
    {
        string name;
        size_t name_size;
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);

        size_t created_model_number;
        archive.loadBinary(&created_model_number, sizeof(created_model_number));

        for (size_t count_created=0; count_created<created_model_number;++count_created)
        {
            D3DXVECTOR3 position;
            archive.loadBinary(&position, sizeof(position));
            D3DXQUATERNION rotation;
            archive.loadBinary(&rotation, sizeof(rotation));
            D3DXVECTOR3 scale;
            archive.loadBinary(&scale, sizeof(scale));

            auto game_obeject = GameObjectFactory::CreateModel(m_mode_names[count], position, rotation, scale);
            ModelInfo info;
            info.m_transform = static_cast<Transform*>(game_obeject->GetComponent("Transform"));
            info.m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            m_created_model_infos[count].push_back(info);
        }
    }

    file.close();
    MessageBox(NULL, " .stage file load complete", path.c_str(), MB_OK | MB_ICONWARNING);

}
//--------------------------------------------------------------------------------
// セット位置
//--------------------------------------------------------------------------------
void BehaviorModelEdit::SetPosition(const D3DXVECTOR3& position)
{
    if (m_model_infos.empty()) { return; }
    m_model_infos[m_current_model_number].m_transform->SetPosition(position);
}
//--------------------------------------------------------------------------------
// モデル生成
//--------------------------------------------------------------------------------
void BehaviorModelEdit::Create(void)
{
    auto game_object = GameObjectFactory::CreateModel(m_mode_names[m_current_model_number],D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
    auto transform = static_cast<Transform*>(game_object->GetComponent("Transform"));
    auto& current_info = m_model_infos[m_current_model_number];
    auto& position = current_info.m_transform->GetPosition();
    auto& rotation = current_info.m_transform->GetRotation();
    auto& scale = current_info.m_transform->GetScalling();
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScaling(scale);
    ModelInfo info;
    info.m_transform = transform;
    info.m_rotation = current_info.m_rotation;
    m_created_model_infos[m_current_model_number].push_back(info);
}
//--------------------------------------------------------------------------------
// ウィンドウ
//--------------------------------------------------------------------------------
void BehaviorModelEdit::ShowMainWindow(void)
{
    if (!ImGui::Begin("Model Edit", &m_show))
    {
        ImGui::End();
        return;
    }
    ShowTypeListBox();

    if (!m_model_infos.empty())
    {
        auto& current_info = m_model_infos[m_current_model_number];
        
        // 回転
        if (ImGui::SliderFloat3("Rotation", &current_info.m_rotation.x, 0.0f, D3DX_PI*2.0f))
        {
            D3DXQUATERNION rotation;
            D3DXQuaternionRotationYawPitchRoll(&rotation, current_info.m_rotation.y, current_info.m_rotation.x, current_info.m_rotation.z);
            current_info.m_transform->SetRotation(rotation);
        }

        // スケール
        D3DXVECTOR3 scale=current_info.m_transform->GetScalling();
        if (ImGui::InputFloat3("Scale", &scale.x))
        {
            current_info.m_transform->SetScaling(scale);
        }

        // 作成
        if (ImGui::Button("Create Model")) { Create(); }
    }

    // モデルリスト
    size_t model_number = m_mode_names.size();
    for (size_t count = 0; count < model_number; ++count)
    {
        ImGui::Text("%s:%d", string(m_mode_names[count].begin(), m_mode_names[count].end()).c_str(), static_cast<int>(m_created_model_infos[count].size()));
    }
    if (ImGui::Button("ShowCreateList")) { m_show_list ^= 1; }

    ImGui::End();
}

//--------------------------------------------------------------------------------
// モデルリストウィンドウ
//--------------------------------------------------------------------------------
void BehaviorModelEdit::ShowTypeListBox(void)
{
    size_t model_number = m_mode_names.size();

    if (ImGui::Combo("Model", &m_current_model_number, m_mode_names))
    {
        for (size_t count = 0; count < model_number; ++count)
        {
            m_model_infos[count].m_transform->GetGameObject()->SetActive(m_current_model_number == static_cast<int>(count));//
        }
    }
}
//--------------------------------------------------------------------------------
// 生成したモデルリストウィンドウ
//--------------------------------------------------------------------------------
void BehaviorModelEdit::ShowCreatedList(void)
{
    if (!m_show_list) { return; }
    if (!ImGui::Begin("Created Model List Window", &m_show))
    {
        ImGui::End();
        return;
    }
    size_t model_number = m_mode_names.size();
    for (size_t count = 0; count < model_number; ++count)
    {
        string model_name(m_mode_names[count].begin(), m_mode_names[count].end());
        if (ImGui::TreeNode(model_name.c_str()))
        {
            int count_object = 0;
            for (auto iterator = m_created_model_infos[count].begin(); iterator != m_created_model_infos[count].end();)
            {
                bool is_dalete = false;
                string buffer = model_name + to_string(count_object);
                if (ImGui::TreeNode(buffer.c_str()))
                {
                    D3DXVECTOR3 position = iterator->m_transform->GetPosition();
                    D3DXVECTOR3 scale = iterator->m_transform->GetScalling();

                    // Offset
                    if (ImGui::InputFloat3("Position", &position.x))
                    {
                        iterator->m_transform->SetPosition(position);
                    }
                    if (ImGui::InputFloat3("Rotation", &iterator->m_rotation.x))
                    {
                        D3DXQUATERNION rotation;
                        D3DXQuaternionRotationYawPitchRoll(&rotation, iterator->m_rotation.y, iterator->m_rotation.x, iterator->m_rotation.z);
                        iterator->m_transform->SetRotation(rotation);
                    }
                    if (ImGui::InputFloat3("Scale", &scale.x))
                    {
                        iterator->m_transform->SetScaling(scale);
                    }

                    is_dalete = ImGui::Button("Delete");
                    ImGui::TreePop();
                }

                if (is_dalete)
                {
                    iterator->m_transform->GetGameObject()->SetAlive(false);
                    iterator = m_created_model_infos[count].erase(iterator);
                }
                else{
                    ++iterator;
                    ++count_object;
                }
            }
            ImGui::TreePop();
        }

    }
    ImGui::End();
}
//--------------------------------------------------------------------------------
// クリア
//--------------------------------------------------------------------------------
void BehaviorModelEdit::ClearList(void)
{
    for (auto& list : m_created_model_infos)
    {
        for (auto& info : list)
        {
            info.m_transform->GetGameObject()->SetAlive(false);
        }
        list.clear();
    }
}
//--------------------------------------------------------------------------------
// ゲットモデルパス
//--------------------------------------------------------------------------------
string BehaviorModelEdit::GetProjectPath(void)
{
    char path[MAX_PATH + 1];
    if (0 != GetModuleFileNameA(NULL, path, MAX_PATH))
    {
        char drive[MAX_PATH + 1], directory[MAX_PATH + 1], filename[MAX_PATH + 1], extension[MAX_PATH + 1];
        // パス名を構成要素に分解する
        _splitpath_s(path, drive, directory, filename, extension);
        string projectpath = drive;
        projectpath += directory;
        return string(projectpath.begin(), projectpath.end());
    }
    return string();
}
//--------------------------------------------------------------------------------
// ゲットモデルファイル
//--------------------------------------------------------------------------------
vector<string> BehaviorModelEdit::GetFilesFromFolder(const string& path, const string& extension)
{
    HANDLE handle;
    WIN32_FIND_DATA data;
    vector<string> file_name;

    // 拡縮子の設定
    string serch_name = path + "\\*." + extension;

    handle = FindFirstFile(serch_name.c_str(), &data);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return file_name;
    }
    do {
        if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){}
        else {
            file_name.push_back(data.cFileName);
        }
    } while (FindNextFile(handle, &data));
    FindClose(handle);
    file_name.shrink_to_fit();
    return file_name;
}