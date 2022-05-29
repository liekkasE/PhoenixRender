#include "base/FXCommon.h"
#include "RenderCore.hpp"
#include "SwapChain.h"

#include "GlobalContext.hpp"


namespace FX
{

	std::shared_ptr<GlobalContext> GlobalContext::s_context = nullptr;
	FX::AppCore* gAppCore = nullptr;

	GlobalContext::GlobalContext()
	{
		m_RenderCore = std::make_unique<RenderCore>();
	}

	void GlobalContext::Init()
	{
		if (!s_context)
		{
			assert(false);
			return;
		}

		s_context = std::make_unique<GlobalContext>();
	}

	void GlobalContext::InitSubSystems()
	{
		m_RHI = std::make_shared<RHI>();
		m_RHI->Init();

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

}



