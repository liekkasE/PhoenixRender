#include "Pipeline.h"
#include "RenderPass.h"
#include "GlobalContext.hpp"
#include "SwapChain.h"

namespace FX {






    void Pipeline::addPass()
    {
        m_DefaultRenderPass = new DefaultRenderPass();
        m_DefaultRenderPass->Init();

    }

    void Pipeline::Execute()
    {

        std::shared_ptr<View> spView;
        if (!GetMainView().expired())
        {
            spView = GetMainView().lock();
        }

        ID3D11Buffer* pconstant_buffer = spView->GetPerViewConstantBuffer();
        GetD3DContext()->VSSetConstantBuffers(0, 1, &pconstant_buffer);


        m_DefaultRenderPass->Execute();
    }

    void Pipeline::addDefaultView()
    {
        std::shared_ptr<View> defaultView = std::make_shared<View>(ViewType::EEye);
        m_Views[m_mainViewName] = defaultView;

    }

    void Pipeline::update()
    {
        //update view
        for (auto& it : m_Views)
        {
            it.second->updateCamera();
        }
    }

    std::weak_ptr<FX::View> Pipeline::GetMainView()
    {
        auto iter = m_Views.find(m_mainViewName);
        if (iter != m_Views.end())
        {
            return iter->second;
        }

        return std::shared_ptr<View>();
    }

}