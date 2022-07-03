#pragma once
#include "base/FXCommon.h"
#include "SwapChain.h"

namespace FX 
{
    typedef std::string MeshName;
    typedef uint32_t Index;

    //general definition of a instance
    //managed by scene
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

        struct MeshCreateDesc
        {
            uint32_t m_MaxInsCnt = 0;
        };

        //used in vertex shader stage
        struct VSInstance
        {
            glm::mat4x4 m_Trans;
        };

        Mesh(const std::vector<Vertex>& raw_vb, const std::vector<Index>& raw_ib, const MeshType& mesh_type);
        void init(const MeshCreateDesc& desc);

        Mesh() {}
        Mesh(const Mesh& buf) = delete;
        Mesh(Mesh& buf) = delete;
        Mesh(Mesh&& buf) = delete;
        Mesh& operator=(Mesh&& buf) = delete;
        Mesh& operator=(const Mesh& buf) = delete;
        ~Mesh();

        void flushInstances();

        void addRenderInstance(const MeshInstance& Ins);
        void clearRenderInstance();

        void draw() const;

        //PER_VERTEX ATTRIBUTE
        Buffer* m_GpuVb = nullptr;
        Buffer* m_GpuIb = nullptr;

        //PRE_INSTANCE ATTRIBUTE
        std::vector<VSInstance> m_CpuInstances;
        Buffer* m_GpuInstances = nullptr;
        uint32_t m_AllocatedCnt = 0; // cnt in cpu_instances vector

        MeshCreateDesc m_CreateDesc;

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

        void submit();

        const std::unordered_map<MeshName, Mesh*> GetAllMesh() const { return m_Meshes; }

    private:

        std::unordered_map<MeshName, Mesh*> m_Meshes;
    };


}