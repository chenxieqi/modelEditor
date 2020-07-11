//--------------------------------------------------------------------------------
// �e�N�X�`���}�l�[�W���[
//--------------------------------------------------------------------------------
#include "texture_manager.h"
#include "game_system.h"
#include "render_system.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}
//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void TextureManager::Init()
{

}
//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void TextureManager::Uninit()
{
    for (auto& iterator:m_textures)
    {
        SAFE_RELEASE(iterator.second.m_texture);
    }
    m_textures.clear();
}
//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void TextureManager::Update()
{

}
//--------------------------------------------------------------------------------
// �g��
//--------------------------------------------------------------------------------
void TextureManager::Use(const string& texture_name)
{
    auto key = hash<string>()(texture_name);
    auto iterator = m_textures.find(key);
    if (iterator != m_textures.end())
    {
        iterator->second.m_texture_number++;
    }
    else
    {
        TextureInfo textureinfo;
        auto device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
        string path = "data/texture/" + texture_name;
        HRESULT hr = D3DXCreateTextureFromFile(device,path.c_str(), &textureinfo.m_texture);
        if (FAILED(hr)) 
        {
            //MessageBox(NULL, "�e�N�X�`���t�@�C���ǂݍ��ݎ��s", "�G���[", MB_OK);
        }
        m_textures.emplace(key,textureinfo);
    }
}
//--------------------------------------------------------------------------------
// �j��
//--------------------------------------------------------------------------------
void TextureManager::DisUse(const string& texture_name)
{
    auto key = hash<string>()(texture_name);
    auto iterator = m_textures.find(key);
    if (iterator != m_textures.end())
    {
        iterator->second.m_texture_number--;
        if (iterator->second.m_texture_number == 0)
        {
            SAFE_RELEASE(iterator->second.m_texture);
            m_textures.erase(iterator);
        }
    }
 
}
//--------------------------------------------------------------------------------
// �Q�b�g�e�N�X�`��
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TextureManager::GetTexture(const string& texture_name) const
{
    auto key = hash<string>()(texture_name);
    auto iterator = m_textures.find(key);
    if (iterator != m_textures.end())
    {
        return iterator->second.m_texture;
    }
    return NULL;
}
//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
TextureManager* TextureManager::Create()
{
    TextureManager* texturemanager = new TextureManager;
    return texturemanager;
}