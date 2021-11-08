#pragma once

#include "../InstancePipe/InstancePipe.h"
#include "../../../Component/Physics/_Collider.h"

namespace BTYPE
{
	struct Terrain
	{
		XMFLOAT3	Weight;
		XMINT3		Index;

		Terrain();
		Terrain(float weight_0, int index_0, float weight_1, int index_1, float weight_2, int index_2);

		static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc);
	};
}

namespace SHADER
{
	class TerrainShader : public MeshShader
	{
	public:
		TerrainShader() : MeshShader()
		{
			this->raw_ms.rasterizer_desc.CullMode = D3D11_CULL_BACK;
			raw_ms.pixel_shader_path = L"ps_Terrain";
		}
	};
	REGIST_RESOURCE(TerrainShader);
	//namespace RESOURCECONSTRUCT {
	//	struct ResourceConstruct_TerrainShader : public EDITOR::ResourceConstruct
	//	{
	//		ResourceConstruct_TerrainShader() : ResourceConstruct("TerrainShader") {}
	//		virtual void create_resource(std::unique_ptr<Resource>& resource)
	//		{
	//			auto result = std::make_unique<MeshShader>();
	//			result->raw_ms.pixel_shader_path = L"terrain_pshader_Terrain";

	//			resource = std::move(result);
	//		}
	//	};
	//	static ResourceConstruct_TerrainShader ResourceConstruct_TerrainShaderInstance;
	//}
}

namespace MESH
{
	class TerrainMesh : public Mesh
	{
	public:
		struct TerrainSection
		{
			// vertex
			std::vector<BTYPE::Vertex_default>				vertex;
			std::vector<BTYPE::Terrain>						terrain;
			VertexBuffer<BTYPE::Vertex_default>				buffer_vertex;
			VertexBuffer<BTYPE::Terrain>					buffer_terrain;
			// index
			std::vector<BTYPE::IB_Default>					index;
			IndexBuffer<BTYPE::IB_Default>					buffer_index;
			// Foliage
			std::map<int, InstancePipe>						foliages;

			TerrainSection();
		};
		struct FoliageInfo
		{
			SafePtr<MESH::MeshMesh>			mesh_mesh;
			SafePtr<SHADER::MeshShader>		mesh_shader;
		};

		int size = 100;
		int num_of_splits = 19;
		int uv_x = 10;
		int uv_y = 10;

		std::map<int, FoliageInfo>					foliage_info_map;
		std::map<std::vector<int>, TerrainSection>	terrain_section_data;

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		void set_terrain_mesh(SubGraphics* sub_graphics, std::vector<int> key);
		void draw_terrain_mesh(SubGraphics* sub_graphics, std::vector<int> key);

		void pos_to_key(
			float x, float y,
			std::vector<int>& result
		);
		void make_relative_keys(
			float x, float y,
			std::vector<std::vector<int>>& keys,
			float brush_size
		);

		// 굳이 외울 필요는 없나.. 필요할 때 만들면 될테니??
		void create_section(std::vector<int> key);
		void create_section_buffer(std::vector<int> key);

		void create_foliage(int foliage_key);
		void delete_foliage(int foliage_key);

		int get_num_of_foliage(
			int foliage_index,
			float pos_x,
			float pos_y,
			float brush_size
		);
		void add_single_foliage(
			int foliage_index,
			float pos_x,
			float pos_y,
			float brush_size
		);
		//
		// 과연 이 코드를 다시 봤을 때 알아 볼 수 있을 것인가??..
		void remove_single_foliage(
			int foliage_index,
			float pos_x,
			float pos_y,
			float brush_size
		);
		void fix_foliage_pipeline();

		template <typename PayLoadType>
		void serialize_terrain_info(PayLoadType* payload)
		{
			payload->with(&this->size);
			payload->with(&this->num_of_splits);
			payload->with(&this->uv_x);
			payload->with(&this->uv_y);
		}
		void save_binary(std::string path);
		void load_binary(std::string path);

		virtual void draw_detail_view() override;
	};
	REGIST_RESOURCE(TerrainMesh);
}

class TerrainPipe
{
public:
	SafePtr<SHADER::TerrainShader> shader;
	SafePtr<MESH::TerrainMesh> mesh;

	bool is_draw_able();

	void draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void draw_terrain_pipe_detail_view();
};
