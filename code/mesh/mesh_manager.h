//--------------------------------------------------------------------------------
//  ���b�V���}�l�[�W���[
//--------------------------------------------------------------------------------

#pragma once
#include "main.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

using namespace std;
//--------------------------------------------------------------------------------
//  �\����
//--------------------------------------------------------------------------------
enum MeshType
{
    TYPE_3DMESH = 0,
    TYPE_3DSKIN,
    TYPE_2DMESH
};

typedef struct
{
    D3DXVECTOR3 pos; // rhw���O����
    D3DXVECTOR2 texcoord;
    D3DXCOLOR color;
}VERTEX_2D;

typedef struct
{
    D3DXVECTOR3 pos; 
    D3DXVECTOR3 normal; // �@��
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
//  �N���X
//--------------------------------------------------------------------------------
class MeshManager
{
public:
    MeshManager();
    ~MeshManager();
    //--------------------------------------------------------------------------------
    //  ��{�֐�
    //--------------------------------------------------------------------------------
    void Init();
    void Uninit();
    void Update(const string& name,const vector<VERTEX_3D>& vertexes,const list<int>& indexes);
    //--------------------------------------------------------------------------------
    //  �g�p�E�j��
    //--------------------------------------------------------------------------------
    void Use(const string& mesh_name);
    void DisUse(const string& mesh_name);
    void DisUseImmediately(const string& mesh_name);
    void Make(const string& mesh_name, const D3DPRIMITIVETYPE type, const vector<VERTEX_3D>& vertexes, const vector<int>& index, const int polygon_number);
    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    Mesh* GetMesh(const string& mesh_name);
    //--------------------------------------------------------------------------------
    //  �Z�[�u
    //--------------------------------------------------------------------------------
    void SaveMeshToFile(const string& mesh_name, const string& file_name);
    //--------------------------------------------------------------------------------
    //  ����
    //--------------------------------------------------------------------------------
    static MeshManager* Create();
private:
    //--------------------------------------------------------------------------------
    //  �\����
    //--------------------------------------------------------------------------------
    struct MeshInfo// smart point
    {
        MeshInfo():user_number(1), pointer(nullptr){}
        int user_number;
        Mesh* pointer;
    };
    //--------------------------------------------------------------------------------
    //  �����o�[�ϐ�
    //--------------------------------------------------------------------------------
    unordered_map<size_t, MeshInfo> m_meshs;

    //--------------------------------------------------------------------------------
    //  ���[�h
    //--------------------------------------------------------------------------------
    MeshInfo LoadFromMeshFile(const string& mesh_name);
    MeshInfo LoadXFile(const string& mesh_name);
    //--------------------------------------------------------------------------------
    //  ���b�V�������֐�
    //--------------------------------------------------------------------------------
    MeshInfo CreateCube(void);
    MeshInfo CreateSphere(void);
    MeshInfo CreateSkyBox(void);
    MeshInfo CreatePolygon2D(void);
    MeshInfo CreatePolygon3D(void);
    MeshInfo CreateMeshField(void);
    //--------------------------------------------------------------------------------
    //  �o�b�t�@�����֐�
    //--------------------------------------------------------------------------------
    bool CreateBuffer2D(Mesh* mesh)const;
    bool CreateBuffer3D(Mesh* mesh)const;
   
};
