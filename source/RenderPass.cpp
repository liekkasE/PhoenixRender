#include "GlobalContext.hpp"
#include "RenderPass.h"
#include "SwapChain.h"
#include "Shader.h"

namespace FX
{


    void DefaultRenderPass::Init()
    {
        m_width = 1280;
        m_height = 720;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = m_width;
        desc.Height = m_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        HRESULT hr =  GetD3DDevice()->CreateTexture2D(&desc, nullptr, &m_rt);

        if (hr < 0)
        {
            const WCHAR* err_msg = DXGetErrorStringW(hr);
        }

        D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
        rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtv_desc.Format = desc.Format;
        rtv_desc.Texture2D.MipSlice = 0;

        ID3D11Texture2D* backbuffer;
        auto wp = GetD3DSwapChain();
        if (wp.expired() == false)
        {
            wp.lock()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
        }
        hr = GetD3DDevice()->CreateRenderTargetView(backbuffer, &rtv_desc, &m_rtv);
        if (hr < 0)
        {
            const WCHAR* err_msg = DXGetErrorStringW(hr);
        }


        D3D11_SAMPLER_DESC SamDesc;
        SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        SamDesc.MipLODBias = 0.0f;
        SamDesc.MaxAnisotropy = 1;
        SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
        SamDesc.MinLOD = 0;
        SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = GetD3DDevice()->CreateSamplerState(&SamDesc, &m_sample_state);

        if (hr < 0)
        {
            const WCHAR* err_msg = DXGetErrorStringW(hr);
        }
    }

    void DefaultRenderPass::Execute()
    {
        
        std::weak_ptr<View> mainView = GlobalContext::Get()->m_RenderCore->GetMainPipeline()->GetMainView();
        std::shared_ptr<Scene> mainScene = GlobalContext::Get()->m_RenderCore->GetMainScene();


        std::vector<MeshInstance>& allMeshInstance = mainScene->GetAllMeshInstance();

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = m_width;
        vp.Height = m_height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        GetD3DContext()->RSSetViewports(1, &vp);

        std::unordered_map<MeshName, std::vector<MeshInstance>> DrawMeshMapToMeshInstance;
        for (const auto& ins : allMeshInstance)
        {
            DrawMeshMapToMeshInstance[ins.m_MeshName].push_back(ins);
        }

        // Clear the back buffer 
        float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
        GetD3DContext()->OMSetRenderTargets(1, &m_rtv, nullptr);
        GetD3DContext()->ClearRenderTargetView(m_rtv, ClearColor);

        //for each mesh, draw instances call
        for (const auto& it : DrawMeshMapToMeshInstance)
        {
            Mesh* mesh = GlobalContext::Get()->m_MeshManager->GetMeshByName(it.first);
            size_t ins_cnt = it.second.size();

            GetD3DContext()->IASetInputLayout(mesh->m_Shader->m_Layout);

            // Set vertex buffer
            UINT pos_vb_stride = sizeof(glm::vec3) + sizeof(glm::vec4);// vertex position and color
            UINT pos_vb_offset = 0;
            ID3D11Buffer* pVB = mesh->m_GpuVb->GetBuffer().m_gpu_addr;
            GetD3DContext()->IASetVertexBuffers(0, 1, &pVB, &pos_vb_stride, &pos_vb_offset);

            UINT ins_vb_stride = sizeof(glm::vec4);// vertex position and color
            UINT ins_vb_offset = 0;
            ID3D11Buffer* pInsVB = mainScene->m_GpuInstanceBuffer->GetBuffer().m_gpu_addr;
            GetD3DContext()->IASetVertexBuffers(1, 1, &pInsVB, &ins_vb_stride, &ins_vb_offset);

            GetD3DContext()->IASetIndexBuffer(mesh->m_GpuIb->GetBuffer().m_gpu_addr, DXGI_FORMAT_R32_UINT,0);

            GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            GetD3DContext()->VSSetShader(mesh->m_Shader->m_Vs, nullptr, 0);
            GetD3DContext()->PSSetShader(mesh->m_Shader->m_Ps, nullptr, 0);
            GetD3DContext()->PSSetSamplers(0, 1, &m_sample_state);

            std::shared_ptr<View> spView;
            if (!mainView.expired())
            {
                spView = mainView.lock();
            }

            ID3D11Buffer* pconstant_buffer = spView->GetPerViewConstantBuffer();
            GetD3DContext()->VSSetConstantBuffers(0, 1, &pconstant_buffer);

            GetD3DContext()->DrawIndexedInstanced((UINT)(mesh->m_GpuIb->GetNoofElements()), (UINT)ins_cnt, 0, 0, 0);
        }

        auto wp = GetD3DSwapChain();
        if (wp.expired() == false)
        {
            wp.lock()->Present(0, 0);
        }

    }


}