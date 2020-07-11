//--------------------------------------------------------------------------------
// ‚RDƒŒƒ“ƒ_ƒ‰[
//--------------------------------------------------------------------------------
#include "render3D.h"
#include "render_system.h"
#include "game_system.h"


Render3D::~Render3D()
{
}

void Render3D::Update()
{
    GameSystem::Instance().GetRenderSystem().Register(this);
}

void Render3D::RenderBy(const RenderSystem& render_system)
{
    render_system.Render3dMesh(m_mesh_name);
}