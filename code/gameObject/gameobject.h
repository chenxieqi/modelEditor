//--------------------------------------------------------------------------------
// オブジェクト
//--------------------------------------------------------------------------------
#pragma once

#include <unordered_map>
#include <string>

class Component;

using namespace std;
//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class GameObject
{
public:
    GameObject();
    ~GameObject();
    //--------------------------------------------------------------------------------
    // 基本関数
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update();
    void LateUpdate();
    //--------------------------------------------------------------------------------
    // コンポーネント追加
    //--------------------------------------------------------------------------------
    void AddComponent(Component* component);
    //--------------------------------------------------------------------------------
    // セッター
    //--------------------------------------------------------------------------------
    void SetName(std::string name) { m_name = name; }
    void SetAlive(const bool& alive);
    void SetActive(const bool& active);
    //--------------------------------------------------------------------------------
    // ゲッター
    //--------------------------------------------------------------------------------
    Component* GetComponent(std::string name);
    auto& GetName() const{ return m_name; }
    auto& IsAlive() const{ return m_alive; }
    
private:
    //--------------------------------------------------------------------------------
    // メンバー変数
    //--------------------------------------------------------------------------------
    unordered_multimap<string,Component*>   m_component;
    string                                  m_name;
    bool                                    m_alive;
    bool                                    m_active;
};

