#include "base/FXCommon.h"
#include "RenderPass.h"
#include "Scene.h"
#include "math.h"

#include "NaiveBox.h"
#include "GpuBuffer.h"
#include "View.h"

#include "Mesh.h"


namespace FX
{

    void Scene::Init()
    {

        //add boxes mesh
        Mesh* box = new Mesh(naive_box_vertices, naive_box_indices, MeshType::StaticMesh);
        GlobalContext::Get()->m_MeshManager->addMesh("naive_box", box);
        box->init(Mesh::MeshCreateDesc{ 1 });

        //generate box
        

        MeshInstance oneInstance;
        oneInstance.m_MeshName = "naive_box";
        oneInstance.m_WorldPos = glm::vec3(0, 0, -10);

        m_Instances.emplace_back(oneInstance);
  
    }

    





    void Scene::update()
    {
        //logic
        static uint32_t update_cnt = 0;
        update_cnt = (update_cnt + 1) % 360;

        for (MeshInstance& ins : m_Instances)
        {
            glm::mat4 mat(1.0f);
            glm::mat4 rot_matrix = glm::rotate(mat, glm::radians(1.0f* update_cnt), glm::vec3(0,0,1.0f));
            ins.m_Rot = glm::quat_cast(rot_matrix);
        }




        //perpare instances which are really needed to be rendered
        std::shared_ptr<MeshManager> mesh_manager = GlobalContext::Get()->m_MeshManager;

        for (auto& iter : mesh_manager->GetAllMesh())
        {
            iter.second->clearRenderInstance();
        }

        for (int i = 0;i<m_Instances.size();i++)
        {
            Mesh* mesh = mesh_manager->GetMeshByName(m_Instances[i].m_MeshName);
            mesh->addRenderInstance(m_Instances[i]);
        }

        mesh_manager->submit();

    }

}