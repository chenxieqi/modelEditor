//--------------------------------------------------------------------------------
// �t�B�[���h�R���C�_�[
//--------------------------------------------------------------------------------
#pragma once
#include "collider.h"
#include "main.h"
#include <vector>

using namespace std;

//--------------------------------------------------------------------------------
// �\����
//--------------------------------------------------------------------------------
struct PolygonInfo//�|���S�����
{
    D3DXVECTOR3 left_up;
    D3DXVECTOR3 right_down;
    D3DXVECTOR3 side;
    D3DXVECTOR3 normal;
};
//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class ColliderField :
    public Collider
{
public:
    ColliderField(GameObject* game_object,const string& name) :Collider(game_object, "ColliderFiled", COLLIDER_FIELD),
        m_block_number_x(0),
        m_block_number_z(0),
        m_block_size(D3DXVECTOR2(0,0))
    {
        Load(name);
    }
    ~ColliderField();
    //--------------------------------------------------------------------------------
    // �Q�b�g�|���S�����
    //--------------------------------------------------------------------------------
    PolygonInfo* GetPolygon(const D3DXVECTOR3& vertex)const;
private:
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    int                 m_block_number_x;
    int                 m_block_number_z;
    D3DXVECTOR2         m_block_size;
    vector<D3DXVECTOR3> m_vertexes;
    //--------------------------------------------------------------------------------
    // ���[�h
    //--------------------------------------------------------------------------------
    void Load(const string& name);
};

