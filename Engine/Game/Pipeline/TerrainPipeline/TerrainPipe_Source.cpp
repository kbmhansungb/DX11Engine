#include "TerrainPipe_Source.h"
#include "../../../Component/Graphics/_SubGraphics.h"


BTYPE::Terrain::Terrain() : Weight(1.0f, 0.0f, 0.0f), Index(0, 0, 0) {}

BTYPE::Terrain::Terrain(float weight_0, int index_0, float weight_1, int index_1, float weight_2, int index_2)
{
	Weight.x = weight_0;
	Weight.y = weight_1;
	Weight.z = weight_2;

	Index.x = index_0;
	Index.y = index_1;
	Index.z = index_2;
}

void BTYPE::Terrain::T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc)
{
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT, 1,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
}

MESH::TerrainMesh::TerrainSection::TerrainSection()
{
}

void MESH::TerrainMesh::load_resource(Engine* engine)
{
}

void MESH::TerrainMesh::release_resource(Engine* engine)
{
}

void MESH::TerrainMesh::set_terrain_mesh(SubGraphics* sub_graphics, std::vector<int> key)
{
	auto& section = this->terrain_section_data[key];

	UINT strides[] = { section.buffer_vertex.Stride(), section.buffer_terrain.Stride() };
	ID3D11Buffer* buffer_address[] = { section.buffer_vertex.Get(), section.buffer_terrain.Get() };
	UINT offset[] = { 0, 0 };

	sub_graphics->Device_context
		->IASetVertexBuffers(0, 2, buffer_address, strides, offset);

	sub_graphics->Device_context
		->IASetIndexBuffer(section.buffer_index.Get(), section.buffer_index.get_index_buffer_type(), 0);
}

void MESH::TerrainMesh::draw_terrain_mesh(SubGraphics* sub_graphics, std::vector<int> key)
{
	auto& section = this->terrain_section_data[key];

	sub_graphics->Device_context->DrawIndexed(section.index.size(), 0, 0);
}

void MESH::TerrainMesh::pos_to_key(float x, float y, std::vector<int>& result)
{
	result.resize(2);
	int x_offset = 0; if (x < 0) x_offset -= 1;
	int y_offset = 0; if (y < 0) y_offset -= 1;
	result[0] = static_cast<int>(x / size) + x_offset;
	result[1] = static_cast<int>(y / size) + y_offset;
}

void MESH::TerrainMesh::make_relative_keys(
	float x, float y, 
	std::vector<std::vector<int>>& keys,
	float brush_size)
{
	// condition
	keys.clear();

	// 브러시 사이즈를 바탕으로 조작해야할 벡터들을 만들어서 사용.

	float start_x, end_x, start_y, end_y, mesh_size;
	start_x = x - brush_size;
	end_x = x + brush_size;
	start_y = y - brush_size;
	end_y = y + brush_size;
	mesh_size = static_cast<float>(this->size);

	std::vector<int> start_key, end_key;
	pos_to_key(start_x, start_y, start_key);
	pos_to_key(end_x, end_y, end_key);

	int ks_x, ke_x, ks_y, ke_y;
	ks_x = start_key[0];
	ke_x = end_key[0];
	ks_y = start_key[1];
	ke_y = end_key[1];

	// 터레인의 사이즈 만큼 이동하면 터레인 map에서 중복되는 경우는 없다.
	for (int x = ks_x; x <= ke_x; ++x)
		for (int y = ks_y; y <= ke_y; ++y)
		{
			keys.push_back({x, y});
		}
}

// 굳이 외울 필요는 없나.. 필요할 때 만들면 될테니??

