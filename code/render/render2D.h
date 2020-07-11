//--------------------------------------------------------------------------------
// ‚QDƒŒƒ“ƒ_ƒ‰[
//--------------------------------------------------------------------------------
#pragma once
#include "renderer.h"
class Render2D :
    public Renderer
{
public:
    Render2D(GameObject* game_object) :Renderer(game_object,"Render2D") {}
    ~Render2D();
    void Update()override;
    void RenderBy(const RenderSystem& render_system)override;

};

