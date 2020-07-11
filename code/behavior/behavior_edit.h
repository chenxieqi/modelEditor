//--------------------------------------------------------------------------------
// �G�f�B�^�[
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"
#include "main.h"
#include <string>

class GameObject;
class BehaviorFieldEdit;
class BehaviorModelEdit;

using namespace std;
//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class BehaviorEdit :
    public Behavior
{
public:
    BehaviorEdit(GameObject* game_object):Behavior(game_object, "Edit"),
        m_position(D3DXVECTOR3(0.0f,0.0f,0.0f)),
        m_move_speed(1.0f)
    {}
    ~BehaviorEdit();
    //--------------------------------------------------------------------------------
    // ��{�֐�
    //--------------------------------------------------------------------------------
    void Init()override;
    void Uninit()override;
    void Update()override;
    void LateUpdate()override;
    //--------------------------------------------------------------------------------
    // �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetFieldEdit(BehaviorFieldEdit* field_edit) { m_field_edit = field_edit; }
    void SetModelEdit(BehaviorModelEdit* model_edit) { m_model_edit = model_edit; }
private:
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    GameObject *        m_player;
    BehaviorFieldEdit * m_field_edit;
    BehaviorModelEdit*  m_model_edit;
    bool                m_show_camera_window;
    D3DXVECTOR3         m_position;
    float               m_move_speed;
    string              m_stage_name;
    //--------------------------------------------------------------------------------
    // �Z�[�u
    //--------------------------------------------------------------------------------
    void Save(void);
    void SaveAsBinary(const string& name);
    //--------------------------------------------------------------------------------
    // ���[�h
    //--------------------------------------------------------------------------------
    void Load(void);
    void LoadFrom(const string& name);
    //--------------------------------------------------------------------------------
    // �E�B���h�E
    //--------------------------------------------------------------------------------
    void ShowCameraWindow(void);
    void ShowPositionWindow(void);
};