void MESH::TerrainMesh::create_section(std::vector<int> key)
{
	if (this->terrain_section_data.find(key) != this->terrain_section_data.end())
		this->terrain_section_data.erase(key);

	// 기억해두자. []오퍼레이터로 가져오면 아무것도 없을 때 디폴트로 생성한다.
	auto& section = this->terrain_section_data[key];

	int index_size = this->num_of_splits;
	int vertices_size = (index_size + 1) * (index_size + 1);
	int indices_size = index_size * index_size * 6;

	section.vertex.resize(vertices_size);
	section.terrain.resize(vertices_size);
	section.index.resize(indices_size);

	float stride = (float)size / (float)index_size;
	float x_uv_stride = (float)uv_x / (float)index_size;
	float y_uv_stride = (float)uv_y / (float)index_size;

	float base_x = (float)key[0] * (float)size;
	float base_y = (float)key[1] * (float)size;

	// vertex, terrain 정보 설정.
	for (int y = 0; y < index_size + 1; ++y)
		for (int x = 0; x < index_size + 1; ++x)
		{
			int now_index = y * (index_size + 1) + x;
			section.vertex[now_index].Position = XMFLOAT3(x * stride + base_x, 0.0f /*임시로 원래는 0*/, y * stride + base_y);
			section.vertex[now_index].Texture_coordinate.x = x * x_uv_stride;
			section.vertex[now_index].Texture_coordinate.y = y * y_uv_stride;
			section.vertex[now_index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			section.terrain[now_index].Weight.x = 1.0f;
		}

	// index 정보 설정.
	for (int y_plane = 0; y_plane < index_size; ++y_plane)
		for (int x_plane = 0; x_plane < index_size; ++x_plane)
		{
			int index_line = index_size * y_plane * 6 + x_plane * 6;
			section.index[index_line + 0] = (y_plane + 0) * (index_size + 1) + x_plane + 0;
			section.index[index_line + 2] = (y_plane + 0) * (index_size + 1) + x_plane + 1;
			section.index[index_line + 1] = (y_plane + 1) * (index_size + 1) + x_plane + 0;
			section.index[index_line + 3] = (y_plane + 1) * (index_size + 1) + x_plane + 0;
			section.index[index_line + 5] = (y_plane + 0) * (index_size + 1) + x_plane + 1;
			section.index[index_line + 4] = (y_plane + 1) * (index_size + 1) + x_plane + 1;
		}

	// 여기서 만들어진 텍스쳐의 처리를 해줘야함.


	// buffer 생성
	create_section_buffer(key);
}

void MESH::TerrainMesh::create_section_buffer(std::vector<int> key)
{
	if (this->terrain_section_data.find(key) == this->terrain_section_data.end()) return;

	auto context = EngineContext::get_instance();
	auto& section = this->terrain_section_data[key];

	section.buffer_vertex.free_buffer();
	section.buffer_vertex.initialize(
		context->Device.Get(),
		section.vertex.data(),
		section.vertex.size());

	section.buffer_terrain.free_buffer();
	section.buffer_terrain.initialize(
		context->Device.Get(),
		section.terrain.data(),
		section.terrain.size());

	section.buffer_index.free_buffer();
	section.buffer_index.initialize(
		context->Device.Get(),
		section.index.data(),
		section.index.size());
}

void MESH::TerrainMesh::create_foliage(int foliage_key)
{
	foliage_info_map[foliage_key];
}

void MESH::TerrainMesh::delete_foliage(int foliage_key)
{
	foliage_info_map.erase(foliage_key);
	for (auto& section : terrain_section_data)
	{
		section.second.foliages.erase(foliage_key);
	}
}

int MESH::TerrainMesh::get_num_of_foliage(int foliage_index, float pos_x, float pos_y, float brush_size)
{
	std::vector<std::vector<int>> keys;
	make_relative_keys(pos_x, pos_y, keys, brush_size);

	int count_of_foliage = 0;
	float half_of_brush = brush_size / 2.f;
	float min_x = pos_x - half_of_brush;
	float max_x = pos_x + half_of_brush;
	float min_y = pos_y - half_of_brush;
	float max_y = pos_y + half_of_brush;

	for (auto& key : keys)
	{
		if (this->terrain_section_data.find(key) == this->terrain_section_data.end()) continue;

		auto& it = this->terrain_section_data[key];
		auto& foliage_pipe = it.foliages[foliage_index];

		for (auto& elum : foliage_pipe.worlds.data)
		{
			DirectX::XMVECTOR s, r, t;
			DirectX::XMMatrixDecompose(&s, &r, &t, elum.world);

			if (
				t.m128_f32[0] <= max_x && t.m128_f32[0] >= min_x &&
				t.m128_f32[2] <= max_y && t.m128_f32[2] >= min_y
				)
			{
				count_of_foliage++;
			}
		}
	}
	return count_of_foliage;
}

void MESH::TerrainMesh::add_single_foliage(int foliage_index, float pos_x, float pos_y, float brush_size)
{
	srand((unsigned int)time(NULL));
	// 0 ~ 1까지 정규화된 값을 가져옴
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * brush_size;
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * brush_size;
	float half_of_brush = brush_size / 2.f;

	float repos_x = pos_x - half_of_brush + r1;
	float repos_y = pos_y - half_of_brush + r2;
	// 키를 만들고 키가 유효한지 검사
	std::vector<int> key;
	pos_to_key(repos_x, repos_y, key);

	auto it = this->terrain_section_data.find(key);
	if (it != this->terrain_section_data.end())
	{
		Line st{};
		st.type = Line::TYPE::STRAIGHT;
		st.origin.m128_f32[0] = repos_x;
		st.origin.m128_f32[2] = repos_y;
		st.vec.m128_f32[1] = 1.0f;
		Hit hit;
		Collider::line_mesh_interaction(
			st,
			hit,
			it->second.vertex,
			it->second.index,
			true
		);
		float height = hit.position.m128_f32[1];

		DirectX::XMVECTOR
			s{ 1.f, 1.f, 1.f, 0.f },
			r{},
			t{ repos_x, height, repos_y, 1.0f };
		XMMATRIX mat
			= DirectX::XMMatrixScaling(s.m128_f32[0], s.m128_f32[1], s.m128_f32[2])
			* DirectX::XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f)
			* DirectX::XMMatrixTranslation(t.m128_f32[0], t.m128_f32[1], t.m128_f32[2]);

		it->second.foliages[foliage_index].worlds.data
			.push_back(mat);
		it->second.foliages[foliage_index].worlds
			.initialize(EngineContext::get_instance()->Device.Get());
	}

	fix_foliage_pipeline();
}

