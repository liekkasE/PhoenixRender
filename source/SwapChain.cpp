#include "SwapChain.h"

#include <d3d11.h>

#include "GlobalContext.hpp"



namespace FX
{
    std::shared_ptr<RHI> g_RHI = nullptr;

    void RHI::createSelf()
    {
        if (g_RHI) return;

        g_RHI = std::make_shared<HWD3D11Device>();
    }


    std::shared_ptr<RHI> RHI::Get()
    {
        return g_RHI;
    }

    int HWD3D11Device::Init()
    {
        HRESULT hr = S_OK;

        RECT rc;
        GetClientRect(gAppCore->GetHWND(), &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        UINT createDeviceFlags = 0;

#ifdef FX_GFX_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = gAppCore->GetHWND();
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        IDXGISwapChain* pTempSwapChain = nullptr;
        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            m_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                D3D11_SDK_VERSION, &sd, &pTempSwapChain, &m_device, &m_featureLevel, &m_context);
            if (SUCCEEDED(hr))
            {
                m_SwapChain.InitSwapChain(pTempSwapChain);
                break;
            }
        }

        if (FAILED(hr))
            return hr;

        return hr;
    }


    void HWD3D11Device::Cleanup()
    {
        //TODO
    }




    void SwapChain::InitSwapChain(IDXGISwapChain* in_pSwapChain)
    {
        if (m_pSwapChain) return;
        m_pSwapChain = std::shared_ptr<IDXGISwapChain>(in_pSwapChain);
    }

    ID3D11Texture2D* SwapChain::GetBackBuffer()
    {
        ID3D11Texture2D* out_back_buffer = nullptr;

        HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&out_back_buffer);

        return out_back_buffer;
    }

}