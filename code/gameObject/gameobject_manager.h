//--------------------------------------------------------------------------------
// �I�u�W�F�N�g�}�l�[�W���[
//--------------------------------------------------------------------------------
#pragma once
#include <unordered_map>
#include <string>

class GameObject;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class GameobjectManager
{
public:
    GameobjectManager();
    ~GameobjectManager();
    //--------------------------------------------------------------------------------
    // ��{�֐�
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update();
    void LateUpdate();
    //--------------------------------------------------------------------------------
    // ����
    //--------------------------------------------------------------------------------
    static GameobjectManager* Create();
    //--------------------------------------------------------------------------------
    // �I�u�W�F�N�g�ǉ�
    //--------------------------------------------------------------------------------
    void AddGameObject(GameObject* gameobject);
    //--------------------------------------------------------------------------------
    // �N���A
    //--------------------------------------------------------------------------------
    void Clear();
private:
    //--------------------------------------------------------------------------------
    // �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    std::unordered_multimap<std::string, GameObject*> m_gameobject;
};
