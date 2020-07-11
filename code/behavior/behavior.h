//--------------------------------------------------------------------------------
// ビヘイビア
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"

class Behavior:public Component
{
public:
    Behavior(GameObject* game_object, std::string name) :Component(game_object, name) {}
    ~Behavior();

private:

};

