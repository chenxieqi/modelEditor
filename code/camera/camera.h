//--------------------------------------------------------------------------------
// カメラ
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

class Camera
{
public:
    Camera();
    ~Camera();
    virtual void Init();
    virtual void Uninit();
    virtual void Update();
    //--------------------------------------------------------------------------------
    // セットカメラ
    //--------------------------------------------------------------------------------
    void Set();
    //--------------------------------------------------------------------------------
    // カメラ移動
    //--------------------------------------------------------------------------------
    void Move(const D3DXVECTOR3& movement);
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    void SetDistance(const float& distance) { m_distance = distance; }
    void SetPivot(const D3DXVECTOR3& position) { m_pivot.position = position; }
    void SetPitch(const float& rotation_x) { m_rig.rotation.x = rotation_x; }
    void SetYaw(const float& rotation_y) { m_pivot.rotation.y = rotation_y; }
    void SetFov(const float& camera_fov) { m_fov = camera_fov; }
    void SetNear(const float& camera_near) { m_near = camera_near; }
    void SetFar(const float& camera_far) { m_far = camera_far; }
    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    auto& GetCameraEye() const{ return m_eye; }
    auto& GetCameraAt()const { return m_at; }
    auto& GetCameraForward()const { return m_forward; }
    auto& GetCameraUp()const { return m_up; }
    auto& GetCameraRight()const { return m_right; }
    auto& GetCameraViewMatrix()const { return m_view_matrix; }
    auto& GetCameraProjectMatrix()const { return m_project_matrix; }
    auto& GetCameraViewTranspose()const { return m_view_transpose; }

protected:
    //--------------------------------------------------------------------------------
    // 回転関数
    //--------------------------------------------------------------------------------
    void Pitch(const float& rotation_x);
    void Yaw(const float& rotation_y);
    //--------------------------------------------------------------------------------
    // 構造体
    //--------------------------------------------------------------------------------
    struct CameraTransform
    {
        D3DXVECTOR3 position;
        D3DXVECTOR3 rotation;
    };
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    CameraTransform m_rig;
    CameraTransform m_pivot;
    D3DXVECTOR3     m_eye;
    D3DXVECTOR3     m_at;
    D3DXVECTOR3     m_forward;
    D3DXVECTOR3     m_up;
    D3DXVECTOR3     m_right;
    D3DXMATRIX      m_view_matrix;
    D3DXMATRIX      m_project_matrix;
    D3DXMATRIX      m_view_transpose;
    float           m_near;
    float           m_far;
    float           m_fov;
    float           m_distance;
};

