#include "RenderPass.h"

namespace FX
{





	Mesh::Mesh(const std::vector<Vertex>& raw_vb, const std::vector<Index>& raw_ib, const MeshType& mesh_type)
	{
		std::vector<glm::vec3>* vertexPositions = new std::vector<glm::vec3>();
		std::vector<glm::vec4>* vertexColors = new std::vector<glm::vec4>();
		std::vector<Index>* indexes = new std::vector<Index>();

		for (int i = 0; i < raw_vb.size(); i++)
		{
			vertexPositions->emplace_back(raw_vb[i].m_Pos);
			vertexColors->emplace_back(raw_vb[i].m_Color);
		}

		for (int i = 0; i < raw_ib.size(); i++)
		{
			indexes->emplace_back(raw_ib[i]);
		}

		m_GpuVb = new Buffer(vertexPositions->size(), sizeof(glm::vec3),
			BufferCreateFlags::Enum::VertexBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN,
			vertexPositions);

		m_GpuColorsVb = new Buffer(vertexColors->size(), sizeof(glm::vec4),
			BufferCreateFlags::Enum::VertexBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN,
			vertexColors);

		m_GpuIb = new Buffer(indexes->size(), sizeof(Index),
			BufferCreateFlags::Enum::VertexBuffer, BufferBindFlags::Enum::None, DXGI_FORMAT_UNKNOWN,
			indexes);

		delete(vertexPositions);
		delete(vertexColors);
		delete(indexes);


		//build shader and layout

		ShaderDesc sd;
		if (mesh_type == MeshType::StaticMesh)
		{
			D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "COLOR"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"WORLD",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1}
			};
			sd.m_InputDesc = inputLayoutDesc;
			sd.m_InputElementNums = 3;
		}
		else if (mesh_type == MeshType::DynamicMesh)
		{
			D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "COLOR"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			sd.m_InputDesc = inputLayoutDesc;
			sd.m_InputElementNums = 2;
		}

		sd.m_HlslFile = L"C:\\workspace\\Phoenix\\Content\\Shaders\\NaiveBox.hlsl";
		Shader* naiveBoxShader = createShader(sd);
		m_Shader = naiveBoxShader;
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