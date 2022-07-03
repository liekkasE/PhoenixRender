#pragma once
#include "base/FXCommon.h"
#include "View.h"
#include "Mesh.h"
#include "GpuBuffer.h"
#include "Pipeline.h"
#include "Pass.h"

namespace FX 
{

    struct PerObjectTransform
    {
        glm::vec3 m_Pos;
        glm::quat m_Ori;
        glm::mat4x4 m_Trans;
    };

    class BaseInstance
    {
    protected:
        PerObjectTransform m_Pose;
        uint32_t m_MeshType;
    };

    class StaticMeshInstance : public BaseInstance
    {
    public:

    private:
    };


    class Scene
    {
        friend class RenderCore;
    public:
        void Init();
        void update();

        std::vector<MeshInstance>& GetAllMeshInstance() { return m_Instances; }
        
    private:
        std::vector<MeshInstance> m_Instances;

            
    };





}