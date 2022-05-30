#pragma once
#include "base/FXCommon.h"
#include "GpuBuffer.h"

namespace FX 
{
    class View;
    using ViewName = std::string;
    using ViewTable = std::unordered_map<ViewName, std::shared_ptr<View>>;

    enum ViewType
    {
        EEye = 0,
        EShadow = 1
    };

    struct ViewCamShaderData
    {
        glm::vec3 position { 0.0f, 0.0f, 0.0f};
        float pad = 0.0f;
        glm::quat ori {0.0f, 0.0f, 0.0f, 1.0f};
        glm::mat4 viewMat { glm::mat4(1.0f) };
        glm::mat4 viewproj{ glm::mat4(1.0f) };
    };


    /// <summary>
    /// view用于culling
    /// view持有以pass种类进行分类的需要绘制的网格种类
    /// </summary>
    class View
    {
    public:




        View()
        :m_ViewType(ViewType::EEye)
        {
            m_view_buffer = new Buffer(1, sizeof(ViewCamShaderData),
                BufferCreateFlags::Enum::ConstantBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN, static_cast<void*>(&m_cam_vp));

            m_cam_vp.position = glm::vec3(0.0f, 0.0f, 0.0f);
            m_cam_vp.ori = glm::quatLookAtRH(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        View(const ViewType& viewType)
            :m_ViewType(viewType)
        {
            m_view_buffer = new Buffer(1, sizeof(ViewCamShaderData),
                BufferCreateFlags::Enum::ConstantBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN, static_cast<void*>(&m_cam_vp));
        }

        void updateCamera()
        {
            glm::mat4 mat(1.0f);
            glm::mat4 rot_matrix = glm::toMat4(m_cam_vp.ori);
            glm::mat4 move_matrix = glm::translate(mat, m_cam_vp.position);

            
            m_cam_vp.viewMat = glm::lookAtRH(m_cam_vp.position, glm::vec3(0.0f, 0.0f, -1.0f) + m_cam_vp.position, glm::vec3(0.0f, 1.0f, 0.0f));
            m_cam_vp.viewproj = glm::perspectiveRH_ZO(80.0f, 16.0f / 9.0f, 0.01f, 100.0f);

            m_view_buffer->updateBuffer();
        }

        ID3D11Buffer* GetPerViewConstantBuffer() const { return m_view_buffer->GetBuffer().m_gpu_addr; }

    private:

        ViewType m_ViewType;
        ViewCamShaderData m_cam_vp;

        Buffer* m_view_buffer;

        

    };






}