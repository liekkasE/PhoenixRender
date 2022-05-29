#pragma once

#include "base/FXCommon.h"
#include "Pass.h"
#include "View.h"

namespace FX {

	class Pass;
	class Pipeline
	{
	public:


		void Execute();


		std::shared_ptr<View> GetMainView();


	private:

		//will be the pass graph
		std::shared_ptr<Pass> m_DefaultRenderPass;

		ViewTable m_Views;
		ViewName m_mainViewName = "MainView";
		



	};


}