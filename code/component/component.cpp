//--------------------------------------------------------------------------------
// ÉpÅ[Éc
//--------------------------------------------------------------------------------
#include "component.h"

Component::Component(GameObject* game_object,std::string name)
{
    m_game_object = game_object;
    m_name = name;
}

Component::~Component()
{
}