//--------------------------------------------------------------------------------
// タイトルモード
//--------------------------------------------------------------------------------
#pragma once
#include "mode.h"

class ModeTitle:public Mode
{
public:
    ModeTitle(){}
    ~ModeTitle(){}

    void Init()override;
    void Uninit()override;
    void Update()override;
};

