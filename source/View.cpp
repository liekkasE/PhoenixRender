#include "RenderPass.h"
#include "View.h"
namespace FX
{






    View::View(const ViewType& viewType)
        :m_ViewType(viewType)
    {
        m_view_buffer = new Buffer(1, sizeof(ViewCamShaderData),
            BufferCreateFlags::Enum::ConstantBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN, static_cast<void*>(&m_cam_vp));
    }

    View::View()
        :m_ViewType(ViewType::EEye)
    {
        m_view_buffer = new Buffer(1, sizeof(ViewCamShaderData),
            BufferCreateFlags::Enum::ConstantBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN, static_cast<void*>(&m_cam_vp));

        m_cam_vp.position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_cam_vp.ori = glm::quatLookAtRH(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void View::updateCamera()
    {
        glm::mat4 mat(1.0f);
        glm::mat4 rot_matrix = glm::toMat4(m_cam_vp.ori);
        glm::mat4 move_matrix = glm::translate(mat, m_cam_vp.position);


        m_cam_vp.viewMat = glm::lookAtRH(m_cam_vp.position, glm::vec3(0.0f, 0.0f, -2.0f) + m_cam_vp.position, glm::vec3(0.0f, 1.0f, 0.0f));
        m_cam_vp.viewMat = glm::transpose(m_cam_vp.viewMat);

        glm::mat4 proj = glm::transpose(glm::perspectiveFovRH_ZO(glm::radians(80.0f), 1280.0f, 720.0f, 0.01f, 100.0f));
            
        m_cam_vp.viewproj = proj* m_cam_vp.viewMat;

        m_view_buffer->updateBuffer();
    }

}