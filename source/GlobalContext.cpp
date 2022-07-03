#include "base/FXCommon.h"
#include "GlobalContext.hpp"
#include "RenderCore.hpp"
#include "SwapChain.h"



namespace FX
{

    GlobalContext::~GlobalContext()
    {
    }

    std::shared_ptr<GlobalContext> GlobalContext::s_context = nullptr;
    FX::AppCore* gAppCore = nullptr;

    GlobalContext::GlobalContext()
    {
        m_RenderCore = std::make_unique<RenderCore>();
    }

    void GlobalContext::Init()
    {
        if (s_context)
        {
            assert(false);
            return;
        }

        s_context = std::make_unique<GlobalContext>();
    }

    void GlobalContext::InitSubSystems()
    {
        m_RHI = std::make_shared<HWD3D11Device>();
        m_RHI->Init();

        m_MeshManager = std::make_shared<MeshManager>();
        m_RenderCore = std::make_shared<RenderCore>();
        m_RenderCore->Init();


    }

    std::shared_ptr<GlobalContext> GlobalContext::Get()
    {
        return s_context;
    }

    void GlobalContext::Tick()
    {
        m_RenderCore->tick();
    }

    void AppCore::Init()
    {
        GlobalContext::Init();
        GlobalContext::Get()->InitSubSystems();
    }

}



