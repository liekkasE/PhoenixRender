#pragma once
#include "base/FXCommon.h"
#include "SwapChain.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

namespace FX {



	struct GpuBuffer
	{
		D3D11_BUFFER_DESC m_desc;
		uint64_t m_allocated_index; 
		ID3D11Buffer* m_gpu_addr;
		ID3D11ShaderResourceView* m_bufferSRV;
		ID3D11UnorderedAccessView* m_bufferUAV;
		DXGI_FORMAT				m_format;
		uint64_t m_offset;
		uint64_t m_Size;
	};

	struct BufferCreateFlags
	{
		enum Enum
		{
			ConstantBuffer = 1 << 1,
			StructuredBuffer = 1 << 2,
			StandardBuffer = 1 << 3,
			TextureBuffer = 1 << 4,
			ArgumentsBuffer = 1 << 5,
			AppendBuffer = 1 << 6,
			VertexBuffer = 1 << 7,
			IndexBuffer = 1 << 8,
		};
	};

	struct BufferBindFlags
	{
		enum Enum
		{
			None = 0,
			SRV = 1 << 1,
			UAV = 1 << 2
		};
	};

	class Buffer
	{
	public:
		Buffer() {}
		Buffer(uint64_t noofElements, uint64_t elementSize,
			BufferCreateFlags::Enum createFlags, BufferBindFlags::Enum bindFlags = BufferBindFlags::None,
			DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
			void* data = nullptr);

		Buffer(const Buffer& buf);
		Buffer(Buffer& buf);
		Buffer(Buffer&& buf);
		Buffer& operator=(Buffer&& buf);
		Buffer& operator=(const Buffer& buf);

		virtual ~Buffer();

		const GpuBuffer& GetBuffer() const;

		ID3D11ShaderResourceView* GetBufferSRV() const;

		ID3D11UnorderedAccessView* GetBufferUAV() const;

		bool IsDirty() const;

		DXGI_FORMAT GetFormat() const;

		uint64_t GetElementSize() const;

		void updateBuffer();

	private:
		void* m_CpuAddr;
		uint64_t m_CpuOffset;
		uint64_t m_CpuSize;
		uint64_t m_ElementSize;

		GpuBuffer m_GpuBuffer;
		bool	m_Dirty;
	};


	class GpuBufferManager
	{
	public:

		GpuBuffer createConstantBuffer(uint64_t request_size);





	private:
		std::vector<ID3D11Buffer*> m_allocated_buffers;
	};


}
