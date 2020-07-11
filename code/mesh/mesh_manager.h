//--------------------------------------------------------------------------------
//  メッシュマネージャー
//--------------------------------------------------------------------------------

#pragma once
#include "main.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

using namespace std;
//--------------------------------------------------------------------------------
//  構造体
//--------------------------------------------------------------------------------
enum MeshType
{
    TYPE_3DMESH = 0,
    TYPE_3DSKIN,
    TYPE_2DMESH
};

typedef struct
{
    D3DXVECTOR3 pos; // rhwを外した
    D3DXVECTOR2 texcoord;
    D3DXCOLOR color;
}VERTEX_2D;

typedef struct
{
    D3DXVECTOR3 pos; 
    D3DXVECTOR3 normal; // 法線
    D3DXVECTOR2 texcoord;
    D3DXCOLOR color;
}VERTEX_3D;

struct Mesh
{
    Mesh() :type(TYPE_3DMESH),
        primitive_type(D3DPT_TRIANGLELIST),
        vertex_number(0),
        index_number(0),
        polygon_number(0),
        vertex_buffer(nullptr),
        index_buffer(nullptr) {}
    ~Mesh() {
        SAFE_RELEASE(vertex_buffer);
        SAFE_RELEASE(index_buffer);
    }

    MeshType type;
    D3DPRIMITIVETYPE primitive_type;
    int vertex_number;
    int index_number;
    int polygon_number;
    LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
    LPDIRECT3DINDEXBUFFER9  index_buffer;

};
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class MeshManager
{
public:
    MeshManager();
    ~MeshManager();
    //--------------------------------------------------------------------------------
    //  基本関数
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update(const string& name,const vector<VERTEX_3D>& vertexes,const list<int>& indexes);
    //--------------------------------------------------------------------------------
    //  使用・破棄
    //--------------------------------------------------------------------------------
    void Use(const string& mesh_name);
    void DisUse(const string& mesh_name);
    void DisUseImmediately(const string& mesh_name);
    void Make(const string& mesh_name, const D3DPRIMITIVETYPE type, const vector<VERTEX_3D>& vertexes, const vector<int>& index, const int polygon_number);
    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    Mesh* GetMesh(const string& mesh_name);
    //--------------------------------------------------------------------------------
    //  セーブ
    //--------------------------------------------------------------------------------
    void SaveMeshToFile(const string& mesh_name, const string& file_name);
    //--------------------------------------------------------------------------------
    //  生成
    //--------------------------------------------------------------------------------
    static MeshManager* Create();
private:
    //--------------------------------------------------------------------------------
    //  構造体
    //--------------------------------------------------------------------------------
    struct MeshInfo// smart point
    {
        MeshInfo():user_number(1), pointer(nullptr){}
        int user_number;
        Mesh* pointer;
    };
    //--------------------------------------------------------------------------------
    //  メンバー変数
    //--------------------------------------------------------------------------------
    unordered_map<size_t, MeshInfo> m_meshs;

    //--------------------------------------------------------------------------------
    //  ロード
    //--------------------------------------------------------------------------------
    MeshInfo LoadFromMeshFile(const string& mesh_name);
    MeshInfo LoadXFile(const string& mesh_name);
    //--------------------------------------------------------------------------------
    //  メッシュ生成関数
    //--------------------------------------------------------------------------------
    MeshInfo CreateCube(void);
    MeshInfo CreateSphere(void);
    MeshInfo CreateSkyBox(void);
    MeshInfo CreatePolygon2D(void);
    MeshInfo CreatePolygon3D(void);
    MeshInfo CreateMeshField(void);
    //--------------------------------------------------------------------------------
    //  バッファ生成関数
    //--------------------------------------------------------------------------------
    bool CreateBuffer2D(Mesh* mesh)const;
    bool CreateBuffer3D(Mesh* mesh)const;
   
};
