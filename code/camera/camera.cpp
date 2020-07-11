//--------------------------------------------------------------------------------
// カメラ
//--------------------------------------------------------------------------------
#include "camera.h"
#include "camera_manager.h"
#include "game_system.h"

Camera::Camera():
    m_eye(D3DXVECTOR3(0.0f,0.0f,0.0f)),
    m_at(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_forward(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_up(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_right(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
    m_near(0.1f),
    m_far(1000.0f),
    m_fov(75.0f/180.0f*PI),
    m_distance(10.0f)
{
    m_rig.position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_rig.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_pivot.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_pivot.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXMatrixIdentity(&m_view_matrix);
    D3DXMatrixIdentity(&m_project_matrix);
    D3DXMatrixIdentity(&m_view_transpose);
}

Camera::~Camera()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void Camera::Init()
{
    GameSystem::Instance().GetCameraManager().RegisterAsMain(this);
}
//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void Camera::Uninit()
{

}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void Camera::Update()
{

}
//--------------------------------------------------------------------------------
// セットカメラ
//--------------------------------------------------------------------------------
void Camera::Set()
{
    D3DXMATRIX rig_matrix, pivot_matrix;
    D3DXMATRIX translation_matrix, rotation_matrix;
    D3DXMatrixTranslation(&translation_matrix, m_rig.position.x, m_rig.position.y, m_rig.position.z);
    D3DXMatrixRotationYawPitchRoll(&rotation_matrix, m_rig.rotation.x, m_rig.rotation.y, m_rig.rotation.z);
    rig_matrix = rotation_matrix * translation_matrix;

    D3DXMatrixTranslation(&translation_matrix, m_pivot.position.x, m_pivot.position.y, m_pivot.position.z);
    D3DXMatrixRotationYawPitchRoll(&rotation_matrix, m_pivot.rotation.x, m_pivot.rotation.y, m_pivot.rotation.z);
    pivot_matrix = rotation_matrix * translation_matrix;

    pivot_matrix *= rig_matrix;

    D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &pivot_matrix);
    D3DXVec3TransformNormal(&m_up, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &pivot_matrix);
    D3DXVec3Cross(&m_right, &m_up, &m_forward);

    D3DXVec3TransformCoord(&m_eye, &D3DXVECTOR3(0.0f, 0.0f, -m_distance), &pivot_matrix);
    m_at = m_eye + m_forward * m_distance;

    // プロジェクション（パースペクティブ）行列の作成
    D3DXMatrixPerspectiveFovLH(&m_project_matrix, m_fov, (float)SCREEN_WIDTH / SCREEN_HEIGHT, m_near, m_far);
    D3DXMatrixLookAtLH(&m_view_matrix, &m_eye, &m_at, &m_up);

    D3DXMatrixTranspose(&m_view_transpose, &m_view_matrix);
}
//--------------------------------------------------------------------------------
// カメラ移動
//--------------------------------------------------------------------------------
void Camera::Move(const D3DXVECTOR3& movement)
{
    m_rig.position += movement;
}
//--------------------------------------------------------------------------------
// ｘ軸回転
//--------------------------------------------------------------------------------
void Camera::Pitch(const float& rotation_x)
{
    m_rig.rotation.x += rotation_x;
}
//--------------------------------------------------------------------------------
// ｙ軸回転
//--------------------------------------------------------------------------------
void Camera::Yaw(const float& rotation_y)
{
    m_pivot.rotation.y += rotation_y;
}