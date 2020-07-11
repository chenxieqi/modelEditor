//--------------------------------------------------------------------------------
// ‚RDƒŒƒ“ƒ_ƒ‰[
//--------------------------------------------------------------------------------
#pragma once
#include "renderer.h"
class Render3D :
    public Renderer
{
public:
    Render3D(GameObject* game_object) :Renderer(game_object,"Render3D") {}
    ~Render3D();
    void Update()override;
    void RenderBy(const RenderSystem& render_system)override;

};

