#include "base/FXCommon.h"
#include "GlobalContext.hpp"
#include "SwapChain.h"
#include "GpuBuffer.h"


namespace FX {

    Buffer::Buffer(const Buffer& buf)
    :m_CpuAddr(buf.m_CpuAddr),
    m_Dirty(buf.m_Dirty),
    m_ElementSize(buf.m_ElementSize),
    m_GpuBuffer(buf.m_GpuBuffer),
    m_CpuSize(buf.m_CpuSize)
    {}

    Buffer::Buffer(Buffer& buf)
        :m_CpuAddr(buf.m_CpuAddr),
        m_Dirty(buf.m_Dirty),
        m_ElementSize(buf.m_ElementSize),
        m_GpuBuffer(buf.m_GpuBuffer),
        m_CpuSize(buf.m_CpuSize)
    {}

    Buffer::Buffer(Buffer&& buf)
        :m_CpuAddr(buf.m_CpuAddr),
        m_Dirty(buf.m_Dirty),
        m_ElementSize(buf.m_ElementSize),
        m_GpuBuffer(buf.m_GpuBuffer),
        m_CpuSize(buf.m_CpuSize)
    {}

    Buffer& Buffer::operator=(Buffer&& buf)
    {
        m_CpuAddr = buf.m_CpuAddr;
        m_Dirty = buf.m_Dirty;
        m_ElementSize = buf.m_ElementSize;
        m_GpuBuffer = buf.m_GpuBuffer;
        m_CpuSize = buf.m_CpuSize;
        return *this;
    }

    Buffer& Buffer::operator=(const Buffer& buf)
    {
        m_CpuAddr = buf.m_CpuAddr;
        m_Dirty = buf.m_Dirty;
        m_ElementSize = buf.m_ElementSize;
        m_GpuBuffer = buf.m_GpuBuffer;
        m_CpuSize = buf.m_CpuSize;

        return *this;
    }


    const GpuBuffer& Buffer::GetBuffer() const
    {
        return m_GpuBuffer;
    }

    ID3D11ShaderResourceView* Buffer::GetBufferSRV() const
    {
        return m_GpuBuffer.m_bufferSRV;
    }

    ID3D11UnorderedAccessView* Buffer::GetBufferUAV() const
    {
        return m_GpuBuffer.m_bufferUAV;
    }

    bool Buffer::IsDirty() const
    {
        return m_Dirty;
    }

    DXGI_FORMAT Buffer::GetFormat() const
    {
        return m_GpuBuffer.m_format;
    }

    uint64_t Buffer::GetElementSize() const
    {
        return m_ElementSize;
    }

    uint64_t Buffer::GetNoofElements() const
    {
        return m_noofElements;
    }

    void Buffer::updateBuffer()
    {
        D3D11_MAPPED_SUBRESOURCE MappedResource;

        if (m_GpuBuffer.m_desc.Usage ==  D3D11_USAGE_STAGING)
        {
            GetD3DContext()->Map(m_GpuBuffer.m_gpu_addr, 0, D3D11_MAP_WRITE, 0, &MappedResource);
            memcpy(MappedResource.pData, m_CpuAddr, m_CpuSize);

            GetD3DContext()->Unmap(m_GpuBuffer.m_gpu_addr, 0);
        }
        else if (m_GpuBuffer.m_desc.Usage == D3D11_USAGE_DYNAMIC)
        {
            //@TODO Need to make better usage of D3D11_MAP_WRITE_DISCARD & D3D11_MAP_NOT_OVERRWRITE
            GetD3DContext()->Map(m_GpuBuffer.m_gpu_addr, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
            memcpy(MappedResource.pData, m_CpuAddr, m_CpuSize);

            GetD3DContext()->Unmap(m_GpuBuffer.m_gpu_addr, 0);
        }
        else if (m_GpuBuffer.m_desc.Usage & (D3D11_USAGE_IMMUTABLE | D3D11_USAGE_DEFAULT))
        {
            GetD3DContext()->UpdateSubresource(m_GpuBuffer.m_gpu_addr, 0, nullptr, m_CpuAddr, (UINT)m_CpuSize, 0);
        }


    }

    Buffer::Buffer(uint64_t noofElements, uint64_t elementSize,
        BufferCreateFlags::Enum createFlags, BufferBindFlags::Enum bindFlags /*= BufferBindFlags::None*/,
        DXGI_FORMAT format /*= DXGI_FORMAT_UNKNOWN*/,
        void* data /*= nullptr*/)
    {
        m_GpuBuffer.m_gpu_addr = nullptr;
        m_GpuBuffer.m_bufferSRV = nullptr;
        m_GpuBuffer.m_bufferUAV = nullptr;
        m_GpuBuffer.m_format = format;
        m_CpuSize = noofElements * elementSize;
        m_ElementSize = elementSize;
        m_noofElements = noofElements;
        m_CpuAddr = data;


        // Setup buffer
        D3D11_BUFFER_DESC Desc;
        ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));

        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Desc.ByteWidth = (UINT)(noofElements * elementSize);
        Desc.MiscFlags = 0;

