//--------------------------------------------------------------------------------
// ‚QDƒŒƒ“ƒ_ƒ‰[
//--------------------------------------------------------------------------------
#include "render2D.h"
#include "render_system.h"
#include "game_system.h"


Render2D::~Render2D()
{
}

void Render2D::Update()
{
    GameSystem::Instance().GetRenderSystem().Register(this);
}


void Render2D::RenderBy(const RenderSystem& render_system)
{
    render_system.Render2dMesh(m_mesh_name);
}