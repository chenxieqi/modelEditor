//--------------------------------------------------------------------------------
// リザルトモード
//--------------------------------------------------------------------------------
#pragma once
#include "mode.h"
class ModeResult :public Mode
{
public:
    ModeResult(){}
    ~ModeResult(){}

    void Init()override;
    void Uninit()override;
    void Update()override;
};

