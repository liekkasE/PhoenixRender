#include "base/FXCommon.h"
#include "RenderPass.h"
#include "Scene.h"
#include "math.h"

#include "NaiveBox.h"
#include "GpuBuffer.h"
#include "View.h"


namespace FX
{

	void Scene::Init()
	{

		//add boxes mesh
		Mesh* box = new Mesh(naive_box_vertices, naive_box_indices, MeshType::StaticMesh);
		GlobalContext::Get()->m_MeshManager->addMesh("naive_box", box);

		//generate box
		for (int i = 0; i < 5; i++)
		{
			float r1 = FX::Math::RandF()*2.0f;
			float r2 = FX::Math::RandF()*2.0f;
			float r3 = FX::Math::RandF()*2.0f;

			MeshInstance oneInstance;
			oneInstance.m_MeshName = "naive_box";
			oneInstance.m_WorldPos = glm::vec3(r1, r2, r3);

			m_Instances.emplace_back(oneInstance);

		}		
	}

	





	void Scene::update()
	{

	}

}