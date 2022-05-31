#include "base/FXCommon.h"
#include "RenderCore.hpp"
#include "SwapChain.h"
#include "GlobalContext.hpp"
#include "RenderPass.h"



namespace FX
{
    RenderCore::RenderCore()
    {
    }


    void RenderCore::Render(std::shared_ptr<Scene> scene)
    {
        //prepare

        m_MainPipeline->update();
        //render
        m_MainPipeline->Execute();
        

        //present
    }

    void RenderCore::tick()
    {
        for (auto scene : m_scenes)
        {
            scene->update();
        }

        for (auto scene : m_scenes)
        {
            Render(scene);
        }

    }


    void RenderCore::Init()
    {
        printf("RenderCore::Init\n");

        m_scenes.emplace_back(std::make_shared<Scene>());
        std::shared_ptr<Scene> new_scene = m_scenes.back();
        new_scene->Init();

        m_MainPipeline = std::make_shared<Pipeline>();
        m_MainPipeline->addPass();
        m_MainPipeline->addDefaultView();

    }





}