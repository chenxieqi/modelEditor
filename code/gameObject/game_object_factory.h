//--------------------------------------------------------------------------------
// ゲームオブジェクトファクトリー
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include <string>
#include "library\cereal\archives\binary.hpp"
class GameObject;
class Transform;

using namespace std;
using namespace cereal;
//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class GameObjectFactory
{
public:
    //--------------------------------------------------------------------------------
    // フィールド生成
    //--------------------------------------------------------------------------------
    static void CreateField(const string& name);
    //--------------------------------------------------------------------------------
    // スカイボックス生成
    //--------------------------------------------------------------------------------
    static void CreateSkyBox(void);
    //--------------------------------------------------------------------------------
    // エディター生成
    //--------------------------------------------------------------------------------
    static void CreateEdit(void);
    //--------------------------------------------------------------------------------
    // モデル生成
    //--------------------------------------------------------------------------------
    static GameObject* CreateModel(const string& name,const D3DXVECTOR3& position,const D3DXQUATERNION& rotation,const D3DXVECTOR3& scaling );
   
private:
    GameObjectFactory(){}
    ~GameObjectFactory(){}
    //--------------------------------------------------------------------------------
    // 子供生成
    //--------------------------------------------------------------------------------
    static GameObject* CreateChild(Transform* parent, BinaryInputArchive& archive);
};

