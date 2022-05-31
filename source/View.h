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


        View();

        View(const ViewType& viewType);

        void updateCamera();


        ID3D11Buffer* GetPerViewConstantBuffer() const { return m_view_buffer->GetBuffer().m_gpu_addr; }

    private:

        ViewType m_ViewType;
        ViewCamShaderData m_cam_vp;

        Buffer* m_view_buffer;

        

    };






}