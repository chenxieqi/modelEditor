//--------------------------------------------------------------------------------
// エディター
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
// クラス
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
    // 基本関数
    //--------------------------------------------------------------------------------
    void Init()override;
    void Uninit()override;
    void Update()override;
    void LateUpdate()override;
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    void SetFieldEdit(BehaviorFieldEdit* field_edit) { m_field_edit = field_edit; }
    void SetModelEdit(BehaviorModelEdit* model_edit) { m_model_edit = model_edit; }
private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    GameObject *        m_player;
    BehaviorFieldEdit * m_field_edit;
    BehaviorModelEdit*  m_model_edit;
    bool                m_show_camera_window;
    D3DXVECTOR3         m_position;
    float               m_move_speed;
    string              m_stage_name;
    //--------------------------------------------------------------------------------
    // セーブ
    //--------------------------------------------------------------------------------
    void Save(void);
    void SaveAsBinary(const string& name);
    //--------------------------------------------------------------------------------
    // ロード
    //--------------------------------------------------------------------------------
    void Load(void);
    void LoadFrom(const string& name);
    //--------------------------------------------------------------------------------
    // ウィンドウ
    //--------------------------------------------------------------------------------
    void ShowCameraWindow(void);
    void ShowPositionWindow(void);
};
