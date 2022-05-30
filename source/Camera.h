#pragma once
#include "base/FXCommon.h"
#include "View.h"
#include "Mesh.h"

namespace FX 
{

    struct Frustum
    {
        float m_NearPlane;
        float m_FarPlane;
        float m_Aspect;
        float m_HFov;
        float m_VFov;
    };



    class Camera
    {
    public:

        const glm::mat4x4 getNewestViewProj()
        {
            if ()
            {

            }

            return m_ViewProj;
        }

        bool checkHadUpdate()
        {
            return m_IsUpdated;
        }
        

    private:
        glm::vec3 m_Pos;
        glm::quat m_Ori;
        Frustum m_Frustum;
        glm::mat4x4 m_ViewProj;

        bool m_IsUpdated;
    };


    class CameraSystem
    {
    public:
        const Camera&

    private:
        std::unordered_map<std::string, Camera> m_CameraMap;
    };


}