#pragma once

#include "base/FXCommon.h"
#include "Pass.h"
#include "View.h"

namespace FX {

    class Pass;
    class Pipeline
    {
    public:

        void addPass();

        void Execute();

        void addDefaultView();

        void update();

        std::weak_ptr<View> GetMainView();


    private:

        //will be the pass graph
        Pass* m_DefaultRenderPass;

        ViewTable m_Views;
        ViewName m_mainViewName = "MainView";
        



    };


}