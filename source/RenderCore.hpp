#pragma once
#include "base/FXCommon.h"
#include "Scene.h"
#include "Pipeline.h"

namespace FX
{	
	//what is rendercore? the definition is ?
	//TODO
	class RenderCore
	{

	public:
		RenderCore();
		~RenderCore()
		{

		}

		void Init();
		void tick();

		static void Render(std::shared_ptr<Scene> scene);

		std::shared_ptr<Pipeline> GetMainPipeline() { return m_MainPipeline; }
		std::shared_ptr<Scene> GetMainScene() { return m_scenes[0]; }
	private:

		std::shared_ptr<Pipeline> m_MainPipeline;
		
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};






}