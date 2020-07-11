//--------------------------------------------------------------------------------
// フィールドコライダー
//--------------------------------------------------------------------------------
#include "collider_field.h"
#include "library\cereal\archives\binary.hpp"
#include <cassert>
#include <fstream>
using namespace cereal;


ColliderField::~ColliderField()
{
}
//--------------------------------------------------------------------------------
// ゲットポリゴン情報
//--------------------------------------------------------------------------------
PolygonInfo* ColliderField::GetPolygon(const D3DXVECTOR3& vertex)const
{
    D3DXVECTOR3 center = D3DXVECTOR3(m_world._41, m_world._42, m_world._43);
    D3DXVECTOR3 left_up = D3DXVECTOR3(center.x - (m_block_size.x*m_block_number_x*0.5f), 0, center.z + (m_block_size.y*m_block_number_z*0.5f));
    
    // プレイヤーがいる地面ブロックの頂点番号
    int left_up_number, right_down_number;

    int left_up_number_x = (int)(((vertex.x - left_up.x) / ((float)m_block_number_x*m_block_size.x))*(float)m_block_number_x);
    int left_up_number_z = -(int)(((vertex.z - left_up.z) / ((float)m_block_number_z*m_block_size.y))*(float)m_block_number_z);
    left_up_number = left_up_number_z * (m_block_number_x + 1) + left_up_number_x;
    right_down_number = (left_up_number_z+1)* (m_block_number_x + 1)+left_up_number_x + 1;

    if (left_up_number_x < 0|| left_up_number_x>m_block_number_x|| left_up_number_z<0||left_up_number_z>m_block_number_z)
    {
        return NULL;
    }

    PolygonInfo* result = new PolygonInfo;
    result->left_up = m_vertexes[left_up_number];
    result->right_down = m_vertexes[right_down_number];

    D3DXVECTOR3 left_up_to_right_down = m_vertexes[right_down_number] - m_vertexes[left_up_number];
    D3DXVECTOR3 left_up_to_vertex = D3DXVECTOR3(vertex.x, 0.0f, vertex.z) - m_vertexes[left_up_number];
    D3DXVECTOR3 cross;
    D3DXVec3Cross(&cross, &left_up_to_right_down, &left_up_to_vertex);

    int side_x, side_z,sign;
    if (cross.y >= 0)
    {
        // 左
        side_x = left_up_number_x + 1;
        side_z = left_up_number_z;
        sign = -1;
    }
    else {
        // 右
        side_x = left_up_number_x;
        side_z = left_up_number_z + 1;
        sign = 1;
    }
    result->side = m_vertexes[side_z*(m_block_number_x + 1) + side_x];
    D3DXVECTOR3 side_to_left_up = result->left_up - result->side;
    D3DXVECTOR3 side_to_right_down = result->right_down - result->side;
    D3DXVec3Cross(&result->normal, &side_to_left_up, &side_to_right_down);
    result->normal *= sign;
    D3DXVec3Normalize(&result->normal, &result->normal);

    return result;
}
//--------------------------------------------------------------------------------
// ロード
//--------------------------------------------------------------------------------
void ColliderField::Load(const string& name)
{
    auto path = "data/field/" + name + ".field";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive archive(file);
    archive.loadBinary(&m_block_number_x, sizeof(m_block_number_x));
    archive.loadBinary(&m_block_number_z, sizeof(m_block_number_z));
    archive.loadBinary(&m_block_size, sizeof(m_block_size));
    int vertex_number;
    archive.loadBinary(&vertex_number, sizeof(vertex_number));
    m_vertexes.resize(vertex_number);
    archive.loadBinary(&m_vertexes[0], sizeof(D3DXVECTOR3) * vertex_number);
    file.close();
}