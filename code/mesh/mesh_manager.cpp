//--------------------------------------------------------------------------------
//  メッシュマネージャー
//--------------------------------------------------------------------------------
#include "mesh_manager.h"
#include "render_system.h"
#include "game_system.h"
#include <fstream>
#include <cassert>
#include "library\cereal\archives\binary.hpp"

using namespace cereal;

MeshManager::MeshManager()
{
}
MeshManager::~MeshManager()
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
void MeshManager::Init()
{

}

//--------------------------------------------------------------------------------
//  終了
//--------------------------------------------------------------------------------
void MeshManager::Uninit()
{
    for (auto iterator = m_meshs.begin(); iterator != m_meshs.end();)
    {
        SAFE_DELETE(iterator->second.pointer);
        iterator = m_meshs.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void MeshManager::Update(const string& name, const vector<VERTEX_3D>& vertexes, const list<int>& indexes)
{
    size_t key = hash<string>()(name);
    auto iterator = m_meshs.find(key);
    if (iterator == m_meshs.end()) { return; }

    VERTEX_3D* pVertex;
    iterator->second.pointer->vertex_buffer->Lock(0, 0, (void**)&pVertex, 0);
    for (int index : indexes)
    {
        pVertex[index] = vertexes[index];
    }
    iterator->second.pointer->vertex_buffer->Unlock();

}

//--------------------------------------------------------------------------------
//  使う
//--------------------------------------------------------------------------------
void MeshManager::Use(const string& mesh_name)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator != m_meshs.end())
    {
        iterator->second.user_number++;
    }
    else {
        MeshInfo meshInfo;
        if (mesh_name.find(".x") != string::npos)meshInfo = LoadXFile(mesh_name);
        else if (mesh_name.find(".mesh") != string::npos)meshInfo = LoadFromMeshFile(mesh_name);
        else if (mesh_name._Equal("Cube"))meshInfo = CreateCube();
        else if (mesh_name._Equal("Sphere"))meshInfo = CreateSphere();
        else if (mesh_name._Equal("Skybox"))meshInfo = CreateSkyBox();
        else if (mesh_name._Equal("Polygon2D"))meshInfo = CreatePolygon2D();
        else if (mesh_name._Equal("Polygon3D"))meshInfo = CreatePolygon3D();
        else if (mesh_name._Equal("Meshfield"))meshInfo = CreateMeshField();
        else { return; }
        m_meshs.emplace(key, meshInfo);
    }
   
}

//--------------------------------------------------------------------------------
//  使わない
//--------------------------------------------------------------------------------
void MeshManager::DisUse(const string& mesh_name)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator != m_meshs.end())
    {
        iterator->second.user_number--;
        if (iterator->second.user_number == 0)
        {
            delete iterator->second.pointer;
            m_meshs.erase(iterator);
        }
    }
}

//--------------------------------------------------------------------------------
//  すぐ消す処理
//--------------------------------------------------------------------------------
void MeshManager::DisUseImmediately(const string& mesh_name)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator == m_meshs.end())
    {
        return;
    }
    auto pointer= iterator->second.pointer;
    m_meshs.erase(iterator);
    SAFE_DELETE(pointer);
    
}

//--------------------------------------------------------------------------------
//  メッシュ作り
//--------------------------------------------------------------------------------
void MeshManager::Make(const string& mesh_name, const D3DPRIMITIVETYPE type, const vector<VERTEX_3D>& vertexes, const vector<int>& index, const int polygon_number)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator != m_meshs.end())
    {
        iterator->second.user_number++;
        return;
    }
    else {
        MeshInfo meshInfo;
        meshInfo.pointer = new Mesh;
        meshInfo.pointer->type = TYPE_3DMESH;
        meshInfo.pointer->primitive_type = type;
        meshInfo.pointer->polygon_number = polygon_number;
        meshInfo.pointer->vertex_number = vertexes.size();
        meshInfo.pointer->index_number = index.size();

        CreateBuffer3D(meshInfo.pointer);
        VERTEX_3D* pV;
        meshInfo.pointer->vertex_buffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
        memcpy(pV, &vertexes[0], sizeof(VERTEX_3D)*meshInfo.pointer->vertex_number);
        meshInfo.pointer->vertex_buffer->Unlock();

        LPWORD pIndex;
        meshInfo.pointer->index_buffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
        for (int count=0;count<meshInfo.pointer->index_number;count++)
        {
            pIndex[count] = index[count];
        }
        meshInfo.pointer->index_buffer->Unlock();

        m_meshs.emplace(key, meshInfo);
    }
}

