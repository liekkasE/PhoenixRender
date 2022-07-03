#pragma once

#include "base/FXCommon.h"

namespace FX
{

#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }



    class GpuBufferManager;
    class RHI;
    class RenderCore;
    class MeshManager;

    //wanna to create a global manager system, so that we can use these system everywhere
    class GlobalContext
    {
    public:
        static void Init();
        static std::shared_ptr<GlobalContext> Get();

        void InitSubSystems();
        void Tick();
        GlobalContext();
        ~GlobalContext();
    public:
        std::shared_ptr<RenderCore> m_RenderCore;
        std::shared_ptr<RHI> m_RHI;
        std::shared_ptr<MeshManager> m_MeshManager;

    private:
       
        static std::shared_ptr<GlobalContext> s_context;
    };

    

    class AppCore
    {
    public:
        AppCore(HWND hWnd)
            :m_hWnd(hWnd)
        {
        }
        ~AppCore() {}

        void Init();

        HWND GetHWND() const { return m_hWnd; }

        void Tick()
        {
            GlobalContext::Get()->Tick();
        }

        void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {}

    private:

        HWND m_hWnd;

    };

    extern AppCore* gAppCore;




    class ServerSystem
    {
    public:
        static void start()
        {
            m_server = std::make_shared<ServerSystem>();

            std::thread serverMainThread(&ServerSystem::loop, *m_server);
            serverMainThread.detach();
        }
    private:
        void loop()
        {
            while (1)
            {
                
            }
        }

    private:
        static std::shared_ptr<ServerSystem> m_server;

    };
    

}