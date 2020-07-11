//--------------------------------------------------------------------------------
// ���W�ϊ�
//--------------------------------------------------------------------------------
#pragma once
#include <unordered_map>
#include <string>

#include "component.h"
#include "main.h"
//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class Transform:public Component
{
public:
    Transform(GameObject* game_object) :Component(game_object,"Transform")
        , m_position(D3DXVECTOR3(0.0f,0.0f,0.0f))
        , m_rotation(D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f))
        , m_scalling(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
        , m_parent(NULL)
    {}
    ~Transform();
    //--------------------------------------------------------------------------------
    // �X�V
    //--------------------------------------------------------------------------------
    void Update()override;
    //--------------------------------------------------------------------------------
    // �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
    void SetPosition(const float& x, const float& y, const float& z) { m_position.x = x; m_position.y = y;m_position.z = z;}
    void SetRotation(const D3DXQUATERNION& rotation) { m_rotation = rotation; }
    void SetRotation(const float& x, const float& y, const float& z, const float& w) {m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; m_rotation.w = w;}
    void SetScaling(const D3DXVECTOR3& scaling) { m_scalling = scaling; }
    void SetScaling(const float& x, const float& y, const float& z) { m_scalling.x = x; m_scalling.y = y; m_scalling.z = z; }
    void SetMatrix(const D3DXMATRIX& matrix) { m_matrix = matrix; }
    void SetParent( Transform* parent);
    void SetChild( Transform* child);
    //--------------------------------------------------------------------------------
    // �q���𖕎E����
    //--------------------------------------------------------------------------------
    void DeleteChild(Transform* child);
    //--------------------------------------------------------------------------------
    // �Q�b�^�[
    //--------------------------------------------------------------------------------
    auto& GetPosition()const { return m_position; }
    auto& GetRotation()const { return m_rotation; }
    auto& GetScalling()const { return m_scalling; }
    auto& GetMatrix()const { return m_matrix; }
    auto& GetParent()const { return m_parent; }
    auto& GetChild()const { return m_child; }

private:
    //--------------------------------------------------------------------------------
    // �}�g���b�N�X�֐�
    //--------------------------------------------------------------------------------
    void UpdateMatrix();
    void UpdateMatrix(const D3DXMATRIX& parent_matrix);
    void CalculateMatrix();
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    D3DXVECTOR3                 m_position;
    D3DXQUATERNION              m_rotation;
    D3DXVECTOR3                 m_scalling;
    D3DXMATRIX                  m_matrix;
    Transform*                  m_parent;
    std::unordered_map<std::string, Transform*> m_child;
};
