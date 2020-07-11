//--------------------------------------------------------------------------------
// カメラエディター
//--------------------------------------------------------------------------------
#include "camera_edit.h"
#include "main.h"
#include "game_system.h"
#include "input_system.h"

CameraEdit::CameraEdit()
{
}


CameraEdit::~CameraEdit()
{
}
//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CameraEdit::Init(void)
{
    Camera::Init();
    SetPitch(45.0f / 180.0f*D3DX_PI);
    SetDistance(20.0f);
}
//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CameraEdit::Update(void)
{
    auto& input = GameSystem::Instance().GetMouseSystem();
    if(input.GetRightPress())
    { 
        float rotation_x = input.GetAxisX();
        float rotation_y = input.GetAxisY();
        float zoom = input.GetAxisZ();
        if (fabsf(rotation_y)>0)
        {
            Yaw(rotation_y*m_rotation_speed);
        }
        if (fabsf(rotation_x) > 0)
        {
            Pitch(rotation_x*m_rotation_speed);
        }
        if (fabsf(zoom) > 0)
        {
            float distance = m_distance + zoom * m_zoom_speed;
            if (distance < m_distance_min) { distance = m_distance_min; }
            if (distance > m_distance_max) { distance = m_distance_max; }
            SetDistance(distance);
        }
    
    }
}