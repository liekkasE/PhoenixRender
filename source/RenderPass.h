#pragma once
#include "base/FXCommon.h"
#include "Pass.h"


namespace FX 
{
	class DefaultRenderPass : public Pass
	{
	public:
		void Init();
		void Render();
		virtual void Execute() final override;

	private:

		ID3D11RenderTargetView* m_rtv = nullptr;
		ID3D11SamplerState* m_sample_state = nullptr;
		ID3D11Texture2D* m_rt;


		float m_width;
		float m_height;

	};


	class ShaderUtility
	{
		static HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);



	};


}