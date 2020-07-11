//--------------------------------------------------------------------------------
// オブジェクトマネージャー
//--------------------------------------------------------------------------------
#pragma once
#include <unordered_map>
#include <string>

class GameObject;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class GameobjectManager
{
public:
    GameobjectManager();
    ~GameobjectManager();
    //--------------------------------------------------------------------------------
    // 基本関数
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update();
    void LateUpdate();
    //--------------------------------------------------------------------------------
    // 生成
    //--------------------------------------------------------------------------------
    static GameobjectManager* Create();
    //--------------------------------------------------------------------------------
    // オブジェクト追加
    //--------------------------------------------------------------------------------
    void AddGameObject(GameObject* gameobject);
    //--------------------------------------------------------------------------------
    // クリア
    //--------------------------------------------------------------------------------
    void Clear();
private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    std::unordered_multimap<std::string, GameObject*> m_gameobject;
};

