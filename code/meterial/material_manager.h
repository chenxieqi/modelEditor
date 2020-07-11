//--------------------------------------------------------------------------------
//  �}�e���A���}�l�[�W���[
//--------------------------------------------------------------------------------
#pragma once
#include <string>
#include <unordered_map>
#include "main.h"

using namespace std;
//--------------------------------------------------------------------------------
//  �}�e���A���N���X
//--------------------------------------------------------------------------------
class Material
{
public:
    Material(){}
    ~Material(){}
    //--------------------------------------------------------------------------------
    //  �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    string m_color_texture;
    string m_nomal_texture;
    D3DXCOLOR m_ambient;
    D3DXCOLOR m_diffuse;
    D3DXCOLOR m_specular;
    D3DXCOLOR m_emissive;
    float m_power;
};

//--------------------------------------------------------------------------------
//  �}�e���A���}�l�[�W���[�N���X
//--------------------------------------------------------------------------------
class MaterialManager
{
public:
    MaterialManager();
    ~MaterialManager();
    //--------------------------------------------------------------------------------
    //  ��{�֐�
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    //--------------------------------------------------------------------------------
    //  ����
    //--------------------------------------------------------------------------------
    static MaterialManager* Create();

    //--------------------------------------------------------------------------------
    //  �g�p�E�j���֐�
    //--------------------------------------------------------------------------------
    //�@�V�����}�e���A�����g��
    void Use(const string& material_name,
        const string& color_texture,
        const string& normal_texture=string(),
        const D3DXCOLOR& ambient = D3DXCOLOR(0, 0, 0, 0),
        const D3DXCOLOR& diffuse = D3DXCOLOR(0, 0, 0, 0), 
        const D3DXCOLOR& specular = D3DXCOLOR(0, 0, 0, 0), 
        const D3DXCOLOR& emissive = D3DXCOLOR(0, 0, 0, 0), 
        const float& power=0);
    //�@�t�@�C������ǂݍ��񂾃}�e���A���g��
    void Use(const string& material_name);
    void DisUse(const string& material_name);
    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    Material* GetMaterial(const string& material_name);
    //--------------------------------------------------------------------------------
    //  ���[�h
    //--------------------------------------------------------------------------------
    Material* loadMaterialFromFile(const string& material_name);
    //--------------------------------------------------------------------------------
    //  �e�N�X�`���g�p�E�j��
    //--------------------------------------------------------------------------------
    void UseTexture(const Material& material);
    void DisTexture(const Material& material);

private:
    //--------------------------------------------------------------------------------
    //  �\����
    //--------------------------------------------------------------------------------
    struct MaterialInfo
    {
        MaterialInfo():material_number(1),pointer(nullptr){}
        int material_number;
        Material* pointer;
    };
    //--------------------------------------------------------------------------------
    //  �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    unordered_map<size_t, MaterialInfo> m_materials;
};