//--------------------------------------------------------------------------------
//  ゲットメッシュ
//--------------------------------------------------------------------------------
Mesh* MeshManager::GetMesh(const string& mesh_name)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator != m_meshs.end())
    {
        return iterator->second.pointer;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
//  ファイルからロードメッシュ
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadFromMeshFile(const string& mesh_name)
{
    string path = "data/mesh/" + mesh_name;
    MeshInfo meshinfo;
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return meshinfo;
    }
    BinaryInputArchive archive(file);

    meshinfo.pointer = new Mesh;
    meshinfo.pointer->type = TYPE_3DMESH;
    archive.loadBinary(&meshinfo.pointer->primitive_type, sizeof(meshinfo.pointer->primitive_type));
    archive.loadBinary(&meshinfo.pointer->vertex_number, sizeof(meshinfo.pointer->vertex_number));
    archive.loadBinary(&meshinfo.pointer->index_number, sizeof(meshinfo.pointer->index_number));
    archive.loadBinary(&meshinfo.pointer->polygon_number, sizeof(meshinfo.pointer->polygon_number));

    if (!CreateBuffer3D(meshinfo.pointer))
    {
        assert(meshinfo.pointer);
        file.close();
        return meshinfo;
    }
    
    VERTEX_3D* pV;
    meshinfo.pointer->vertex_buffer->Lock(0, 0, (void**)&pV, 0);
    archive.loadBinary(pV, sizeof(VERTEX_3D) * meshinfo.pointer->vertex_number);
    meshinfo.pointer->vertex_buffer->Unlock();

    WORD* pIndex;
    meshinfo.pointer->index_buffer->Lock(0, 0, (void**)&pIndex, 0);
    archive.loadBinary(pIndex, sizeof(WORD) * meshinfo.pointer->index_number);
    meshinfo.pointer->index_buffer->Unlock();

    file.close();
    return meshinfo;

}

