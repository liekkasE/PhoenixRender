#include "Pipeline.h"
#include "RenderPass.h"

namespace FX {






    void Pipeline::addPass()
    {
        m_DefaultRenderPass = new DefaultRenderPass();
        m_DefaultRenderPass->Init();

    }

    void Pipeline::Execute()
    {
        m_DefaultRenderPass->Execute();
    }

    void Pipeline::addDefaultView()
    {
        std::shared_ptr<View> defaultView = std::make_shared<View>(ViewType::EEye);
        m_Views[m_mainViewName] = defaultView;

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