#pragma once
#include "base/FXCommon.h"
#include "SwapChain.h"

namespace FX 
{
    typedef std::string MeshName;
    typedef uint32_t Index;

    class MeshInstance
    {
    public:
        MeshName m_MeshName;
        glm::vec3 m_WorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat m_Rot = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    struct Vertex
    {
        glm::vec3 m_Pos;
        glm::vec4 m_Color;
        //glm::vec3 m_normal;
        //glm::vec2 m_uv;
    };

    enum struct MeshType : unsigned int
    {
        StaticMesh = 0,
        DynamicMesh = 1
    };

    class Buffer;
    class Shader;
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& raw_vb, const std::vector<Index>& raw_ib, const MeshType& mesh_type);


        Mesh() {}
        Mesh(const Mesh& buf) = delete;
        Mesh(Mesh& buf) = delete;
        Mesh(Mesh&& buf) = delete;
        Mesh& operator=(Mesh&& buf) = delete;
        Mesh& operator=(const Mesh& buf) = delete;
        ~Mesh();


        Buffer* m_GpuVb = nullptr;
        Buffer* m_GpuIb = nullptr;


        Shader* m_Shader;
    };




    class MeshManager
    {
    public:
        void addMesh(MeshName name, Mesh* mesh);
        

        Mesh* GetMeshByName(const MeshName& name)
        {
            return m_Meshes.find(name) != m_Meshes.end() ? m_Meshes[name] : nullptr;
        }

    private:

        std::unordered_map<MeshName, Mesh*> m_Meshes;
    };


}