//
// 과연 이 코드를 다시 봤을 때 알아 볼 수 있을 것인가??..

void MESH::TerrainMesh::remove_single_foliage(int foliage_index, float pos_x, float pos_y, float brush_size)
{
	srand((unsigned int)time(NULL));

	float half_of_brush = brush_size / 2.f;
	float min_x = pos_x - half_of_brush;
	float max_x = pos_x + half_of_brush;
	float min_y = pos_y - half_of_brush;
	float max_y = pos_y + half_of_brush;

	// key중 하나를 선택
	std::vector<std::vector<int>> keys;
	make_relative_keys(pos_x, pos_y, keys, brush_size);
	auto& key = keys[rand() % keys.size()];

	auto it = this->terrain_section_data.find(key);
	if (it != this->terrain_section_data.end())
	{
		auto& foliage_pipe = it->second.foliages[foliage_index];

		std::vector<int> allow_indexes;
		int index = 0;
		for (auto& elum : foliage_pipe.worlds.data)
		{
			// SRT를 구하고 범위 안에 있다면
			DirectX::XMVECTOR s, r, t;
			DirectX::XMMatrixDecompose(&s, &r, &t, elum.get_world());
			if (
				t.m128_f32[0] <= max_x && t.m128_f32[0] >= min_x &&
				t.m128_f32[2] <= max_y && t.m128_f32[2] >= min_y
				)
			{
				allow_indexes.emplace_back(index);
			}
			index++;
		}

		// 폴리에이지 월드 벡터에서 삭제
		if (allow_indexes.size() == 0) return;

		foliage_pipe.worlds.data
			.erase(foliage_pipe.worlds.data.begin() + allow_indexes[rand() % allow_indexes.size()]);
		foliage_pipe.worlds.initialize(EngineContext::get_instance()->Device.Get());

		// 파이프가 필요없으면 삭제
		if (foliage_pipe.worlds.data.size() == 0)
			it->second.foliages.erase(foliage_index);
	}
}

