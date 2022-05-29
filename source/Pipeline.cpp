#include "Pipeline.h"


namespace FX {






	void Pipeline::Execute()
	{
		m_DefaultRenderPass->Execute();
	}

	std::shared_ptr<FX::View> Pipeline::GetMainView()
	{
		auto iter = m_Views.find(m_mainViewName);
		if (iter != m_Views.end())
		{
			return iter->second;
		}

		return std::shared_ptr<View>();
	}

}