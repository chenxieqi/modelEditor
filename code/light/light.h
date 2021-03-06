//--------------------------------------------------------------------------------
// ライト
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class Light
{
public:
    Light();
    ~Light();
    //--------------------------------------------------------------------------------
    // ゲット方向
    //--------------------------------------------------------------------------------
    auto& GetDirection()const { return m_direction; }
private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    D3DXCOLOR       m_ambient;
    D3DXCOLOR       m_diffuse;
    D3DXVECTOR3     m_direction;
 };

