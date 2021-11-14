#include "TerrainPipe_Renderer.h"
#include "../../Engine.h"
#include "../../../Component/Graphics/_SubGraphics.h"
#include "../../../Template/EditorWindow/EditorWindow.h"
#include "../../../Template/Editor_ImageView/Editor_ImageView.h"
#include "../../../Component/Physics/_Collider.h"

/// <summary>
/// 
/// </summary>
enum class TERRAIN_TOOL_SETTING
{
	CREATER, PAINT, HEIGHT, FOLIAGE, SETTING
};
TERRAIN_TOOL_SETTING terrain_tool_setting = TERRAIN_TOOL_SETTING::CREATER;
void terrain_tool_setting_field()
{
	const char* str[] = { "CREATER", "PAINT", "HEIGHT", "FOLIAGE", "SETTING" };
	if (ImGui::BeginCombo(
		"select mode##TERRAIN", 
		str[(int)terrain_tool_setting]))
	{
		bool is_selected = false;
		if (ImGui::Selectable("CREATER", &is_selected))
		{
			terrain_tool_setting = TERRAIN_TOOL_SETTING::CREATER;
		}
		if (ImGui::Selectable("PAINT", &is_selected))
		{
			terrain_tool_setting = TERRAIN_TOOL_SETTING::PAINT;
		}
		if (ImGui::Selectable("HEIGHT", &is_selected))
		{
			terrain_tool_setting = TERRAIN_TOOL_SETTING::HEIGHT;
		}
		if (ImGui::Selectable("FOLIAGE", &is_selected))
		{
			terrain_tool_setting = TERRAIN_TOOL_SETTING::FOLIAGE;
		}
		if (ImGui::Selectable("SETTING", &is_selected))
		{
			terrain_tool_setting = TERRAIN_TOOL_SETTING::SETTING;
		}

		if (is_selected)
		{
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

// �ͷ��� ������ ���� �����ʹ� �̰��� ���δ�.
namespace TERRAIN
{
	XMFLOAT3 m_pos;
	enum class TERRAIN_HEIGHT_TOOL
	{
		ADD, SMOOTH, FIT
	};
	TERRAIN_HEIGHT_TOOL terrain_height_tool = TERRAIN_HEIGHT_TOOL::ADD;
	float fit_height = 0.0f;
	void terrain_height_tool_field()
	{
		const char* str[] = { "ADD", "SMOOTH", "FIT"};
		if (ImGui::BeginCombo(
			"height mode##TERRAIN", 
			str[(int)terrain_height_tool]))
		{
			bool is_selected = false;
			if (ImGui::Selectable("ADD", &is_selected))
			{
				terrain_height_tool = TERRAIN_HEIGHT_TOOL::ADD;
			}
			if (ImGui::Selectable("SMOOTH", &is_selected))
			{
				terrain_height_tool = TERRAIN_HEIGHT_TOOL::SMOOTH;
			}
			if (ImGui::Selectable("FIT", &is_selected))
			{
				terrain_height_tool = TERRAIN_HEIGHT_TOOL::FIT;
			}
			ImGui::EndCombo();
		}
		switch (terrain_height_tool)
		{
		case TERRAIN::TERRAIN_HEIGHT_TOOL::ADD:
			break;
		case TERRAIN::TERRAIN_HEIGHT_TOOL::SMOOTH:
			break;
		case TERRAIN::TERRAIN_HEIGHT_TOOL::FIT:
			ImGui::float_field(&fit_height, "fit height");
			break;
		}
	}
	SafePtr<TEXTURE::Texture> texture;
	std::unique_ptr<TEXTURE::TextureData> texture_data;
	void terrain_texture_field()
	{
		ImGui::base_field(texture, "brush##TERRAIN");
		auto bfi = ImGui::get_base_field_instance();

		if (bfi->state == ImGui::base_field_state::IS_CHANGED)
		{
			if (texture.is_vaild())
			{
				if (texture->GetTextureResourceView() == nullptr)
				{
					texture->load_resource(EDITOR::Shared::get_shared()->engine);
				}
				texture->make_texture_data(texture_data);
			}
		}
	}
	float get_terrain_color(float u, float v)
	{
		return TERRAIN::texture_data->read_color_with_coordinate(u, v).rgba[0];
	}
	bool is_texture_vaild()
	{
		return (
			texture.is_vaild() &&
			texture_data.get() != nullptr
			);
	}
	float brush_size = 10.0f;
	float brush_weight = 0.1f;
	void brush_size_and_weight_field()
	{
		ImGui::InputFloat("brush size##TERRAIN", &brush_size);
		if (brush_size < 0.0f) brush_size = 1.0f;
		ImGui::InputFloat("brush weight##TERRAIN", &brush_weight);
	}
	int paint_index = 0;
	float paint_weight = 1.0f;
	void paint_index_and_weight_field()
	{
		ImGui::INT_field(&paint_index, "paint index");
		ImGui::float_field(&paint_weight, "paint weight");
	}
	int foliage_index = 0;
	int foliage_size = 1;
	float foliage_brush_size = 0;
	SafePtr<MESH::MeshMesh>* p_foliage_mesh_mesh = nullptr;
	SafePtr<SHADER::MeshShader>* p_foliage_instance_shader = nullptr;
	void foliage_field(TerrainRenderer* terrain_renderer)
	{
		ImGui::INT_field(&foliage_index, "foliage index");
		ImGui::INT_field(&foliage_size, "foliage size");
		ImGui::float_field(&foliage_brush_size, "foliage brush size");

		// condition
		if (terrain_renderer->terrain_pipe.mesh.is_vaild() == false) return;
		ImGui::NewLine();
		auto& fmap = terrain_renderer->terrain_pipe.mesh->foliage_info_map;
		auto it = fmap.find(foliage_index);
		if (it != fmap.end())
		{
			p_foliage_mesh_mesh = &it->second.mesh_mesh;
			p_foliage_instance_shader = &it->second.mesh_shader;

			auto bpi = ImGui::get_base_field_instance();
			ImGui::base_field(*p_foliage_mesh_mesh, "foliage mesh");
			ImGui::base_field(*p_foliage_instance_shader, "foliage shader");

			if (bpi->state == ImGui::base_field_state::IS_CHANGED)
				terrain_renderer->terrain_pipe.mesh->fix_foliage_pipeline();

			ImGui::NewLine();
			if (ImGui::RightAngleButton("Delete foliage"))
			{
				terrain_renderer->terrain_pipe.mesh
					->delete_foliage(foliage_index);
			}
		}
		else
		{
			ImGui::NewLine();
			if (ImGui::RightAngleButton("Create foliage"))
			{
				terrain_renderer->terrain_pipe.mesh
					->create_foliage(foliage_index);
			}
		}

		/*std::string foliage_count_text = "Foliage count : " + terrain_renderer->terrain_pipe.mesh->terrain_section_data.begin
		ImGui::Text()*/
	}
}

//////
// BTYPE::Terrain

//////
// Terrain mesh

#include <ctime>
#include "TerrainPipe_Renderer.h"

//////
// Terrain renderer

void TerrainRenderer::draw(SubGraphics* sub_graphics)
{
	auto CEW = EDITOR::CEW::get_instance();
	if (CEW->last_select.get() == this->owner.get())
	{
		set_brush_shape(TERRAIN::m_pos, TERRAIN::brush_size);
	}
	else
	{
		set_brush_shape(TERRAIN::m_pos, TERRAIN::brush_size, false);
	}

	terrain_pipe.draw(sub_graphics, owner->world);
}
void TerrainRenderer::draw_mesh_only(SubGraphics* sub_graphics)
{
	auto CEW = EDITOR::CEW::get_instance();
	if (CEW->last_select.get() == this->owner.get())
	{
		set_brush_shape(TERRAIN::m_pos, TERRAIN::brush_size);
	}
	else
	{
		set_brush_shape(TERRAIN::m_pos, TERRAIN::brush_size, false);
	}

	terrain_pipe.draw_mesh_only(sub_graphics, owner->world);
}

void TerrainRenderer::get_hit_result_by_editor_view(Hit& hit)
{

	auto cew = EDITOR::Create_editor_window::get_instance();

	// Ray�� ���� ��
	// ���̸� ���ϱ� ���ؼ��� ���÷����� ������,
	// ���콺�� ��ġ�� �ʿ�� �Ѵ�.
	Line ray;
	ImVec2 rm_pos = cew->edwiv->get_relative_mouse_position();
	cew->camera_3d->make_ray_by_screen_pos(
		ray,
		cew->edwiv->width,
		cew->edwiv->height,
		rm_pos.x - DEFINE_IMAGE_PADDING,
		rm_pos.y - DEFINE_IMAGE_PADDING
	);

	// Ray�� terrain�� hit�� ���� ��
	hit.position = XMVECTOR();
	hit.is_hit = false;
	hit.distance = D3D11_FLOAT32_MAX;
	for (auto& mesh : this->terrain_pipe.mesh->terrain_section_data)
	{
		Hit Temp{};
		Temp.distance = D3D11_FLOAT32_MAX;

		Collider::line_mesh_interaction(
			ray, Temp,
			mesh.second.vertex,
			mesh.second.index,
			false
		);

		if (Temp.is_hit == true)
		{
			if (hit.distance > Temp.distance)
			{
				hit = Temp;
			}
		}
	}
}

//void TerrainRenderer::make_relative_keys(
//	float x, float y,
//	std::vector<std::vector<int>>& keys
//)
//{
//	// condition
//	keys.clear();
//
//	// �귯�� ����� �������� �����ؾ��� ���͵��� ���� ���.
//
//	float start_x, end_x, start_y, end_y, mesh_size;
//	start_x		= x - TERRAIN::brush_size;
//	end_x		= x + TERRAIN::brush_size;
//	start_y		= y - TERRAIN::brush_size;
//	end_y		= y + TERRAIN::brush_size;
//	mesh_size	= static_cast<float>(this->terrain_pipe.mesh->size);
//
//	int ks_x, ke_x, ks_y, ke_y;
//	ks_x = static_cast<int>(start_x	/ mesh_size);
//	ke_x = static_cast<int>(end_x / mesh_size);
//	ks_y = static_cast<int>(start_y	/ mesh_size);
//	ke_y = static_cast<int>(end_y / mesh_size);
//
//	// �ͷ����� ������ ��ŭ �̵��ϸ� �ͷ��� map���� �ߺ��Ǵ� ���� ����.
//	for (int x = ks_x; x <= ke_x; ++x)
//		for (int y = ks_y; y <= ke_y; ++y)
//			keys.push_back({x, y});
//}

/// <summary>
/// TERRAIN�� �귯�� ũ�⸦ �������� ������ ���س�
/// �ͷ����� �޽��� ��ȿ�ϴٴ� �����Ͽ� �۾��� �̷����.
/// </summary>
/// <param name="terrain_renderer"></param>
/// <param name="key"></param>
/// <param name=""></param>
template <typename FUNC>
void make_range(
	TerrainRenderer* terrain_renderer, 
	const std::vector<int>& key, 
	const XMFLOAT3& position,
	FUNC func
)
{
	auto section = &terrain_renderer->terrain_pipe.mesh->terrain_section_data[key];
	float MinX = position.x - TERRAIN::brush_size / 2.0f;
	float MaxX = position.x + TERRAIN::brush_size / 2.0f;
	float MinZ = position.z - TERRAIN::brush_size / 2.0f;
	float MaxZ = position.z + TERRAIN::brush_size / 2.0f;

	concurrency::parallel_for((size_t)0, section->vertex.size(),
		[=](size_t index)
		{
			// �� �����
			if (section->vertex[index].Position.x >= MinX &&
				section->vertex[index].Position.x <= MaxX &&
				section->vertex[index].Position.z >= MinZ &&
				section->vertex[index].Position.z <= MaxZ)
			{
				func
				(
					index, 
					section->vertex[index],
					section->terrain[index],
					TERRAIN::get_terrain_color
					(
						(section->vertex[index].Position.x - MinX) / TERRAIN::brush_size,
						(section->vertex[index].Position.z - MinZ) / TERRAIN::brush_size
					)
				);
			}
		});
}
template <typename FUNC>
void make_range_no_parallel_for(
	TerrainRenderer* terrain_renderer,
	const std::vector<int>& key,
	const XMFLOAT3& position,
	FUNC func
)
{
	auto section = &terrain_renderer->terrain_pipe.mesh->terrain_section_data[key];
	float MinX = position.x - TERRAIN::brush_size / 2.0f;
	float MaxX = position.x + TERRAIN::brush_size / 2.0f;
	float MinZ = position.z - TERRAIN::brush_size / 2.0f;
	float MaxZ = position.z + TERRAIN::brush_size / 2.0f;

	for (size_t index = 0; index < section->vertex.size(); ++index)
	{
		// �� �����
		if (section->vertex[index].Position.x >= MinX &&
			section->vertex[index].Position.x <= MaxX &&
			section->vertex[index].Position.z >= MinZ &&
			section->vertex[index].Position.z <= MaxZ)
		{
			float u = (section->vertex[index].Position.x - MinX) / TERRAIN::brush_size;
			float v = (section->vertex[index].Position.z - MinZ) / TERRAIN::brush_size;

			func
			(
				index,
				section->vertex[index],
				section->terrain[index],
				TERRAIN::get_terrain_color(u, v)
			);
		}
	}
}
// size_t index
// BTYPE::Vertex_default& vertex
// BTYPE::Terrain& terrain
// float amount = (texture_data�� uv �ݵ����Ʈ�� rgba���� r ��)
#define make_range_lambda_param \
size_t index, \
BTYPE::Vertex_default& vertex, \
BTYPE::Terrain& terrain, \
float amount

void TerrainRenderer::adjust_height(
	const std::vector<int>& key,
	const XMFLOAT3& position
)
{
	// check condition
	if (TERRAIN::texture_data == nullptr) return;
	if (terrain_pipe.mesh.is_vaild() == false) return;
	if (terrain_pipe.mesh->terrain_section_data.find(key) == terrain_pipe.mesh->terrain_section_data.end()) return;

	//
	auto shared = EDITOR::Shared::get_shared();
	float delta_time = shared->engine->timer.get_deleta_time();

	// 
	make_range(this, key, position,
		[=](make_range_lambda_param)
		{
			vertex.Position.y
				+= amount
				* TERRAIN::brush_weight
				* delta_time;
		});
	terrain_pipe.mesh->create_section_buffer(key);
}

void TerrainRenderer::fit_height(
	const std::vector<int>& key, 
	const XMFLOAT3& position, 
	float fit_height
)
{
	// check condition
	if (TERRAIN::texture_data == nullptr) return;
	if (terrain_pipe.mesh.is_vaild() == false) return;
	if (terrain_pipe.mesh->terrain_section_data.find(key) == terrain_pipe.mesh->terrain_section_data.end()) return;

	//
	auto shared = EDITOR::Shared::get_shared();
	float delta_time = shared->engine->timer.get_deleta_time();

	// 
	make_range(this, key, position,
		[=](make_range_lambda_param)
		{
			vertex.Position.y
				+= (fit_height - vertex.Position.y)
				* amount
				* TERRAIN::brush_weight
				* delta_time;
		});
	terrain_pipe.mesh->create_section_buffer(key);
}

#define DEFINE_WEIGHT_ARR_SIZE 3
// ���� ���˾� ���ڳ�.
void TerrainRenderer::paint_texture(
	const std::vector<int>& key, 
	const XMFLOAT3& position
)
{
	// check condition
	if (TERRAIN::texture_data == nullptr) return;
	if (terrain_pipe.mesh.is_vaild() == false) return;
	if (terrain_pipe.mesh->terrain_section_data.find(key) == terrain_pipe.mesh->terrain_section_data.end()) return;

	auto shared = EDITOR::Shared::get_shared();
	float delta_time = shared->engine->timer.get_deleta_time();
	float add_amount = TERRAIN::paint_weight * delta_time;
	// 
	make_range(this, key, position,
		[=](make_range_lambda_param)
		{
			//// ���Խ��� ����Ʈ�� �� �ε����� ������ ���ϰ�
			//// ����Ʈ ���� ���� ������ �ϴ� ������ ����.

			//// 1. �ε��� ������ ������.
			//int last_weighted_index = 0;
			//float* weight_arr = &terrain.Weight.x;
			//for (
			//	int temp_index = 0; 
			//	temp_index < DEFINE_WEIGHT_ARR_SIZE; 
			//	++temp_index
			//	)
			//{
			//	// last_weighted_index�� ������ 0 ~ 2������ �ȴ�.
			//	if (weight_arr[temp_index] == 0.0f)
			//	{
			//		last_weighted_index = temp_index;
			//		break;
			//	}
			//}
			//// ������ weight�� 0�� �ƴҰ��
			//if (weight_arr[DEFINE_WEIGHT_ARR_SIZE - 1] != 0.0f)
			//{
			//	last_weighted_index = DEFINE_WEIGHT_ARR_SIZE;
			//}
			//// 2. ��ġ�� ���� �ִ��� ������ Ȯ���� �Ŀ� �۾��� �����Ѵ�.
			//int painted_texture_index = -1;
			//int* index_arr = &terrain.Index.x;
			//for (int i = 0; i < DEFINE_WEIGHT_ARR_SIZE; ++i)
			//{
			//	if (index_arr[i] == TERRAIN::paint_index)
			//	{
			//		painted_texture_index = i;
			//		break;
			//	}
			//}
			//if (painted_texture_index == -1)
			//{
			//	painted_texture_index = last_weighted_index;
			//}
			//// 3. �ε��� ������ ���� ����Ʈ ���ҿ� �߰��� ������.
			//float deleta 
			//	= amount
			//	* TERRAIN::brush_weight
			//	* delta_time;
			//if (painted_texture_index != DEFINE_WEIGHT_ARR_SIZE)
			//{
			//	index_arr[painted_texture_index]
			//		= TERRAIN::paint_index;
			//	weight_arr[painted_texture_index]
			//		+= deleta;
			//}
			////else // last_weighted_index == DEFINE_WEIGHT_ARR_SIZE
			////{
			////	// ��� ����Ʈ�� ��ȿ�� ������ ���ִ� ���
			////	// ���ο� ����Ʈ�� �߰��ϱ� ���ٴ� ���� �����ϴ� �� �켱�Ѵ�.
			////}
			//// ����Ʈ ���Ҵ� ��� ������ �ؾ��ϱ� ������ ������ ���� ���� �´�.
			//float sub_amount = deleta / (float)last_weighted_index;
			//for (int i = 0; i < last_weighted_index; ++i)
			//{
			//	weight_arr[i] -= sub_amount;
			//}
			//// ����Ʈ�� �������� �ٿ�� �ϰ� ������ ���� �����ϵ��� ��.
			/// �ִ��� ������ ��� ����..
			/// map���� weight������� �����ϰ� ū ������� ���ϸ� �׳��� �����ϴ�.
			
			int* index_arr = &terrain.Index.x;
			float* weight_arr = &terrain.Weight.x;
			// �ε��� ���ϱ�

			int registed_size = 0;
			{
				for (int i = 0; i < DEFINE_WEIGHT_ARR_SIZE; ++i)
					if (weight_arr[i] > 0) ++registed_size;
			}

			if (registed_size == 0)
			{
				// ��ȿ�� �ε����� ���ٸ�.. 
				//������ �ִ� ���̹Ƿ� �ʱ���·� �����.
				index_arr[0] = 0;
				index_arr[1] = 0;
				index_arr[2] = 0;

				weight_arr[0] = 1;
				weight_arr[1] = 0;
				weight_arr[2] = 0;
			}
			else
			{
				std::map<int, float> map;

				// ��Ƽ�ʿ� ����Ʈ �ε��� �� �Ҵ�.
				bool have_index = false;
				for (int i = 0; i < DEFINE_WEIGHT_ARR_SIZE; ++i)
				{
					if (weight_arr[i] > 0.f)
						if (index_arr[i] == TERRAIN::paint_index)
						{
							have_index = true;
							map.insert(
								std::pair(
									index_arr[i],
									weight_arr[i] + add_amount * amount
								));
						}
						else
						{
							map.insert(
								std::pair(
									index_arr[i],
									weight_arr[i] - add_amount * amount / (float)registed_size
								));
						}
				}
				if (have_index == false)
					map.insert(
						std::pair(
							TERRAIN::paint_index,
							add_amount * amount
						));

				std::multimap<float, int> weight_index_map;
				for (auto& pair : map)
					weight_index_map.insert(std::make_pair(pair.second, pair.first));

				// ���⼭ ����Ʈ ����Ʈ ����

				index_arr[0] = 0;
				index_arr[1] = 0;
				index_arr[2] = 0;
				weight_arr[0] = 0;
				weight_arr[1] = 0;
				weight_arr[2] = 0;

				// ������ ������� �Ҵ�
				int index = 0;
				auto pair = weight_index_map.rbegin();
				float weighted_sum = 0.f;
				while (
					pair != weight_index_map.rend() &&
					index < DEFINE_WEIGHT_ARR_SIZE &&
					pair->first > 0.f &&
					weighted_sum < 1.0f
					)
				{
					if (weighted_sum + pair->first >= 1.0f)
					{
						index_arr[index] = pair->second;
						weight_arr[index] = 1.f - weighted_sum;
					}
					else
					{
						index_arr[index] = pair->second;
						weight_arr[index] = std::min(pair->first, 1.0f);
					}

					weighted_sum += pair->first;
					++pair;
					++index;
				}

				//// �븻������
				//XMVECTOR vec;
				//vec.m128_f32[0] = weight_arr[0];
				//vec.m128_f32[1] = weight_arr[1];
				//vec.m128_f32[2] = weight_arr[2];
				//vec = DirectX::XMVector3Normalize(vec);
				//weight_arr[0] = vec.m128_f32[0];
				//weight_arr[1] = vec.m128_f32[1];
				//weight_arr[2] = vec.m128_f32[2];
			}
		});
	terrain_pipe.mesh->create_section_buffer(key);
}
void TerrainRenderer::make_foliage(
	const XMFLOAT3& position,
	int foliage_index,
	float brush_size,
	int foliage_size
)
{
	if (this->terrain_pipe.mesh.is_vaild() == false) return;
	if (
		this->terrain_pipe.mesh->foliage_info_map.find(foliage_index) 
		== this->terrain_pipe.mesh->foliage_info_map.end()
		) 
		return;

	int num_of_foliage = this->terrain_pipe.mesh->get_num_of_foliage(
		foliage_index,
		position.x,
		position.z,
		brush_size);

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �־��� ������� ������.")
	int delta_size = foliage_size - num_of_foliage;
	while (delta_size > 0)
	{
		// foliage�� �߰��ϴ� �۾�.
		this->terrain_pipe.mesh->add_single_foliage(
			foliage_index,
			position.x,
			position.z,
			brush_size
		);
		delta_size--;
	}
	while (delta_size < 0)
	{
		// foliage�� ���ִ� �۾�.
		this->terrain_pipe.mesh->remove_single_foliage(
			foliage_index,
			position.x,
			position.z,
			brush_size
		);
		delta_size++;
	}
}
void TerrainRenderer::set_brush_shape(XMFLOAT3& pos, float b_size, bool is_true)
{
	auto ec = EngineContext::get_instance();
	if (TERRAIN::texture.is_vaild() == false || is_true == false)
	{
		ConstantBuffer<BTYPE::CB_TERRAIN_MPOS> t;
		t.initialize(ec->Device.Get());
		t.data.m_pos = {0.0f, 0.0f, 0.0f};
		t.data.b_size = 0.0f;
		t.apply_changes(ec->Device_context.Get());
		t.set_constant_buffer(ec->Device_context.Get());

		//ec->Device_context->PSSetSamplers(10, 1, nullptr);
		//ec->Device_context->PSSetShaderResources(10, 1, nullptr);
		return;
	}
	else
	{
		ConstantBuffer<BTYPE::CB_TERRAIN_MPOS> t;
		t.initialize(ec->Device.Get());
		t.data.m_pos = pos;
		t.data.b_size = b_size;
		t.apply_changes(ec->Device_context.Get());
		t.set_constant_buffer(ec->Device_context.Get());

		ec->Device_context->PSSetSamplers(10, 1, TERRAIN::texture->GetSamplerStateAddress());
		ec->Device_context->PSSetShaderResources(10, 1, TERRAIN::texture->GetTextureResourceViewAddress());
		return;
	}
}

//////
// Terrain tool�� ���õ� ������ ���⿡ �����Ѵ�.

void TerrainRenderer::draw_detail_view()
{
	ViaRenderer::draw_detail_view();

	if (ImGui::CollapsingHeader("Terrain renderer"))
	{
		terrain_tool_setting_field();
		// �ͷ��� �� ���� ����
		ImGui::NewLine();
		this->terrain_pipe.draw_terrain_pipe_detail_view();
		ImGui::NewLine();
		// test code
		{

			////���콺 ������ Ȯ��
			//auto cew = EDITOR::Create_editor_window::get_instance();
			//ImVec2 rm_pos = cew->edwiv->get_relative_mouse_position();
			//ImGui::InputFloat2("mouse pos", &rm_pos[0]);

		}
		
		// share setting

		// �ͷ��� ���� �׸��� ����
		auto cew = EDITOR::Create_editor_window::get_instance();
		switch (terrain_tool_setting)
		{
		case TERRAIN_TOOL_SETTING::CREATER:
		{
			static std::vector<int> vec;
			if (vec.size() == 0) vec.resize(2);

			ImGui::INT2_field(vec.data(), "Section point");
			ImGui::NewLine();
			if (ImGui::RightAngleButton("Create button"))
				this->terrain_pipe.mesh->create_section(vec);

			// ������� ������ ���� â.
			if (ImGui::TreeNode("section list"))
			{
				if (this->terrain_pipe.mesh.is_vaild())
				{
					for (const auto& pair : this->terrain_pipe.mesh->terrain_section_data)
					{
						std::string str
							= std::to_string(pair.first[0])
							+ "_"
							+ std::to_string(pair.first[1]);
						ImGui::Text(str.c_str());
					}
				}
				ImGui::TreePop();
			}
		}
		break;
		case TERRAIN_TOOL_SETTING::PAINT:
		{
			TERRAIN::brush_size_and_weight_field();
			ImGui::NewLine();
			TERRAIN::terrain_texture_field();
			ImGui::NewLine();
			TERRAIN::paint_index_and_weight_field();
			ImGui::NewLine();

			if (cew->edwiv->is_selected_window)
			{

				// Ray�� terrain�� hit�� ���� ��
				Hit result;
				this->get_hit_result_by_editor_view(result);

				if (result.is_hit)
				{
					XMFLOAT3 m_pos; XMStoreFloat3(&m_pos, result.position);
					TERRAIN::m_pos = m_pos;
				}

				// �浹�Ǵ� ������ ���� ��.
				if (ImGui::GetIO().MouseDown[0] &&
					result.is_hit)
				{
					XMFLOAT3 pos; XMStoreFloat3(&pos, result.position);

					std::vector<std::vector<int>> keys;
					this->terrain_pipe.mesh->make_relative_keys(pos.x, pos.z, keys, TERRAIN::brush_size);
					for (auto& key : keys)
						this->paint_texture(key, pos);
				}
			}
			else
			{
				XMFLOAT3 m_pos{};
				TERRAIN::m_pos = m_pos;
			}
		}
		break;
		case TERRAIN_TOOL_SETTING::HEIGHT:
		{
			TERRAIN::brush_size_and_weight_field();
			ImGui::NewLine();
			TERRAIN::terrain_texture_field();
			ImGui::NewLine();
			TERRAIN::terrain_height_tool_field();
			ImGui::NewLine();

			if (cew->edwiv->is_selected_window)
			{

				// Ray�� terrain�� hit�� ���� ��
				Hit result;
				this->get_hit_result_by_editor_view(result);

				if (result.is_hit)
				{
					XMFLOAT3 m_pos; XMStoreFloat3(&m_pos, result.position);
					TERRAIN::m_pos = m_pos;
				}

				// ���� ���콺�� ������ ��
				if (ImGui::GetIO().MouseDown[0] &&
					result.is_hit)
				{
					XMFLOAT3 pos; XMStoreFloat3(&pos, result.position);

					std::vector<std::vector<int>> keys;
					this->terrain_pipe.mesh->make_relative_keys(pos.x, pos.z, keys, TERRAIN::brush_size);
					switch (TERRAIN::terrain_height_tool)
					{
					case TERRAIN::TERRAIN_HEIGHT_TOOL::ADD:
						for (auto& key : keys)
							this->adjust_height(key, pos);
						break;
					case TERRAIN::TERRAIN_HEIGHT_TOOL::SMOOTH:
					{
						// ��հ� ���ϱ�
						static int count;
						static float target_height;

						count = 0;
						target_height = 0.0f;
						for (auto& key : keys)
						{
							make_range_no_parallel_for(
								this, key, pos,
								[=](make_range_lambda_param)
								{
									++count;
									target_height += vertex.Position.y;
								});
						}
						target_height /= (float)count;

						if (count == 0) break;
						for (auto& key : keys)
						{
							this->fit_height(key, pos, target_height);
						}
					}
					break;
					case TERRAIN::TERRAIN_HEIGHT_TOOL::FIT:
						for (auto& key : keys)
							this->fit_height(key, pos, TERRAIN::fit_height);
						break;
					}
				}
				// end ���� ���콺�� ������ ��
			}
			else
			{
				XMFLOAT3 m_pos{};
				TERRAIN::m_pos = m_pos;
			}
		}
		break;
		case TERRAIN_TOOL_SETTING::FOLIAGE:
		{
			TERRAIN::foliage_field(this);
			ImGui::NewLine();

			if (cew->edwiv->is_selected_window)
			{
				// Ray�� terrain�� hit�� ���� ��
				Hit result;
				this->get_hit_result_by_editor_view(result);

				// ���� ���콺�� ������ ��
				if (ImGui::GetIO().MouseDown[0] &&
					result.is_hit)
				{
					XMFLOAT3 pos; XMStoreFloat3(&pos, result.position);
					make_foliage(
						pos,
						TERRAIN::foliage_index,
						TERRAIN::foliage_brush_size,
						TERRAIN::foliage_size
					);
				}
				// end ���� ���콺�� ������ ��
			}
		}
		break;
		case TERRAIN_TOOL_SETTING::SETTING:

			if (terrain_pipe.mesh.is_vaild() == false)
			{
				ImGui::Text(" Terrain mesh is not vaild.");
			}
			else
			{
				bool c1 = ImGui::INT_field(&this->terrain_pipe.mesh->size, "Size");
				bool c2 = ImGui::INT_field(&this->terrain_pipe.mesh->num_of_splits, "Num of splits");
				bool c3 = ImGui::INT_field(&this->terrain_pipe.mesh->uv_x, "UV x");
				bool c4 = ImGui::INT_field(&this->terrain_pipe.mesh->uv_y, "UV y");

				// ���ð��� �ϳ��� ������ ���� ��.
				if (c1 || c2 || c3 || c4)
				{
					// ��ü ������ �ٽ� ����� �Լ�.
					this->terrain_pipe.mesh->terrain_section_data.clear();
					//for (auto& pair : this->terrain_pipe.mesh->terrain_section_data)
					//{
					//	this->terrain_pipe.mesh->create_section_buffer(pair.first);
					//}
				}
			}
			// default
			break;
		}
	}
}

