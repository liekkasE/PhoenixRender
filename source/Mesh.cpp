#include "Mesh.h"
#include "Shader.h"
#include "GpuBuffer.h"

namespace FX
{





    Mesh::Mesh(const std::vector<Vertex>& raw_vb, const std::vector<Index>& raw_ib, const MeshType& mesh_type)
    {
        m_GpuVb = new Buffer(raw_vb.size(), sizeof(Vertex),
            BufferCreateFlags::Enum::VertexBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN,
            const_cast<Vertex*>(&raw_vb[0]));

        m_GpuIb = new Buffer(raw_ib.size(), sizeof(Index),
            BufferCreateFlags::Enum::IndexBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN,
            const_cast<Index*>(&raw_ib[0]));


        //build shader and layout

        ShaderDesc sd;
        if (mesh_type == MeshType::StaticMesh)
        {
            D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3];
            
            inputLayoutDesc[0].SemanticName = "POSITION";
            inputLayoutDesc[0].SemanticIndex = 0;
            inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
            inputLayoutDesc[0].InputSlot = 0;
            inputLayoutDesc[0].AlignedByteOffset = 0;
            inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            inputLayoutDesc[0].InstanceDataStepRate = 0;

            inputLayoutDesc[1].SemanticName = "COLOR";
            inputLayoutDesc[1].SemanticIndex = 0;
            inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            inputLayoutDesc[1].InputSlot = 0;
            inputLayoutDesc[1].AlignedByteOffset = sizeof(glm::vec3);
            inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            inputLayoutDesc[1].InstanceDataStepRate = 0;

            inputLayoutDesc[2].SemanticName = "WORLD";
            inputLayoutDesc[2].SemanticIndex = 0;
            inputLayoutDesc[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            inputLayoutDesc[2].InputSlot = 1;
            inputLayoutDesc[2].AlignedByteOffset = 0;
            inputLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            inputLayoutDesc[2].InstanceDataStepRate = 0;
            
            sd.m_InputDesc = &inputLayoutDesc[0];
            sd.m_InputElementNums = 3;
        }
        else if (mesh_type == MeshType::DynamicMesh)
        {
        }

        sd.m_HlslFile = L"C:\\workspace\\Phoenix\\source\\shaders\\NaiveBox.hlsl";
        Shader* naiveBoxShader = createShader(sd);
        m_Shader = naiveBoxShader;
    }

    Mesh::~Mesh()
    {
        delete(m_GpuVb);
        //delete(m_GpuColorsVb);
        delete(m_GpuIb);
        delete(m_Shader);
    }

    void MeshManager::addMesh(MeshName name, Mesh* mesh)
    {
        if (m_Meshes.find(name) != m_Meshes.end())
        {
            printf("[error]|mesh is existed\n");
            return;
        }
        m_Meshes[name] = mesh;
    }

}