//--------------------------------------------------------------------------------
//  ファイルからXファイルをロードする
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::LoadXFile(const string& mesh_name)
{
    MeshInfo meshInfo;
    return meshInfo;
}
//--------------------------------------------------------------------------------
//  キューブを作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateCube(void)
{
    WORD index[] = { 0,1,2,
        1,3,2,
        4,5,6,
        6,5,7,
        8,9,10,
        10,9,11,
        12,13,14,
        14,13,15,
        16,17,18,
        18,17,19,
        20,21,22,
        22,21,23
    };

    VERTEX_3D v1[] = {
        
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f),  D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),   D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f) , D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,0.5f),   D3DXVECTOR3(1.0f,0.0f,0.0f)  ,D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f), D3DXVECTOR3(1.0f,0.0f,0.0f)  ,D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f) , D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    
    { D3DXVECTOR3(0.5f,0.5f,0.5f),   D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,0.5f),  D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,0.5f),  D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    
    { D3DXVECTOR3(-0.5f,0.5f,0.5f) ,  D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f),  D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f),  D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    
    { D3DXVECTOR3(-0.5f,0.5f,0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,0.5f),   D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,0.5f),   D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },

    };

    Mesh* mesh = new Mesh;
    mesh->index_number = 36;
    mesh->vertex_number = 24;
    mesh->polygon_number = 12;

    CreateBuffer3D(mesh);
    VERTEX_3D* pV;
    mesh->vertex_buffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
    memcpy(pV, &v1, sizeof(v1));
    //pV->color = m_PolygonColor;
    mesh->vertex_buffer->Unlock();

    LPWORD pIndex;
    mesh->index_buffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
    // インデックスデータを書き込み
    memcpy(pIndex, &index, sizeof(index));

    mesh->index_buffer->Unlock();

    MeshInfo meshInfo;
    meshInfo.pointer = mesh;

    return meshInfo;
}
//--------------------------------------------------------------------------------
//  球体を作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSphere(void)
{

    MeshInfo meshInfo;
    return meshInfo;
}
//--------------------------------------------------------------------------------
//  スカイボックスを作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateSkyBox(void)
{
    WORD index[] = { 0,1,2,
        1,3,2,
        4,5,6,
        6,5,7,
        8,9,10,
        10,9,11,
        12,13,14,
        14,13,15,
        16,17,18,
        18,17,19,
        20,21,22,
        22,21,23
    };
    VERTEX_3D v1[] = {

     // 後ろ
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DXVECTOR2(0.75f,0.5f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),   D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR2(0.75f,0.25f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DXVECTOR2(1.0f,0.5f)  ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f),  D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR2(1.0f,0.25f) ,D3DCOLOR_RGBA(255,255,255,255) },

    // 右										   
    { D3DXVECTOR3(0.5f,-0.5f,0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f) , D3DXVECTOR2(0.5f,0.5f)  ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,0.5f),   D3DXVECTOR3(1.0f,0.0f,0.0f)  ,D3DXVECTOR2(0.5f,0.25f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f), D3DXVECTOR3(1.0f,0.0f,0.0f)  ,D3DXVECTOR2(0.75f,0.5f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f) , D3DXVECTOR2(0.75f,0.25f),D3DCOLOR_RGBA(255,255,255,255) },
    

    // 前										   
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f) ,D3DXVECTOR2(0.25f,0.5f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,0.5f), D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR2(0.25f,0.25f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,0.5f), D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR2(0.5f,0.5f)  ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,0.5f),  D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR2(0.5f,0.25f) ,D3DCOLOR_RGBA(255,255,255,255) },
    

    // 左										   
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f) ,D3DXVECTOR2(0.0f,0.5f)  ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DXVECTOR2(0.0f,0.25f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DXVECTOR2(0.25f,0.5f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,0.5f),  D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DXVECTOR2(0.25f,0.25f),D3DCOLOR_RGBA(255,255,255,255) },
    
    // 上										   
    { D3DXVECTOR3(0.5f,0.5f,-0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f) ,D3DXVECTOR2(0.25f,0.25f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,0.5f,0.5f),   D3DXVECTOR3(0.0f,1.0f,0.0f) ,D3DXVECTOR2(0.25f,0.0f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,-0.5f), D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXVECTOR2(0.5f,0.25f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,0.5f,0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f) ,D3DXVECTOR2(0.5f,0.0f)  ,D3DCOLOR_RGBA(255,255,255,255) },
    
    // 下										  
    { D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f,-1.0f,0.0f), D3DXVECTOR2(0.25f,0.75f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-0.5f,-0.5f,0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f) ,D3DXVECTOR2(0.25f,0.5f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f) ,D3DXVECTOR2(0.5f,0.75f) ,D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(0.5f,-0.5f,0.5f),   D3DXVECTOR3(0.0f,-1.0f,0.0f) ,D3DXVECTOR2(0.5f,0.5f)  ,D3DCOLOR_RGBA(255,255,255,255) },

    };

    Mesh* mesh = new Mesh;
    mesh->index_number = 36;
    mesh->vertex_number = 24;
    mesh->polygon_number = 12;

    CreateBuffer3D(mesh);
    VERTEX_3D* pV;
    mesh->vertex_buffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
    memcpy(pV, &v1, sizeof(v1));
    //pV->color = m_PolygonColor;
    mesh->vertex_buffer->Unlock();

    LPWORD pIndex;
    mesh->index_buffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
    // インデックスデータを書き込み
    memcpy(pIndex, &index, sizeof(index));

    mesh->index_buffer->Unlock();

    MeshInfo meshInfo;
    meshInfo.pointer = mesh;

    return meshInfo;
}
//--------------------------------------------------------------------------------
//  ２Dのポリゴンを作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreatePolygon2D(void)
{
    WORD index[] = {
        0,1,2,3
    };
    VERTEX_2D v1[] = {
    { D3DXVECTOR3(-1.0f, 1.0f,0.0f) , D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(1.0f,  1.0f,0.0f) , D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-1.0f,-1.0f,0.0f)  ,D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(1.0f, -1.0f,0.0f) , D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) }
    };

    Mesh* mesh = new Mesh;
    mesh->type = MeshType::TYPE_2DMESH;
    mesh->index_number = 4;
    mesh->vertex_number = 4;
    mesh->polygon_number = 2;
    mesh->primitive_type = D3DPT_TRIANGLESTRIP;

    CreateBuffer2D(mesh);
    VERTEX_2D* pV;
    mesh->vertex_buffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
    memcpy(pV, &v1, sizeof(v1));
    mesh->vertex_buffer->Unlock();

    LPWORD pIndex;
    mesh->index_buffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
    memcpy(pIndex, &index, sizeof(index));
    mesh->index_buffer->Unlock();

    MeshInfo meshInfo;
    meshInfo.pointer = mesh;

    return meshInfo;
}
//--------------------------------------------------------------------------------
//  ３Dのポリゴンんを作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreatePolygon3D(void)
{
    WORD index[] = {
        0,1,2,
        1,3,2
    };
    VERTEX_3D v1[] = {
    { D3DXVECTOR3(-1.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f)   ,D3DXVECTOR2(0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(1.0f, 1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f)  , D3DXVECTOR2(1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(-1.0f,-1.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DXVECTOR2(0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
    { D3DXVECTOR3(1.0f, -1.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR2(1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) }
    };

    Mesh* mesh = new Mesh;
    mesh->index_number = 6;
    mesh->vertex_number = 4;
    mesh->polygon_number = 2;

    CreateBuffer3D(mesh);
    VERTEX_3D* pV;
    mesh->vertex_buffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
    memcpy(pV, &v1, sizeof(v1));
    //pV->color = m_PolygonColor;
    mesh->vertex_buffer->Unlock();

    LPWORD pIndex;
    mesh->index_buffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
    // インデックスデータを書き込み
    memcpy(pIndex, &index, sizeof(index));

    mesh->index_buffer->Unlock();

    MeshInfo meshInfo;
    meshInfo.pointer = mesh;

    return meshInfo;
}
//--------------------------------------------------------------------------------
//  メッシュフィールドを作成
//--------------------------------------------------------------------------------
MeshManager::MeshInfo MeshManager::CreateMeshField(void)
{

    MeshInfo meshInfo;
    return meshInfo;
}
//--------------------------------------------------------------------------------
//  ２Dバッファを作成
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer2D(Mesh* mesh)const
{
    LPDIRECT3DDEVICE9 device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    HRESULT hr;
    hr=device->CreateVertexBuffer(sizeof(VERTEX_2D) * mesh->vertex_number, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mesh->vertex_buffer, NULL);
    if(FAILED(hr)){ return false;}
    hr=device->CreateIndexBuffer(sizeof(WORD) * mesh->index_number,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,D3DPOOL_MANAGED, &mesh->index_buffer, NULL);
    if (FAILED(hr)){return false;}
    return true;
}
//--------------------------------------------------------------------------------
//  ３Dバッファを作成
//--------------------------------------------------------------------------------
bool MeshManager::CreateBuffer3D(Mesh* mesh)const
{
    LPDIRECT3DDEVICE9 device = GameSystem::Instance().GetRenderSystem().GetD3DDevice();
    HRESULT hr;
    hr = device->CreateVertexBuffer(sizeof(VERTEX_3D) * mesh->vertex_number, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mesh->vertex_buffer, NULL);
    if (FAILED(hr)) { return false; }
    hr = device->CreateIndexBuffer(sizeof(WORD) * mesh->index_number, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mesh->index_buffer, NULL);
    if (FAILED(hr)) { return false; }
    return true;
}

//--------------------------------------------------------------------------------
//  ファイルに書き出し
//--------------------------------------------------------------------------------
void MeshManager::SaveMeshToFile(const string& mesh_name, const string& file_name)
{
    size_t key = hash<string>()(mesh_name);
    auto iterator = m_meshs.find(key);
    if (iterator == m_meshs.end()){return ;}

    auto mesh = iterator->second.pointer;

    string path = "data/mesh/" + file_name + ".mesh";
    ofstream file(path, ios::binary);
    if (!file.is_open())return;
    BinaryOutputArchive archive(file);

    archive.saveBinary(&mesh->primitive_type, sizeof(mesh->primitive_type));
    archive.saveBinary(&mesh->vertex_number, sizeof(mesh->vertex_number));
    archive.saveBinary(&mesh->index_number, sizeof(mesh->index_number));
    archive.saveBinary(&mesh->polygon_number, sizeof(mesh->polygon_number));

    VERTEX_3D* pV;
    mesh->vertex_buffer->Lock(0, 0, (void**)&pV, 0);
    archive.saveBinary(pV, sizeof(VERTEX_3D)*mesh->vertex_number);
    mesh->vertex_buffer->Unlock();

    WORD* pIndex;
    mesh->index_buffer->Lock(0, 0, (void**)&pIndex, 0);
    archive.saveBinary(pIndex, sizeof(WORD)*mesh->index_number);
    mesh->index_buffer->Unlock();

    file.close();
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
MeshManager* MeshManager::Create()
{
    MeshManager* meshmanager = new MeshManager;
    return meshmanager;
}
