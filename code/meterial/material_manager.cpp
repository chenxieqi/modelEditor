//--------------------------------------------------------------------------------
//  マテリアルマネージャー
//--------------------------------------------------------------------------------
#include "material_manager.h"
#include "texture_manager.h"
#include "game_system.h"
#include <fstream>
#include <cassert>
#include "library\cereal\archives\binary.hpp"
using namespace cereal;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void MaterialManager::Init()
{

}
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MaterialManager::Uninit()
{
    for (auto iterator : m_materials)
    {
        if (iterator.second.pointer != NULL)
        {
            DisTexture(*iterator.second.pointer);
            SAFE_DELETE(iterator.second.pointer);
        }
    }
    m_materials.clear();
}


//--------------------------------------------------------------------------------
//  マテリアルを使う
//--------------------------------------------------------------------------------
void MaterialManager::Use(const string& material_name,
    const string& color_texture,
    const string& normal_texture,
    const D3DXCOLOR& ambient,
    const D3DXCOLOR& diffuse,
    const D3DXCOLOR& specular,
    const D3DXCOLOR& emissive,
    const float& power)
{
    size_t key = hash<string>()(material_name);
    auto iterator = m_materials.find(key);
    if (iterator != m_materials.end())
    {
        iterator->second.material_number++;
    }
    else {
        MaterialInfo materialInfo;
        materialInfo.pointer = new Material;
        materialInfo.pointer->m_color_texture = color_texture;
        materialInfo.pointer->m_nomal_texture = normal_texture;
        materialInfo.pointer->m_ambient = ambient;
        materialInfo.pointer->m_diffuse = diffuse;
        materialInfo.pointer->m_emissive = emissive;
        materialInfo.pointer->m_specular = specular;
        materialInfo.pointer->m_power = power;
        materialInfo.material_number = 0;
        m_materials.emplace(key, materialInfo);

        UseTexture(*materialInfo.pointer);
    }
}

void MaterialManager::Use(const string& material_name)
{
    size_t key = hash<string>()(material_name);
    auto iterator = m_materials.find(key);
    if (iterator != m_materials.end())
    {
        iterator->second.material_number++;
    }
    else {
        MaterialInfo materialInfo;
        materialInfo.pointer = loadMaterialFromFile(material_name);
        m_materials.emplace(key, materialInfo);
        if (materialInfo.pointer != NULL)
        {
            UseTexture(*materialInfo.pointer);
        }
    }
}

//--------------------------------------------------------------------------------
//  マテリアルを破棄
//--------------------------------------------------------------------------------
void MaterialManager::DisUse(const string& material_name)
{
    size_t key = hash<string>()(material_name);
    auto iterator = m_materials.find(key);
    if (iterator != m_materials.end())
    {
        iterator->second.material_number--;

        if (iterator->second.material_number == 0)
        {
            if (iterator->second.pointer != NULL)
            {
                DisTexture(*iterator->second.pointer);

                delete iterator->second.pointer;
            }
            m_materials.erase(iterator);
        }
    }
}

//--------------------------------------------------------------------------------
//  マテリアルの取得
//--------------------------------------------------------------------------------
Material* MaterialManager::GetMaterial(const string& material_name)
{
    size_t key = hash<string>()(material_name);
    auto iterator = m_materials.find(key);
    if (iterator != m_materials.end())
    {
        return iterator->second.pointer;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
//  ファイルからマテリアルの読み込み
//--------------------------------------------------------------------------------

Material* MaterialManager::loadMaterialFromFile(const string& material_name)
{
    string path = "data/material/" + material_name + ".material";

    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return nullptr;
    }
    // Create an output archive
    BinaryInputArchive archive(file);
    auto material = new Material;
    int buffer_size=0;
    string buffer;

    archive.loadBinary(&buffer_size, sizeof(buffer_size));
    buffer.resize(buffer_size);
    archive.loadBinary(&buffer[0], buffer_size);
    material->m_color_texture = string(buffer.begin(), buffer.end());

    archive.loadBinary(&buffer_size, sizeof(buffer_size));
    buffer.resize(buffer_size);
    archive.loadBinary(&buffer[0], buffer_size);
    material->m_nomal_texture = string(buffer.begin(), buffer.end());
   
    archive.loadBinary(&material->m_ambient, sizeof(material->m_ambient));
    archive.loadBinary(&material->m_diffuse, sizeof(material->m_diffuse));
    archive.loadBinary(&material->m_specular, sizeof(material->m_specular));
    archive.loadBinary(&material->m_emissive, sizeof(material->m_emissive));
    archive.loadBinary(&material->m_power, sizeof(material->m_power));

    file.close();
    return material;
}

//--------------------------------------------------------------------------------
//  テクスチャの使用
//--------------------------------------------------------------------------------
void MaterialManager::UseTexture(const Material& material)
{
    auto& texture_manager = GameSystem::Instance().GetTextureManager();
    texture_manager.Use(material.m_color_texture);
    texture_manager.Use(material.m_nomal_texture);
}

//--------------------------------------------------------------------------------
//  テクスチャの破棄
//--------------------------------------------------------------------------------
void MaterialManager::DisTexture(const Material& material)
{
    auto& texture_manager = GameSystem::Instance().GetTextureManager();
    texture_manager.DisUse(material.m_color_texture);
    texture_manager.DisUse(material.m_nomal_texture);
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
MaterialManager* MaterialManager::Create()
{
    MaterialManager* materialmanager = new MaterialManager;
    return materialmanager;
}
