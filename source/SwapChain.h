#pragma once
#include "base/FXCommon.h"



namespace FX
{
	#define GetD3DDevice() std::dynamic_pointer_cast<HWD3D11Device>(GlobalContext::Get()->m_RHI)->GetDXDevice()
	#define GetD3DContext()  std::dynamic_pointer_cast<HWD3D11Device>(GlobalContext::Get()->m_RHI)->GetDXContext()
	#define GetD3DSwapChain() std::dynamic_pointer_cast<HWD3D11Device>(GlobalContext::Get()->m_RHI)->GetD3D11SwapChain()
		

	class SwapChain
	{


	public:
		void InitSwapChain(IDXGISwapChain* in_pSwapChain);


		ID3D11Texture2D* GetBackBuffer();


		std::weak_ptr<IDXGISwapChain> GetIDXGISwapChain()
		{
			return m_pSwapChain;
		}

	private:
		std::shared_ptr<IDXGISwapChain> m_pSwapChain = nullptr;
	};



}

namespace FX
{

	class RHI
	{
	public:
		static void createSelf();
		static std::shared_ptr<RHI> Get();

		virtual int Init() = 0;
		virtual void Cleanup() = 0;
		
	};

	

	class HWD3D11Device : public RHI
	{
	public:
		int Init() override;
		void Cleanup() override;

		ID3D11Texture2D* GetBackBuffer()
		{
			return m_SwapChain.GetBackBuffer();
		}

		ID3D11Device* GetDXDevice()
		{
			return m_device;
		}

		ID3D11DeviceContext* GetDXContext()
		{
			return m_context;
		}

		std::weak_ptr<IDXGISwapChain> GetD3D11SwapChain() {
			return m_SwapChain.GetIDXGISwapChain();
		}

	private:
		ID3D11Device* m_device = NULL;
		ID3D11DeviceContext* m_context = NULL;
		D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;

		SwapChain m_SwapChain;
	};


}


