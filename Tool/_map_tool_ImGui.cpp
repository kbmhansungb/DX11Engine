//#include "_map_tool_ImGui.h"
//#include "ToolMain_ImGui.h"
//#include "_map_tool_component.h"
//// widget state
//
//void MapToolImGui::draw(SubGraphics* Sub_graphics)
//{
//	auto Data = ToolData::get_map_tool_data();
//	ImGuiWindowFlags Window_flags = 0;
//
//	///// <summary>
//	///// 
//	///// </summary>
//	//static ConstantBuffer<BTYPE::CB_DirectLight> Direct_light;
//	//if (Direct_light.Get() == nullptr)
//	//{
//	//	Direct_light.data.Ambient_color = XMFLOAT3(1.0f, 1.0f, 1.0f);
//	//	Direct_light.data.Ambient_strength = 0.2f;
//	//	Direct_light.data.Light_color = XMFLOAT3(1.0f, 1.0f, 1.0f);
//	//	Direct_light.data.Light_direction = XMFLOAT3(0.5, -0.5, -0.5f);
//	//	Direct_light.data.Light_strength = 0.8f;
//	//	Direct_light.initialize(Data->engine->Device.Get());
//	//	Direct_light.apply_changes(Data->engine->Device_context.Get());
//	//	Data->engine->Device_context->PSSetConstantBuffers(
//	//		Direct_light.get_constant_buffer_slot(),
//	//		1,
//	//		Direct_light.GetAddressOf());
//	//}
//
//	//int Button_size = 5;
//
//	//ImVec2 WindowPos = ImGui::GetWindowPos();
//	//WindowPos.x -= 50;
//	//WindowPos.y -= 50;
//	////ImGui::SetNextWindowSize(ImVec2(10 + Button_size * 60, 10 + 50 + 10));
//	//ImGui::SetNextWindowSize(ImVec2(200, 100));
//	//ImGui::SetNextWindowPos(WindowPos);
//
//	///// <summary>
//	///// 
//	///// </summary>
//	//Window_flags = 0;
//	//Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
//	//Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
//	//Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
//	//Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
//	//Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
//	//ImGui::Begin("Map tool icon select", 0, Window_flags);
//
//	//ImGui::Checkbox("Map tool setting", &Map_tool_setting);
//	//ImGui::Checkbox("Map tool", &Map_tool_brush);
//	//ImGui::Checkbox("Map tool material", &Map_tool_material);
//
//	//ImGui::End();
//
//	///// <summary>
//	///// 
//	///// </summary>
//	//if (Map_tool_setting)
//	//{
//	//	Window_flags = 0;
//	//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar;
//	//	ImGui::Begin("Map tool setting", &Map_tool_setting);
//
//	//	Data->camera_3D->draw_imgui();
//	//	Data->simple_move_transform->draw_imgui();
//	//	Data->terrain_renderer->draw_imgui();
//	//	if (ImGui::CollapsingHeader("Light setting"))
//	//	{
//	//		ImGui::DragFloat3("Ambient color", &Direct_light.data.Ambient_color.x, 0.01f, 0.0f, 1.0f);
//	//		ImGui::DragFloat("Ambient strength", &Direct_light.data.Ambient_strength, 0.01f, 0.0f, 1.0f);
//	//		ImGui::NewLine();
//	//		ImGui::DragFloat3("Light color", &Direct_light.data.Light_color.x, 0.01f, 0.0f, 1.0f);
//	//		ImGui::DragFloat3("Light direction", &Direct_light.data.Light_direction.x, 0.01f, -1.0f, 1.0f);
//	//		ImGui::DragFloat("Light strength", &Direct_light.data.Light_strength, 0.01f, 0.0f, 1.0f);
//
//	//		static XMVECTOR Vector;
//	//		Direct_light.apply_changes(Data->engine->Device_context.Get());
//	//		Data->engine->Device_context->PSSetConstantBuffers(
//	//			Direct_light.get_constant_buffer_slot(),
//	//			1,
//	//			Direct_light.GetAddressOf());
//	//	}
//
//	//	ImGui::End();
//	//}
//
//	/// <summary>
//	/// 
//	/// </summary>
//	if (Data->Tool_main_ImGui->State == ToolMainImGui::TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL)
//	{
//		Window_flags = 0;
//		Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar;
//		ImGui::Begin("Map tool brush", 0, Window_flags);
//
//		{
//			//ImGui::BeginChild("Select button", ImVec2(0, 50), Window_flags);
//
//			if (ImGui::Button("Mesh define", ImVec2(100, 20)))
//				Brush_state = MapToolButton::CREATE_STATE;
//
//			ImGui::SameLine();
//			if (ImGui::Button("Height brush", ImVec2(100, 20)))
//				Brush_state = MapToolButton::HEIGHT_BRUSH;
//
//			ImGui::SameLine();
//			if (ImGui::Button("Texture brush", ImVec2(100, 20)))
//				Brush_state = MapToolButton::TEXTURE_BRUSH;
//
//			ImGui::SameLine();
//			if (ImGui::Button("Foliage brush", ImVec2(100, 20)))
//				Brush_state = MapToolButton::FOLIAGE_BRUSH;
//
//			ImGui::SameLine();
//			if (ImGui::Button("Brush shape", ImVec2(100, 20)))
//				Brush_state = MapToolButton::BRUSH_SHAPE;
//
//			ImGui::SameLine();
//			if (ImGui::Button("Save and load", ImVec2(100, 20)))
//				Brush_state = MapToolButton::SAVE_AND_LOAD;
//			//ImGui::EndChild();
//		}
//
//		string temp_str;
//
//		static const int Brush_size = 3;
//		static const char* Brush_names[Brush_size] = { "Adjust", "Fit", "Smooth" };
//		SafePtr<TEXTURE::Image> image;
//		static char format[10] = "objnomtl";
//		static char path[100] = "triangle.obj";
//		static aiReturn ai_return = aiReturn::aiReturn_FAILURE;
//
//		static int Texture_select_index = 0;
//		static int Foliage_select_index = 0;
//
//		static char file_name[100] = "save";
//		static HRESULT save_hr = E_FAIL;
//		static int in_line = 0;
//		static int texture_slot = 0;
//		ImGui::BeginChild("Display");
//		{
//			switch (Brush_state)
//			{
//			case MapToolButton::CREATE_STATE:
//				ImGui::Text(" * Mesh define");
//				ImGui::InputFloat("square_size", &this->square_size);
//				ImGui::InputInt("num_of_splits", &this->num_of_splits);
//				ImGui::InputInt("Max uv x", &this->Max_uv_x);
//				ImGui::InputInt("Max uv y", &this->Max_uv_y);
//				if (ImGui::Button("Create new mesh"))
//				{
//					Data->terrain_renderer->size = this->square_size;
//					Data->terrain_renderer->num_of_splits = this->num_of_splits;
//					Data->terrain_renderer->max_uv_x = this->Max_uv_x;
//					Data->terrain_renderer->max_uv_y = this->Max_uv_y;
//					Data->terrain_renderer->terrain_meshs.clear();
//					Data->terrain_renderer->create_square_plane({ 0, 0 });
//				}
//				ImGui::NewLine();
//				//ImGui::Checkbox("Point", &Data->extend_map);
//				ImGui::InputInt2("Point", Data->now_selected_index.data());
//				if (ImGui::Button("add new terrain"))
//				{
//					Data->terrain_renderer->create_square_plane(Data->now_selected_index);
//				}
//				ImGui::SameLine();
//				if (ImGui::Button("remove terrain"))
//				{
//					Data->terrain_renderer->terrain_meshs.erase(Data->now_selected_index);
//				}
//				if (Data->terrain_renderer->terrain_meshs.find(Data->now_selected_index) != Data->terrain_renderer->terrain_meshs.end())
//					ImGui::Text("have terrain");
//				else
//					ImGui::Text("no terrain");
//
//				ImGui::NewLine();
//				ImGui::Text("terrain state");
//				for (auto iter = Data->terrain_renderer->terrain_meshs.begin(); iter != Data->terrain_renderer->terrain_meshs.end(); ++iter)
//				{
//					temp_str = "( " + to_string(iter->first[0]) + " " + to_string(iter->first[1]) + " )";
//					ImGui::Text(temp_str.c_str());
//				}
//
//				break;
//			case MapToolButton::HEIGHT_BRUSH:
//				ImGui::Text(" * Height brush");
//				ImGui::SliderFloat("brush_size", &this->brush_size, 0.0f, this->square_size);
//				ImGui::SliderFloat("brush_weight", &this->brush_weight, 0.0f, 1000.0f);
//				ImGui::ListBox("Brush", &this->now_selected_brush_type, Brush_names, Brush_size);
//				switch (this->now_selected_brush_type)
//				{
//				case 0:
//					ImGui::SliderFloat("Amount multifly", &this->amount, -1.0f, 1.0f);
//					break;
//				case 1:
//					ImGui::SliderFloat("Height", &this->height, -1000, 1000);
//					break;
//				case 2:
//					break;
//				}
//				break;
//			case MapToolButton::TEXTURE_BRUSH:
//				ImGui::Text(" * Texture brush");
//				ImGui::SliderFloat("brush_size", &this->brush_size, 0.0f, this->square_size);
//				ImGui::NewLine();
//				ImGui::SliderFloat("Texture weight", &this->texture_weight, 0, 1);
//				ImGui::SliderInt("Texture", &this->Texture_index, 0, 9);
//				ImGui::NewLine();
//				ImGui::Text("Texture_input");
//				ImGui::InputInt("Slot", &texture_slot);
//				if (texture_slot > TERRAIN_TEXTURE_SIZE) texture_slot = TERRAIN_TEXTURE_SIZE; //일단은 이렇게 하자. 사용하는 텍스쳐의 최대 갯수 이다.
//				if (texture_slot < 0) texture_slot = 0;
//				if (Data->terrain_material->Terrain_textures[texture_slot].is_vaild())
//				{
//					ImGui::Text(
//						Data->terrain_material->Terrain_textures[texture_slot]
//						->get_name().get_string().c_str());
//				}
//				else
//				{
//					ImGui::Text("No texture");
//				}
//				ImGui::ListBox("Select texture", &Texture_select_index, Data->texture_names.data(), Data->texture_names.size());
//
//				if (ImGui::Button("Clear"))
//				{
//					Data->terrain_material->Terrain_textures[texture_slot].reset();
//				}
//				ImGui::SameLine();
//				if (ImGui::Button("Insert"))
//				{
//					string insert_name = Data->texture_names[Texture_select_index];
//					if (find(Data->textures.begin(), Data->textures.end(), insert_name)
//						!= Data->textures.end())
//					{
//						Data->terrain_material->Terrain_textures[texture_slot]
//							.set(Data->engine->get_resource(Data->engine->make_name(insert_name)));
//					}
//					Data->terrain_material->update_terrain_texture();
//				}
//
//				for (int index = 0; index < TERRAIN_TEXTURE_SIZE; ++index)
//				{
//					if (Data->terrain_material->Terrain_textures[index].is_vaild() == false)
//					{
//						string text = "( " + to_string(index) + " " + "empty" + " )";
//						ImGui::Text(text.c_str());
//					}
//					else
//					{
//						string text = "( " + to_string(index) + " " + Data->terrain_material->Terrain_textures[index]->get_name().get_string() + " )";
//						ImGui::Text(text.c_str());
//					}
//				}
//				break;
//			case MapToolButton::FOLIAGE_BRUSH:
//				ImGui::Text(" * Foliage brush");
//
//				ImGui::SliderFloat("brush_size", &this->brush_size, 0.0f, this->square_size);
//
//				ImGui::InputInt("Index", &terrain_foliage_index);
//				//ImGui::SameLine();
//				//if (Data->terrain_renderer->foliage_meshs.find(terrain_foliage_index)
//				//	== Data->terrain_renderer->foliage_meshs.end())
//				//	ImGui::Text("empty");
//				//else
//				//	ImGui::Text(Data->terrain_renderer->foliage_meshs.find(terrain_foliage_index)->second->get_name().get_string().c_str());
//
//				ImGui::ListBox("model name", &Foliage_select_index, Data->model_names.data(), Data->model_names.size());
//				if (ImGui::Button("Insert"))
//				{
//					Data->terrain_renderer->foliage_meshs[terrain_foliage_index]
//						= Data->engine->get_resource(
//							Data->engine->make_name(Data->model_names[Foliage_select_index])).cast<MODEL::Model>();
//				}
//				ImGui::SameLine();
//				if (ImGui::Button("Remove"))
//				{
//					Data->terrain_renderer->foliage_meshs.erase(terrain_foliage_index);
//				}
//
//				ImGui::NewLine();
//				ImGui::InputInt("Max foliage count", &max_folliage_count);
//
//				ImGui::NewLine();
//				ImGui::Text("Foliage state");
//				for (
//					auto iter = Data->terrain_renderer->foliage_meshs.begin();
//					iter != Data->terrain_renderer->foliage_meshs.end();
//					++iter)
//				{
//					temp_str = "( " + to_string(iter->first) + " " + iter->second->get_name().get_string() + " )";
//					ImGui::Text(temp_str.c_str());
//				}
//				break;
//			case MapToolButton::BRUSH_SHAPE:
//				for (auto iter = Data->brushes.begin(); iter != Data->brushes.end(); ++iter)
//				{
//					image = Data->engine->get_resource(
//						Data->engine->make_name(*iter)).cast<TEXTURE::Image>();
//					if (ImGui::ImageButton(
//						image->GetTextureResourceView(),
//						ImVec2(50.0f, 50.0f)))
//					{
//						Data->terrain_renderer->set_decal_texture(image);
//					}
//
//					if (in_line < 7)
//					{
//						ImGui::SameLine();
//						++in_line;
//					}
//					else if (in_line)
//					{
//						in_line = 0;
//					}
//				}
//				break;
//			case MapToolButton::SAVE_AND_LOAD:
//				ImGui::InputText("File name", file_name, 100);
//				ImGui::NewLine();
//
//#pragma region save
//
//
//				ImGui::SameLine();
//				if (ImGui::Button("Save"))
//				{
//					string path = "Save/"; path += file_name; path += ".md";
//					StringSaver ss(path);
//
//					ss.set_line("terrain renderer");
//					// terrain 상태 저장.
//					ss.set_line(
//						to_string(Data->terrain_renderer->size)
//						+ " " + to_string(Data->terrain_renderer->num_of_splits)
//						+ " " + to_string(Data->terrain_renderer->max_uv_x)
//						+ " " + to_string(Data->terrain_renderer->max_uv_y)
//					);
//					ss.set_line("end terrain renderer");
//					// foliage 저장
//					ss.set_line("foliage name");
//					for (auto& pair : Data->terrain_renderer->foliage_meshs)
//						ss.set_line(
//							to_string(pair.first)
//							+ " " + pair.second->get_name().get_string()
//						);
//					ss.set_line("end foliage name");
//					ss.set_line("terrain material");
//					for (int index = 0; index < TERRAIN_TEXTURE_SIZE; ++index)
//					{
//						string line = to_string(index) + " ";
//						if (Data->terrain_material->Terrain_textures[index].is_vaild())
//							line += Data->terrain_material->Terrain_textures[index]->get_name().get_string();
//						else
//						{
//						} // 유효하지 않을 경우 아무것도 없음.
//						ss.set_line(line);
//					}
//					ss.set_line("end terrain material");
//
//					// height map과 버텍스 칼라, 폴리지 데이터 저장.
//					int width_height = Data->terrain_renderer->num_of_splits + 1;
//
//					for (auto& iter : Data->terrain_renderer->terrain_meshs)
//					{
//						string map_path = "Save/" + string(file_name) + "_" + to_string(iter.first[0]) + "_" + to_string(iter.first[1]) + "_";
//
//						// height map 저장
//						TextureData height_data = TextureData(width_height, width_height);
//						for (int index = 0; index < iter.second.Vertex_vector.size(); ++index)
//							height_data.Byte_array[index].dds = (iter.second.Vertex_vector[index].Position.y);
//
//						string height_file_name = map_path + "height.dds";
//						unique_ptr<ID3D11Texture2D> height_texture = height_data.create_dds_texture_from_data(Data->engine->Device.Get());
//						save_hr = SaveDDSTextureToFile(Data->engine->Device_context.Get(), height_texture.release(), StringHelper::StringToWide(height_file_name).c_str());
//
//						// color map 저장
//						TextureData color_data = TextureData(width_height, width_height, 4);
//						for (int index = 0; index < iter.second.Vertex_vector.size(); ++index)
//						{
//							color_data.Byte_array[index * 4 + 0].dds = iter.second.Vertex_vector[index].VertexColor.x;
//							color_data.Byte_array[index * 4 + 1].dds = iter.second.Vertex_vector[index].VertexColor.y;
//							color_data.Byte_array[index * 4 + 2].dds = iter.second.Vertex_vector[index].VertexColor.z;
//							color_data.Byte_array[index * 4 + 3].dds = iter.second.Vertex_vector[index].VertexColor.w;
//						}
//						string color_file_name = map_path + "color.dds";
//
//						unique_ptr<ID3D11Texture2D> color_texture = color_data.create_dds_texture_from_data(Data->engine->Device.Get(), DXGI_FORMAT_R32G32B32A32_FLOAT, 4);
//						save_hr = SaveDDSTextureToFile(Data->engine->Device_context.Get(), color_texture.release(), StringHelper::StringToWide(color_file_name).c_str());
//
//						// 폴리지 데이터 저장.
//						StringSaver ss_f(map_path + ".mfd");
//						for (auto& type_foliage_data_pair : iter.second.Foliages)
//							for (auto& foliage_data : type_foliage_data_pair.second)
//							{
//								ss_f.set_line(
//									to_string(type_foliage_data_pair.first)
//									+ " " + to_string(foliage_data.Position.x)
//									+ " " + to_string(foliage_data.Position.y)
//									+ " " + to_string(foliage_data.Position.z)
//									+ " " + to_string(foliage_data.Rotation.x)
//									+ " " + to_string(foliage_data.Rotation.y)
//									+ " " + to_string(foliage_data.Rotation.z)
//									+ " " + to_string(foliage_data.Scale.x)
//									+ " " + to_string(foliage_data.Scale.y)
//									+ " " + to_string(foliage_data.Scale.z)
//								);
//							}
//					}
//				} // end save button
//#pragma endregion
//
//				  // assimp를 debug x86으로 만들면 사용가능.
//				  //// export
//				  //ImGui::NewLine();
//				  //ImGui::InputText("Format", format, 10);
//				  //ImGui::InputText("Path", path, 100);
//				  //if (ImGui::Button("Export"))
//				  //{
//				  //	// Assimp로 세이브 하자.
//				  //	ai_return = assimp_export(format, path);
//				  //}
//				  //ImGui::SameLine();
//				  //switch (ai_return)
//				  //{
//				  //case aiReturn_SUCCESS:
//				  //	ImGui::Text("Success");
//				  //	break;
//				  //case aiReturn_FAILURE:
//				  //	ImGui::Text("Failure");
//				  //	break;
//				  //case aiReturn_OUTOFMEMORY:
//				  //	ImGui::Text("Out of memory");
//				  //	break;
//				  //case _AI_ENFORCE_ENUM_SIZE:
//				  //	ImGui::Text("Enforce enum size");
//				  //	break;
//				  //}
//
//#pragma region load
//				ImGui::SameLine();
//				if (ImGui::Button("Load"))
//				{
//					string path = "Save/"; path += file_name; path += ".md";
//					StringReader sr(path);
//					if (sr.get_is_opened() == false)
//					{
//						save_hr = E_FAIL;
//						return;
//					}
//					sr.get_line(); // terrain renderer
//
//					string terrain_line = sr.get_line();
//					Data->terrain_renderer->size = stof(terrain_line.substr(0, terrain_line.find(" ") + 1));
//					terrain_line = terrain_line.substr(terrain_line.find(" ") + 1);
//
//					Data->terrain_renderer->num_of_splits = stoi(terrain_line.substr(0, terrain_line.find(" ") + 1));
//					terrain_line = terrain_line.substr(terrain_line.find(" ") + 1);
//
//					Data->terrain_renderer->max_uv_x = stoi(terrain_line.substr(0, terrain_line.find(" ") + 1));
//					terrain_line = terrain_line.substr(terrain_line.find(" ") + 1);
//
//					Data->terrain_renderer->max_uv_y = stoi(terrain_line);
//					Data->terrain_renderer->terrain_meshs.clear();
//
//					sr.get_line(); // end terrain renderer
//					sr.get_line(); // foliage name
//
//					string foliage = sr.get_line();
//					while (foliage != "end foliage name")
//					{
//						Data->terrain_renderer->foliage_meshs.clear();
//						int foliage_index = stoi(foliage.substr(0, foliage.find(" ") + 1));
//						foliage = foliage.substr(foliage.find(" ") + 1);
//
//						SafePtr<MODEL::Model> foliage_model = Data->engine->get_resource(Data->engine->make_name(foliage)).cast<MODEL::Model>();
//						if (foliage_model.is_vaild())
//						{
//							Data->terrain_renderer->foliage_meshs[foliage_index] = foliage_model;
//						}
//						foliage = sr.get_line();
//					}
//
//					//sr.get_line(); // end foliage name //루프문에 의해 읽힘.
//					sr.get_line(); // terrain material
//
//					string texture = sr.get_line();
//					for (int index = 0; index < TERRAIN_TEXTURE_SIZE; ++index)
//					{
//						Data->terrain_material->Terrain_textures[index].reset();
//					}
//					while (texture != "end terrain material")
//					{
//						int terrain_index = stoi(texture.substr(0, texture.find(" ") + 1));
//						texture = texture.substr(texture.find(" ") + 1);
//
//						SafePtr<TEXTURE::Texture> texture_re = Data->engine->get_resource(Data->engine->make_name(texture)).cast<TEXTURE::Texture>();
//						if (texture_re.is_vaild())
//						{
//							Data->terrain_material->Terrain_textures[terrain_index] = texture_re;
//						}
//						texture = sr.get_line();
//					}
//					Data->terrain_material->update_terrain_texture();
//
//					//sr.get_line(); // end terrain material //루프문에 의해 읽힘.
//
//					vector<string> dds_file_in_saved = get_files_inDirectory("Save\\", "*.dds");
//					// height map과 버텍스칼라, 폴리지데이터를 바탕으로 새로만듬.
//					for (auto& dds_file_name : dds_file_in_saved)
//					{
//						if (dds_file_name.find(file_name) != string::npos)
//						{
//							string key = dds_file_name.substr(dds_file_name.find("_") + 1);
//							int key_x = stoi(key.substr(0, key.find("_") + 1));
//							key = key.substr(key.find("_") + 1);
//							int key_y_index = key.find("_") + 1;
//							string key_y_sub_str = key.substr(0, key_y_index);
//							int key_y = stoi(key_y_sub_str);
//
//							auto iter = Data->terrain_renderer->terrain_meshs.find({ key_x, key_y });
//							if (iter == Data->terrain_renderer->terrain_meshs.end())
//							{
//								Data->terrain_renderer->create_square_plane({ key_x, key_y });
//								iter = Data->terrain_renderer->terrain_meshs.find({ key_x, key_y });
//							}
//
//							if (dds_file_name.find("height") != string::npos)
//							{
//								DirectX::ScratchImage height_scratchImage;
//								DirectX::LoadFromDDSFile(
//									StringHelper::StringToWide("Save/" + dds_file_name).c_str(),
//									DDS_FLAGS::DDS_FLAGS_NONE,
//									nullptr,
//									height_scratchImage
//								);
//
//								float* height_pixel = (float*)height_scratchImage.GetPixels();
//								for (int index = 0; index < height_scratchImage.GetMetadata().width * height_scratchImage.GetMetadata().height; ++index)
//								{
//									iter->second.Vertex_vector[index].Position.y = height_pixel[index];
//								}
//							}
//							else if (dds_file_name.find("color") != string::npos)
//							{
//								DirectX::ScratchImage color_scratchImage;
//								DirectX::LoadFromDDSFile(
//									StringHelper::StringToWide("Save/" + dds_file_name).c_str(),
//									DDS_FLAGS::DDS_FLAGS_NONE,
//									nullptr,
//									color_scratchImage
//								);
//
//								float* color_pixel = (float*)color_scratchImage.GetPixels();
//
//								for (int index = 0; index < color_scratchImage.GetMetadata().width * color_scratchImage.GetMetadata().height; ++index)
//								{
//									iter->second.Vertex_vector[index].VertexColor.x = color_pixel[index * 4 + 0];
//									iter->second.Vertex_vector[index].VertexColor.y = color_pixel[index * 4 + 1];
//									iter->second.Vertex_vector[index].VertexColor.z = color_pixel[index * 4 + 2];
//									iter->second.Vertex_vector[index].VertexColor.w = color_pixel[index * 4 + 3];
//								}
//							}
//							Data->terrain_renderer->update_normal({ key_x, key_y });
//							Data->terrain_renderer->create_buffer({ key_x, key_y });
//						}
//					}
//
//					vector<string> mfd_file_in_saved = get_files_inDirectory("Save\\", "*.mfd");
//					for (auto& mfd_file_name : mfd_file_in_saved)
//					{
//						string key = mfd_file_name.substr(mfd_file_name.find("_") + 1);
//						int key_x = stoi(key.substr(0, key.find("_") + 1));
//						key = key.substr(key.find("_") + 1);
//						int key_y_index = key.find("_") + 1;
//						string key_y_sub_str = key.substr(0, key_y_index);
//						int key_y = stoi(key_y_sub_str);
//
//						auto iter = Data->terrain_renderer->terrain_meshs.find({ key_x, key_y });
//						if (iter == Data->terrain_renderer->terrain_meshs.end())
//						{
//							Data->terrain_renderer->create_square_plane({ key_x, key_y });
//							iter = Data->terrain_renderer->terrain_meshs.find({ key_x, key_y });
//						}
//
//						StringReader sr("Save/" + mfd_file_name);
//						string line = sr.get_line();
//						while (line != "")
//						{
//							int Type = stoi(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Pos_x = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Pos_y = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Pos_z = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Rot_x = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Rot_y = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Rot_z = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Sca_x = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Sca_y = stof(line.substr(0, line.find(" ") + 1));
//							line = line.substr(line.find(" ") + 1);
//
//							float Sca_z = stof(line);
//							//line = line.substr(line.find(" ") + 1);
//
//							iter->second.Foliages[Type].push_back(
//								TerrainMesh::FoliageData
//								(
//									XMFLOAT3(Pos_x, Pos_y, Pos_z),
//									XMFLOAT3(Rot_x, Rot_y, Rot_z),
//									XMFLOAT3(Sca_x, Sca_y, Sca_z)
//								));
//
//							line = sr.get_line();
//						}
//					}
//				}
//#pragma endregion
//
//				ImGui::SameLine();
//				if (SUCCEEDED(save_hr))
//				{
//					ImGui::Text("Success");
//				}
//				else
//				{
//					ImGui::Text("Failure");
//				}
//
//				break;
//			}
//		}
//		ImGui::EndChild();
//		ImGui::End();
//	}
//
//	/// <summary>
//	/// 
//	/// </summary>
//	if (Data->Tool_main_ImGui->State == ToolMainImGui::TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL)
//	{
//		Window_flags = 0;
//		ImGui::Begin("Map tool material");
//
//		if (ImGui::Button("Wirefarem", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_wireframe))
//			);
//		}
//		if (ImGui::Button("Texture normal view", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_normal))
//			);
//		}
//		if (ImGui::Button("Texture weight 012", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_weightview_012))
//			);
//		}
//		if (ImGui::Button("Texture weight 345", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_weightview_345))
//			);
//		}
//		if (ImGui::Button("Texture weight 678", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_weightview_678))
//			);
//		}
//		if (ImGui::Button("Decal only", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material_decalonly))
//			);
//		}
//		if (ImGui::Button("Result", ImVec2(200, 0)))
//		{
//			Data->terrain_renderer->VS_PS_Shader.set(
//				Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material))
//			);
//		}
//		ImGui::NewLine();
//		// Resource view
//
//		ImGui::End();
//	}
//}
