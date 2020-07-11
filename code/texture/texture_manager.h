//--------------------------------------------------------------------------------
// �e�N�X�`���}�l�[�W���[
//--------------------------------------------------------------------------------
#pragma once
#include <string>
#include <unordered_map>
#include "main.h"

using namespace std;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class TextureManager
{
public:
    TextureManager();
    ~TextureManager();
    //--------------------------------------------------------------------------------
    // ��{�֐�
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update();
    //--------------------------------------------------------------------------------
    // �g���E�j��
    //--------------------------------------------------------------------------------
    void Use(const string& texture_name);
    void DisUse(const string& texture_name);
    //--------------------------------------------------------------------------------
    // �Q�b�^�[
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 GetTexture(const string& texture_name) const;
    //--------------------------------------------------------------------------------
    // ����
    //--------------------------------------------------------------------------------
    static TextureManager* Create();
private:
    //--------------------------------------------------------------------------------
    // �\����
    //--------------------------------------------------------------------------------
    struct TextureInfo
    {
        TextureInfo() :m_texture_number(1), m_texture(nullptr){}
        ~TextureInfo() {}
        int m_texture_number;
        LPDIRECT3DTEXTURE9 m_texture;
    };
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    unordered_map<size_t, TextureInfo> m_textures;

};