void MESH::TerrainMesh::fix_foliage_pipeline()
{
	for (auto& pair : this->terrain_section_data)
	{
		for (auto& foliage_pair : pair.second.foliages)
		{
			foliage_pair.second.mesh = foliage_info_map[foliage_pair.first].mesh_mesh;
			foliage_pair.second.shader = foliage_info_map[foliage_pair.first].mesh_shader;
		}
	}
}

void MESH::TerrainMesh::save_binary(std::string path)
{
	BinarySave::BinarySavePayload bsp = BinarySave::BinarySavePayload(path);
	serialize_terrain_info(&bsp);

	//////
	// foliage info map
	SIZE_T foliage_info_map_size = foliage_info_map.size();
	bsp.write(&foliage_info_map_size);
	for (auto& pair : foliage_info_map)
	{
		int key = pair.first;
		std::string mesh_str = "NULL";
		std::string shader_str = "NULL";
		if (pair.second.mesh_mesh.is_vaild())
			mesh_str = pair.second.mesh_mesh->get_name().get_string();
		if (pair.second.mesh_shader.is_vaild())
			shader_str = pair.second.mesh_shader->get_name().get_string();

		bsp.write(&key);
		bsp.write_string(mesh_str);
		bsp.write_string(shader_str);
	}

	//////
	// terrain section data
	SIZE_T terrain_section_data_size = terrain_section_data.size();
	bsp.write(&terrain_section_data_size);
	for (auto& pair : terrain_section_data)
	{
		std::vector<int> key = pair.first;
		bsp.write_array(&key);
		
		bsp.write_array(&pair.second.vertex);
		bsp.write_array(&pair.second.terrain);
		bsp.write_array(&pair.second.index);

		// foliage 설정.
		SIZE_T foliages_size = pair.second.foliages.size();
		bsp.write(&foliages_size);
		for (auto& fpair : pair.second.foliages)
		{
			bsp.write(&fpair.first);
			bsp.write_array(&fpair.second.worlds.data);
		}
	}
}

void MESH::TerrainMesh::load_binary(std::string path)
{
	BinaryLoad::BinaryLoadPayload blp = BinaryLoad::BinaryLoadPayload(path);
	serialize_terrain_info(&blp);

	//////
	// foliage info map
	SIZE_T foliage_info_map_size;
	blp.read(&foliage_info_map_size);
	for (SIZE_T index = 0; index < foliage_info_map_size; ++index)
	{
		int key;
		std::string mesh_str;
		std::string shader_str;

		blp.read(&key);
		blp.read_string(mesh_str);
		blp.read_string(shader_str);

		SafePtr<MESH::MeshMesh> mesh_mesh;
		SafePtr<SHADER::MeshShader> mesh_shader;

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 일단 되게 만들자. 나중에 파라메터로 넘기면 될듯하다.")
		auto shared = EDITOR::Shared::get_shared();
		Engine* engine = shared->engine;
		if (mesh_str != "NULL")
		{
			NAME::Name mesh_name = NAME::Name(&engine->name_container, mesh_str);
			mesh_mesh = engine->resources.get_resource(mesh_name).cast<MESH::MeshMesh>();
		}
		if (shader_str != "NULL")
		{
			NAME::Name shader_name = NAME::Name(&engine->name_container, shader_str);
			mesh_shader = engine->resources.get_resource(shader_name).cast<SHADER::MeshShader>();
		}

		auto& foliage_info = foliage_info_map[key];
		foliage_info.mesh_mesh = mesh_mesh;
		foliage_info.mesh_shader = mesh_shader;
	}

	//////
	// terrain section data
	SIZE_T terrain_section_data_size;
	blp.read(&terrain_section_data_size);
	for (SIZE_T index = 0; index < terrain_section_data_size; ++index)
	{
		std::vector<int> key;
		blp.read_array(&key);

		auto& terrain_section = terrain_section_data[key];
		
		blp.read_array(&terrain_section.vertex);
		blp.read_array(&terrain_section.terrain);
		blp.read_array(&terrain_section.index);

		// buffer는 어떻게 관리해야 할까.

		// foliage 설정.
		SIZE_T foliages_size;
		blp.read(&foliages_size);
		for (SIZE_T index2 = 0; index2 < foliages_size; ++ index2)
		{
			int key;
			blp.read(&key);

			auto& instance_pipe = terrain_section.foliages[key];
			blp.read_array(&instance_pipe.worlds.data);
		}
	}

	// 버퍼 재생성.

}

