//--------------------------------------------------------------------------------
// エディターモード
//--------------------------------------------------------------------------------
#pragma once
#include "mode.h"
class ModeEditor:public Mode
{
public:
    ModeEditor(){}
    ~ModeEditor(){}

    void Init()override;
    void Uninit()override;
    void Update()override;
};

