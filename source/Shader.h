#pragma once
#include "base/FXCommon.h"
#include "SwapChain.h"

namespace FX 
{
	struct ShaderDesc
	{
		std::wstring m_HlslFile;
		D3D11_INPUT_ELEMENT_DESC* m_InputDesc;
		uint32_t m_InputElementNums;
	};

	class Shader
	{
	public:
		Shader(ID3D11InputLayout* Layout, ID3D11VertexShader* Vs, ID3D11PixelShader* Ps)
			:m_Layout(Layout),
			m_Vs(Vs),
			m_Ps(Ps) 
		{}


		ID3D11InputLayout* m_Layout;
		ID3D11VertexShader* m_Vs;
		ID3D11PixelShader* m_Ps;

	};


	HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	Shader* createShader(const ShaderDesc& sd);


}