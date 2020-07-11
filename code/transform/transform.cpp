//--------------------------------------------------------------------------------
// ���W�ϊ�
//--------------------------------------------------------------------------------
#include "transform.h"
#include "gameobject.h"
#include <cassert>

Transform::~Transform()
{
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void Transform::Update()
{
    if (!m_parent)
    {
        UpdateMatrix();
    }
}

void Transform::UpdateMatrix()
{
    CalculateMatrix();

    for (auto& pair : m_child)
    {
        pair.second->UpdateMatrix(m_matrix);
    }
}

void Transform::UpdateMatrix(const D3DXMATRIX& parent_matrix)
{

    CalculateMatrix();

    m_matrix *= parent_matrix;

    for (auto& pair : m_child)
    {
        pair.second->UpdateMatrix(m_matrix);
    }
}

//--------------------------------------------------------------------------------
// �Z�b�g�e
//--------------------------------------------------------------------------------
void Transform::SetParent(Transform* parent)
{
    if (m_parent)
    {
        m_parent->DeleteChild(this);
    }
    
    m_parent = parent;
    if (m_parent)
    {
        m_parent->SetChild(this);
    }
}

//--------------------------------------------------------------------------------
// �Z�b�g�q��
//--------------------------------------------------------------------------------
void Transform::SetChild(Transform* child)
{
    assert(child);
    m_child.emplace(child->GetGameObject()->GetName(), child);
}
//--------------------------------------------------------------------------------
// �q��������
//--------------------------------------------------------------------------------
void Transform::DeleteChild(Transform* child)
{
    assert(child);
    auto iterator = m_child.find(child->GetGameObject()->GetName());
    if (iterator!=m_child.end())// ������
    {
        m_child.erase(iterator);
    }
}
//--------------------------------------------------------------------------------
// �}�g���b�N�X�v�Z
//--------------------------------------------------------------------------------
void Transform::CalculateMatrix()
{
    D3DXMATRIX position, rotation, scale;
    D3DXMatrixTranslation(&position, m_position.x, m_position.y, m_position.z);
    D3DXMatrixRotationQuaternion(&rotation, &m_rotation);
    D3DXMatrixScaling(&scale, m_scalling.x, m_scalling.y, m_scalling.z);
    m_matrix = scale * rotation *position;
}