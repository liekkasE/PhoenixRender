#include "GlobalContext.hpp"
#include "Shader.h"

namespace FX
{





    HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
    {
        HRESULT hr = S_OK;

        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
        //#if defined( DEBUG ) || defined( _DEBUG )
                // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
                // Setting this flag improves the shader debugging experience, but still allows 
                // the shaders to be optimized and to run exactly the way they will run in 
                // the release configuration of this program.
        dwShaderFlags |= D3DCOMPILE_DEBUG;
        //#endif

        ID3DBlob* pErrorBlob;
        hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
            dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
        if (FAILED(hr))
        {
            if (pErrorBlob != NULL)
                OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
            if (pErrorBlob) pErrorBlob->Release();
            return hr;
        }
        if (pErrorBlob) pErrorBlob->Release();

        return S_OK;
    }

    Shader* createShader(const ShaderDesc& sd)
    {
        ID3DBlob* vs_buffer;
        ID3DBlob* ps_buffer;
        ID3D11VertexShader* out_vs;
        ID3D11PixelShader* out_ps;
        ID3D11InputLayout* out_ia;

        HRESULT res = CompileShaderFromFile(sd.m_HlslFile.c_str(), "VS_Main", "vs_5_0", &vs_buffer);
        GetD3DDevice()->CreateVertexShader(vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize(), 0, &out_vs);

        res = CompileShaderFromFile(sd.m_HlslFile.c_str(), "PS_Main", "ps_5_0", &ps_buffer);
        GetD3DDevice()->CreatePixelShader(ps_buffer->GetBufferPointer(), ps_buffer->GetBufferSize(), 0, &out_ps);


        GetD3DDevice()->CreateInputLayout(sd.m_InputDesc, sd.m_InputElementNums, vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize(), &out_ia);

        return new Shader(out_ia, out_vs, out_ps);

    }

}