//--------------------------------------------------------------------------------
// フィールドエディター
//--------------------------------------------------------------------------------
#include "behavior_field_edit.h"
#include "library\ImGui\imgui_impl_dx9.h"
#include "library\cereal\archives\binary.hpp"
#include <fstream>

#include "game_system.h"
#include "mesh_manager.h"
#include "input_system.h"

using namespace cereal;

BehaviorFieldEdit::~BehaviorFieldEdit()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::Init()
{
    Initvertexes();
    vector<int> mesh_indexes = InitIndexes();
    GameSystem::Instance().GetMeshManager().Make("FieldEdit", D3DPT_TRIANGLESTRIP, m_vertex3Ds, mesh_indexes, (m_block_number_x + 2) * 2 * m_block_number_z - 4);
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::Uninit()
{
    m_vertex3Ds.clear();
    GameSystem::Instance().GetMeshManager().DisUse("FieldEdit");
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::Update()
{
    if (!m_show) { return; }

    auto& input = GameSystem::Instance().GetKeyboardSystem();

    auto indexes = GetChoosenIndexes();

    // up->1.0f,down->-1.0f
    float input_value = (float)(input.GetKeyPress(DIK_UP)) - (float)(input.GetKeyPress(DIK_DOWN));
    float raise_value = input_value * m_raise_speed;

    UpdateVertexes(raise_value, indexes);
    GameSystem::Instance().GetMeshManager().Update("FieldEdit", m_vertex3Ds, indexes);

    ShowWindow();
}
void BehaviorFieldEdit::LateUpdate()
{

}
//--------------------------------------------------------------------------------
// 地面情報セーブ
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::SaveAsBinary(const string& name)
{
    // 選択された頂点の色を戻す
    UpdateVertexes(0.0f, list<int>());

    GameSystem::Instance().GetMeshManager().SaveMeshToFile("FieldEdit", name);

    string path = "data/field/" + name + ".field";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .field file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    archive.saveBinary(&m_block_number_x, sizeof(m_block_number_x));
    archive.saveBinary(&m_block_number_z, sizeof(m_block_number_z));
    archive.saveBinary(&m_block_size, sizeof(m_block_size));
    int vertex_number=m_vertex3Ds.size();
    archive.saveBinary(&vertex_number, sizeof(vertex_number));

    for (size_t count = 0; count < vertex_number; ++count)
    {
        archive.saveBinary(&m_vertex3Ds[count].pos, sizeof(m_vertex3Ds[count].pos));
    }

    file.close();
    MessageBox(NULL, ".field file save complete", path.c_str(), MB_OK | MB_ICONWARNING);

}
//--------------------------------------------------------------------------------
// フィールド情報ロード
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::LoadFrom(const string& name)
{
    string path = "data/field/" + name + ".field";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, " .field file not found", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    GameSystem::Instance().GetMeshManager().DisUseImmediately("FieldEdit");
    m_pervious_indexes.clear();
    m_vertex3Ds.clear();

    archive.loadBinary(&m_block_number_x, sizeof(m_block_number_x));
    archive.loadBinary(&m_block_number_z, sizeof(m_block_number_z));
    archive.loadBinary(&m_block_size, sizeof(m_block_size));
    size_t vertex_number;
    archive.loadBinary(&vertex_number, sizeof(vertex_number));

    Initvertexes();
    for (size_t count = 0; count < vertex_number; ++count)
    {
        archive.loadBinary(&m_vertex3Ds[count].pos, sizeof(m_vertex3Ds[count].pos));
    }

    RecalculateNormal(GetAllIndexes());

    vector<int> mesh_indexes = InitIndexes();
    GameSystem::Instance().GetMeshManager().Make("FieldEdit", D3DPT_TRIANGLESTRIP, m_vertex3Ds, mesh_indexes, (m_block_number_x + 2) * 2 * m_block_number_z - 4);
    
    file.close();
    MessageBox(NULL, ".field file load complete", path.c_str(), MB_OK | MB_ICONWARNING);
}
//--------------------------------------------------------------------------------
// 位置調整
//--------------------------------------------------------------------------------
D3DXVECTOR3 BehaviorFieldEdit::AdjustPosition(const D3DXVECTOR3& position)
{
    D3DXVECTOR3 result;
    result = position;

    if (result.x<m_min_positon.x)
    {
        result.x = m_min_positon.x;
    }
    if (result.x>m_max_positon.x)
    {
        result.x = m_max_positon.x;
    }
    if (result.z<m_min_positon.z)
    {
        result.z = m_min_positon.z;
    }
    if (result.z>m_max_positon.z)
    {
        result.z = m_max_positon.z;
    }

    result.y = GetHeight(result);

    return result;
}
//--------------------------------------------------------------------------------
// ウィンドウ
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::ShowWindow(void)
{
    if (!ImGui::Begin("Field Edit", &m_show))
    {
        ImGui::End();
        return;
    }

    // 起伏補間モード平均と線形の比率
    ImGui::SliderFloat("Rate of average and linear", &m_raise_mode_rate, 0.0f, 1.0f);

    // 起伏速度
    ImGui::InputFloat("Raise Speed", &m_raise_speed);

    // 拡縮速度
    ImGui::InputFloat("Extend Speed", &m_extend_speed);

    // 拡縮範囲
    ImGui::DragFloat("Choose Range", &m_choose_range.x, m_extend_speed, 0.0f, 100000.0f);
    m_choose_range.y = m_choose_range.x;

    // 高さを初期化する
    if (ImGui::Button("Set Height To 0"))
    {
        list<int> indexes = GetChoosenIndexes();
        for (int index : indexes)
        {
            m_vertex3Ds[index].pos.y = 0.0f;
        }
        RecalculateNormal(indexes);
        GameSystem::Instance().GetMeshManager().Update("FieldEdit", m_vertex3Ds, indexes);
    }

    ImGui::End();
}

//--------------------------------------------------------------------------------
// 高さ計算してゲット
//--------------------------------------------------------------------------------
float BehaviorFieldEdit::GetHeight(D3DXVECTOR3 position)
{
    D3DXVECTOR3 left_up = D3DXVECTOR3( - (m_block_size.x*m_block_number_x*0.5f), 0,  (m_block_size.y*m_block_number_z*0.5f));

    // プレイヤーがいる地面ブロックの頂点番号
    int left_up_number, right_down_number;

    int left_up_x = (int)(((position.x - left_up.x) / ((float)m_block_number_x*m_block_size.x))*(float)m_block_number_x);
    int left_up_z = -(int)(((position.z - left_up.z) / ((float)m_block_number_z*m_block_size.y))*(float)m_block_number_z);
    int right_down_x = left_up_x + 1;
    int right_down_z = left_up_z + 1;

    if (left_up_x < 0 || left_up_x>=m_block_number_x || left_up_z<0 || left_up_z>=m_block_number_z)
    {
        return NULL;
    }
    D3DXVECTOR3 target_position(position.x,0.0f,position.z);
    D3DXVECTOR3 left_up_position = m_vertex3Ds[left_up_z*(m_block_number_x + 1) + left_up_x].pos;
    D3DXVECTOR3 right_down_position = m_vertex3Ds[right_down_z*(m_block_number_x + 1) + right_down_x].pos;

    D3DXVECTOR3 left_up_to_right_down = right_down_position - left_up_position;
    D3DXVECTOR3 left_up_to_target = target_position - left_up_position;
    D3DXVECTOR3 cross;
    D3DXVec3Cross(&cross, &left_up_to_right_down, &left_up_to_target);

    int side_x, side_z, sign;
    if (cross.y >= 0)
    {
        // 左
        side_x = left_up_x + 1;
        side_z = left_up_z;
        sign = -1;
    }
    else {
        // 右
        side_x = left_up_x;
        side_z = left_up_z + 1;
        sign = 1;
    }
    D3DXVECTOR3 side_position = m_vertex3Ds[side_z*(m_block_number_x + 1) + side_x].pos;
    D3DXVECTOR3 side_to_left_up = left_up_position - side_position;
    D3DXVECTOR3 side_to_right_down = right_down_position - side_position;
    D3DXVECTOR3 normal;
    D3DXVec3Cross(&normal, &side_to_left_up, &side_to_right_down);
    normal *= sign;
    D3DXVec3Normalize(&normal, &normal);
    float y = side_position.y - ((position.x - side_position.x)*normal.x + (position.z - side_position.z)*normal.z) / normal.y;
    return y;

}

//--------------------------------------------------------------------------------
// 選択された頂点のインデックスをゲット
//--------------------------------------------------------------------------------
list<int> BehaviorFieldEdit::GetChoosenIndexes(void)
{
    list<int>   indexes;
    D3DXVECTOR3 edit_position = m_editor_positon;
    edit_position.y = 0.0f;
    D3DXVECTOR3 left_up= D3DXVECTOR3(- (m_block_size.x*m_block_number_x*0.5f), 0, (m_block_size.y*m_block_number_z*0.5f));
    int left_up_x = (int)(((edit_position.x - left_up.x) / ((float)m_block_number_x*m_block_size.x))*(float)m_block_number_x);
    int left_up_z = -(int)(((edit_position.z - left_up.z) / ((float)m_block_number_z*m_block_size.y))*(float)m_block_number_z);
    int right_down_x = left_up_x + 1;
    int right_down_z = left_up_z + 1;

    int range_x = 1 + (int)m_choose_range.x / m_block_size.x;
    int range_z = 1 + (int)m_choose_range.y / m_block_size.y;

    int min_x = left_up_x - range_x;
    int max_x = right_down_x + range_x;
    int min_z = left_up_z - range_z;
    int max_z = right_down_z + range_z;

    min_x = min_x < 0 ? 0 : min_x;
    min_z = min_z < 0 ? 0 : min_z;
    max_x = max_x > m_block_number_x ? m_block_number_x : max_x;
    max_z = max_z > m_block_number_z ? m_block_number_z : max_z;

    for (int count_z = min_z; count_z < max_z; count_z++)
    {
        for (int count_x = min_x; count_x < max_x; count_x++)
        {
            int index = count_z * (m_block_number_x + 1) + count_x;
            D3DXVECTOR3 position = m_vertex3Ds[index].pos;
            position.y = 0.0f;
            float length = D3DXVec3LengthSq(&D3DXVECTOR3(position - edit_position));
            if (length < m_choose_range.x*m_choose_range.x)
            {
                indexes.push_back(index);
            }
        }
    }
    return indexes;
}
//--------------------------------------------------------------------------------
// 全てのインデックスをゲット
//--------------------------------------------------------------------------------
list<int> BehaviorFieldEdit::GetAllIndexes(void)
{
    list<int> indexes;
    int index_max = (m_block_number_x + 1)*(m_block_number_z + 1);
    for (int count = 0; count < index_max; ++count)
    {
        indexes.push_back(count);
    }
    return indexes;
}
//--------------------------------------------------------------------------------
// 法線再計算
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::RecalculateNormal(const list<int>& indexes)
{
    for (int index : indexes)
    {
        int count_z = index / (m_block_number_z + 1);
        int count_x = index - count_z * (m_block_number_z + 1);

        D3DXVECTOR3 normal;
        D3DXVECTOR3 self = m_vertex3Ds[index].pos;
        D3DXVECTOR3 left;
        D3DXVECTOR3 right;
        D3DXVECTOR3 top;
        D3DXVECTOR3 bottom;

        if (count_x == 0)
        {
            left = self;
            right = m_vertex3Ds[count_z * (m_block_number_x + 1) + count_x + 1].pos;
        }
        else if (count_x < m_block_number_x)
        {
            left = m_vertex3Ds[count_z * (m_block_number_x + 1) + count_x - 1].pos;
            right = m_vertex3Ds[count_z * (m_block_number_x + 1) + count_x + 1].pos;
        }
        else
        {
            left = m_vertex3Ds[count_z * (m_block_number_x + 1) + count_x - 1].pos;
            right = self;
        }

        if (count_z == 0)
        {
            top = self;
            bottom = m_vertex3Ds[(count_z + 1) * (m_block_number_x + 1) + count_x].pos;
        }
        else if (count_z < m_block_number_z)
        {
            top = m_vertex3Ds[(count_z - 1) * (m_block_number_x + 1) + count_x].pos;
            bottom = m_vertex3Ds[(count_z + 1) * (m_block_number_x + 1) + count_x].pos;
        }
        else
        {
            top = m_vertex3Ds[(count_z - 1) * (m_block_number_x + 1) + count_x].pos;
            bottom = self;
        }
        D3DXVec3Cross(&normal, &D3DXVECTOR3(right - left), &D3DXVECTOR3(bottom - top));
        D3DXVec3Normalize(&normal, &normal);
        m_vertex3Ds[index].normal = normal;
    }
}
//--------------------------------------------------------------------------------
// 頂点情報初期化
//--------------------------------------------------------------------------------
void BehaviorFieldEdit::Initvertexes(void)
{
    D3DXVECTOR3 left_up = D3DXVECTOR3(-(m_block_size.x*m_block_number_x*0.5f), 0, (m_block_size.y*m_block_number_z*0.5f));
    m_vertex3Ds.resize((m_block_number_x + 1)*(m_block_number_z + 1));

    for (int count_z = 0; count_z<(m_block_number_z + 1); count_z++)
    {
        for (int count_x = 0; count_x < (m_block_number_x + 1); count_x++)
        {
            int index = count_z * (m_block_number_x + 1) + count_x;
            D3DXVECTOR3 position = left_up + D3DXVECTOR3(count_x * m_block_size.x, 0.0f, -count_z * m_block_size.y);
            m_vertex3Ds[index].pos = position;
            m_vertex3Ds[index].texcoord = D3DXVECTOR2(count_x * 1.0f / static_cast<float>(m_block_number_x), count_z * 1.0f / static_cast<float>(m_block_number_z));
            m_vertex3Ds[index].color = D3DCOLOR_RGBA(255, 255, 255, 255);
            m_vertex3Ds[index].normal = D3DXVECTOR3(0, 1, 0);

        }
    }
    D3DXVECTOR3 half_size = D3DXVECTOR3(m_block_number_x*m_block_size.x*0.5f, 0.0f, m_block_number_z*m_block_size.y*0.5f);
    m_min_positon = -half_size;
    m_max_positon = half_size;
}
//--------------------------------------------------------------------------------
// インデックス情報初期化
//--------------------------------------------------------------------------------
vector<int> BehaviorFieldEdit::InitIndexes(void)
{
    vector<int>     indexes;
    int index_number = ((m_block_number_x + 1) * 2 + 2) * m_block_number_z - 1;
    indexes.resize(index_number);

    for (int count_z = 0; count_z < m_block_number_z; ++count_z)
    {
        for (int count_x = 0; count_x < (m_block_number_x + 1) * 2 + 2; ++count_x)
        {
            if (count_x < (m_block_number_x + 1) * 2)
            {
                indexes[count_z * ((m_block_number_x + 1) * 2 + 2) + count_x] = count_x / 2 + (count_x + 1) % 2 * (m_block_number_x + 1) + count_z * (m_block_number_x + 1);
            }
            else if (count_z * ((m_block_number_x + 1) * 2 + 2) + count_x < (((m_block_number_x + 1) * 2 + 2) * m_block_number_z - 1))//縮退ポリゴン
            {
                indexes[count_z * ((m_block_number_x + 1) * 2 + 2) + count_x] = (count_x - 1) / 2 % (m_block_number_x + 1) + count_x % 2 * 2 * (m_block_number_x + 1) + count_z * (m_block_number_x + 1);
            }
        }
    }
    return indexes;
}
//--------------------------------------------------------------------------------
// 頂点情報更新
//--------------------------------------------------------------------------------   
void BehaviorFieldEdit::UpdateVertexes(const float& raise_value, const list<int>& indexes)
{
    // 前フレーム選択された頂点の色を白に戻す
    for (int index : m_pervious_indexes)
    {
        m_vertex3Ds[index].color = D3DCOLOR_RGBA(255, 255, 255, 255);
    }
    GameSystem::Instance().GetMeshManager().Update("FieldEdit", m_vertex3Ds, m_pervious_indexes);

    for (int index : indexes)
    {
        float length = D3DXVec2Length(&D3DXVECTOR2((m_editor_positon.x - m_vertex3Ds[index].pos.x), (m_editor_positon.z - m_vertex3Ds[index].pos.z)));
        float rate = m_choose_range.x == 0.0f ? 1.0f : ((m_choose_range.x - length) / m_choose_range.x);

        if (rate >= m_raise_mode_rate)
        {// 平均的に起伏する
            m_vertex3Ds[index].pos.y += raise_value;
            m_vertex3Ds[index].color = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else 
        {// 距離によって線形補間
            m_vertex3Ds[index].pos.y += raise_value*(rate/m_raise_mode_rate);
            m_vertex3Ds[index].color = D3DCOLOR_RGBA((int)(rate*255), 0,0, 255);
        }
    }

    if (raise_value > 0.0f)
    {
        RecalculateNormal(indexes);
    }

    m_pervious_indexes = indexes;
}