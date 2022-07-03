#include "GlobalContext.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "GpuBuffer.h"
#include "SwapChain.h"

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

            sd.m_InputDesc = &inputLayoutDesc[0];
            sd.m_InputElementNums = 2;
        }
        else if (mesh_type == MeshType::DynamicMesh)
        {
        }

        sd.m_HlslFile = L"C:\\workspace\\Phoenix\\source\\shaders\\NaiveBox.hlsl";
        Shader* naiveBoxShader = createShader(sd);
        m_Shader = naiveBoxShader;



    }

    void Mesh::init(const MeshCreateDesc& desc)
    {
        m_CreateDesc = desc;

        m_CpuInstances.resize(m_CreateDesc.m_MaxInsCnt);
        m_GpuInstances = new Buffer(m_CreateDesc.m_MaxInsCnt, sizeof(Mesh::VSInstance),
            BufferCreateFlags::Enum::StructuredBuffer, BufferBindFlags::Enum::SRV, DXGI_FORMAT_UNKNOWN,
            &(m_CpuInstances[0]));

    }

    Mesh::~Mesh()
    {
        delete(m_GpuVb);
        delete(m_GpuIb);
        delete(m_Shader);
    }

    void Mesh::flushInstances()
    {
        m_GpuInstances->updateBuffer();
    }

    void Mesh::addRenderInstance(const MeshInstance& Ins)
    {
        if (m_AllocatedCnt <= m_CpuInstances.size())
        {
            glm::mat4x4 trans = glm::translate(glm::mat4(1.0f), Ins.m_WorldPos);
            glm::mat4x4 rotation = glm::mat4_cast(Ins.m_Rot);
            glm::mat4x4 scale = glm::scale(glm::mat4(1.0f),Ins.m_Scale);

            m_CpuInstances[m_AllocatedCnt].m_Trans = trans * scale * rotation;
            m_CpuInstances[m_AllocatedCnt].m_Trans = glm::transpose(m_CpuInstances[m_AllocatedCnt].m_Trans);
            m_AllocatedCnt++;
        }
        else
        {
            printf("not enough space\n");
        }
    }

    void Mesh::clearRenderInstance()
    {
        m_AllocatedCnt = 0;
    }

    void Mesh::draw() const
    {

        GetD3DContext()->IASetInputLayout(m_Shader->m_Layout);

        // Set vertex buffer
        UINT pos_vb_stride = sizeof(glm::vec3) + sizeof(glm::vec4);// vertex position and color
        UINT pos_vb_offset = 0;
        ID3D11Buffer* pVB = m_GpuVb->GetBuffer().m_gpu_addr;
        GetD3DContext()->IASetVertexBuffers(0, 1, &pVB, &pos_vb_stride, &pos_vb_offset);


        GetD3DContext()->IASetIndexBuffer(m_GpuIb->GetBuffer().m_gpu_addr, DXGI_FORMAT_R32_UINT, 0);

        GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        GetD3DContext()->VSSetShader(m_Shader->m_Vs, nullptr, 0);

        GetD3DContext()->VSSetShaderResources(0, 1, &(m_GpuInstances->GetBuffer().m_bufferSRV));

        GetD3DContext()->PSSetShader(m_Shader->m_Ps, nullptr, 0);
        //GetD3DContext()->PSSetSamplers(0, 1, &m_sample_state);

        GetD3DContext()->DrawIndexedInstanced((UINT)(m_GpuIb->GetNoofElements()), (UINT)m_AllocatedCnt, 0, 0, 0);
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

    void MeshManager::submit()
    {
        for (auto iter : m_Meshes)
        {
            iter.second->flushInstances();
        }
    }

}