void MESH::TerrainMesh::draw_detail_view()
{
	Mesh::draw_detail_view();

	if (ImGui::CollapsingHeader("TerrainMesh"))
	{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 구현 아직 안함")
		
		static std::string t_path;
		ImGui::Text_field(t_path, "Save path", 100);
		
		std::string p = t_path + ".tmd";
		ImGui::NewLine();
		if (ImGui::RightAngleButton("Save"))
		{
			save_binary(p);
		}
		ImGui::NewLine();
		if (ImGui::RightAngleButton("Load"))
		{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 음.. 버퍼 재할당 해줘야함.")
			load_binary(p);
		}
	}
}

bool TerrainPipe::is_draw_able()
{
	if (this->mesh.is_vaild() == false) return false;
	if (this->shader.is_vaild() == false) return false;
	return true;
}

static SHADER::VertexShader vs;
static void smart_load(SubGraphics* sub_graphics)
{
	if (vs.GetShader() == nullptr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
		BTYPE::Vertex_default::T_get_input_layout(input_layout);
		BTYPE::Terrain::T_get_input_layout(input_layout);

		vs.initialize(
			sub_graphics->Device,
			L"vs_Terrain",
			input_layout
		);
	}
	sub_graphics->Device_context->VSSetShader(
		vs.GetShader(),
		0,
		0
	);
	sub_graphics->Device_context->IASetInputLayout(
		vs.GetInputLayout()
	);
}
void TerrainPipe::draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able() == false) return;

	smart_load(sub_graphics);
	world.set_constant_buffer(sub_graphics->Device_context);
	for (auto& pair : this->mesh->terrain_section_data)
	{
		shader->set_mesh_shader(sub_graphics);
		mesh->set_terrain_mesh(sub_graphics, pair.first);
		mesh->draw_terrain_mesh(sub_graphics, pair.first);
	}

	for (auto& pair : this->mesh->terrain_section_data)
	{
		for (auto& foliage_pair : pair.second.foliages)
		{
			foliage_pair.second.draw(sub_graphics, world);
		}
	}
}

void TerrainPipe::draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able() == false) return;

	smart_load(sub_graphics);
	world.set_constant_buffer(sub_graphics->Device_context);
	for (auto& pair : this->mesh->terrain_section_data)
	{
		mesh->set_terrain_mesh(sub_graphics, pair.first);
		mesh->draw_terrain_mesh(sub_graphics, pair.first);
	}
	for (auto& pair : this->mesh->terrain_section_data)
	{
		for (auto& foliage_pair : pair.second.foliages)
		{
			foliage_pair.second.draw_mesh_only(sub_graphics, world);
		}
	}
}

void TerrainPipe::draw_terrain_pipe_detail_view()
{
	ImGui::base_field(shader, "Terrain shader");
	ImGui::base_field(mesh, "Terrain mesh");
}