        if (createFlags & BufferCreateFlags::ConstantBuffer)
        {
            Desc.Usage = D3D11_USAGE_DYNAMIC;
            Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        }
        else if (createFlags & BufferCreateFlags::VertexBuffer)
        {
            Desc.Usage = D3D11_USAGE_IMMUTABLE;
            Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            Desc.CPUAccessFlags = 0;
        }
        else if (createFlags & BufferCreateFlags::IndexBuffer)
        {
            Desc.Usage = D3D11_USAGE_IMMUTABLE;
            Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            Desc.CPUAccessFlags = 0;
        }
        else
        {
            Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            if (createFlags & BufferCreateFlags::StructuredBuffer || createFlags & BufferCreateFlags::AppendBuffer)
            {
                Desc.Usage = D3D11_USAGE_DYNAMIC;
                Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
                Desc.StructureByteStride = (UINT)elementSize;
            }
        }

        if (bindFlags & BufferBindFlags::UAV)
        {
            Desc.Usage = D3D11_USAGE_DEFAULT;
            Desc.CPUAccessFlags = 0;
            Desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        }

        if (createFlags & BufferCreateFlags::ArgumentsBuffer)
        {
            Desc.Usage = D3D11_USAGE_DEFAULT;
            Desc.CPUAccessFlags = 0;
            Desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
        }

        m_GpuBuffer.m_desc = Desc;

        if (data)
        {
            // Fill in the subresource data.
            D3D11_SUBRESOURCE_DATA InitData;
            InitData.pSysMem = data;
            InitData.SysMemPitch = 0;
            InitData.SysMemSlicePitch = 0;

            GetD3DDevice()->CreateBuffer(&Desc, &InitData, &m_GpuBuffer.m_gpu_addr);
        }
        else
        {
            GetD3DDevice()->CreateBuffer(&Desc, NULL, &m_GpuBuffer.m_gpu_addr);
        }

        if (bindFlags & BufferBindFlags::SRV)
        {
            // We need a resource view to use it in the shader
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

            if (createFlags & BufferCreateFlags::StructuredBuffer)
            {
                shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
            }
            else
            {
                shaderResourceViewDesc.Format = format;
            }

            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            shaderResourceViewDesc.Buffer.NumElements = (UINT)noofElements;

            GetD3DDevice()->CreateShaderResourceView(m_GpuBuffer.m_gpu_addr, &shaderResourceViewDesc, &m_GpuBuffer.m_bufferSRV);
        }

        if (bindFlags & BufferBindFlags::UAV)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc;
            ZeroMemory(&unorderedAccessViewDesc, sizeof(unorderedAccessViewDesc));

            unorderedAccessViewDesc.Format = format;
            unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            unorderedAccessViewDesc.Buffer.NumElements = (UINT)noofElements;

            if (createFlags & BufferCreateFlags::AppendBuffer)
            {
                unorderedAccessViewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
            }

            GetD3DDevice()->CreateUnorderedAccessView(m_GpuBuffer.m_gpu_addr, &unorderedAccessViewDesc, &m_GpuBuffer.m_bufferUAV);
        }
    }



    Buffer::~Buffer()
    {
        SAFE_RELEASE(m_GpuBuffer.m_bufferUAV);
        SAFE_RELEASE(m_GpuBuffer.m_bufferSRV);
        SAFE_RELEASE(m_GpuBuffer.m_gpu_addr);
    }


    FX::GpuBuffer GpuBufferManager::createConstantBuffer(uint64_t request_size)
    {
        D3D11_BUFFER_DESC CBDesc = {};
        CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
        CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
        CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CBDesc.ByteWidth = (UINT)request_size;

        GpuBuffer res;
        res.m_desc = CBDesc;

        GetD3DDevice()->CreateBuffer(&CBDesc, nullptr, &res.m_gpu_addr);

        return res;
    }

}