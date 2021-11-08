//#include "_load_resource.h"
//#include "_map_tool_component.h"
//
//void Tool::load_resource()
//{
//	auto Data = ToolData::get_map_tool_data();
//	auto engine = Data->engine;
//
//	// foliages
//	Data->models = get_files_inDirectory("Foliages\\", "*.*");
//	Data->models.erase(Data->models.begin());
//	Data->models.erase(Data->models.begin());
//	for (auto iter = Data->models.begin(); iter != Data->models.end(); ++iter)
//	{
//		auto brush = engine->add_resource<MODEL::Model>(
//			engine->make_name(*iter),
//			make_unique<MODEL::Model>("Foliages/" + *iter)
//			);
//		engine->load_resource(brush.cast<Resource>());
//	}
//	Data->model_names.resize(Data->models.size());
//	for (int index = 0; index < Data->models.size(); ++index)
//	{
//		Data->model_names[index] = Data->models[index].c_str();
//	}
//
//	// brushes
//	Data->brushes = get_files_inDirectory("Brushes\\", "*.*");
//	Data->brushes.erase(Data->brushes.begin());
//	Data->brushes.erase(Data->brushes.begin());
//	for (auto iter = Data->brushes.begin(); iter != Data->brushes.end(); ++iter)
//	{
//		auto brush = engine->add_resource<TEXTURE::Image>(
//			engine->make_name(*iter),
//			make_unique<TEXTURE::Image>(aiTextureType::aiTextureType_DIFFUSE, "Brushes/" + *iter)
//			);
//		engine->load_resource(brush.cast<Resource>());
//	}
//	Data->brush_names.resize(Data->brushes.size());
//	for (int index = 0; index < Data->brushes.size(); ++index)
//	{
//		Data->brush_names[index] = Data->brushes[index].c_str();
//	}
//
//	// textures
//	Data->textures = get_files_inDirectory("Textures\\", "*.*");
//	Data->textures.erase(Data->textures.begin());
//	Data->textures.erase(Data->textures.begin()); // ¿Ö /
//	for (auto iter = Data->textures.begin(); iter != Data->textures.end(); ++iter)
//	{
//		auto texture = engine->add_resource<TEXTURE::Image>(
//			engine->make_name(*iter),
//			make_unique<TEXTURE::Image>(aiTextureType::aiTextureType_DIFFUSE, "Textures/" + *iter)
//			);
//		engine->load_resource(texture.cast<Resource>());
//	}
//	Data->texture_names.resize(Data->textures.size());
//	for (int index = 0; index < Data->textures.size(); ++index)
//	{
//		Data->texture_names[index] = Data->textures[index].c_str();
//	}
//
//	/// terrain material
//	//
//	auto wireframe = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_wireframe),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_wireframe")
//		);
//	engine->load_resource(wireframe.cast<Resource>());
//
//	//
//	auto normal = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_normal),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_view_normal")
//		);
//	normal->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(normal.cast<Resource>());
//
//	//
//	auto weihtview_012 = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_weightview_012),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_weightview_012")
//		);
//	weihtview_012->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(weihtview_012.cast<Resource>());
//
//	//
//	auto weihtview_345 = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_weightview_345),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_weightview_345")
//		);
//	weihtview_345->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(weihtview_345.cast<Resource>());
//
//	//
//	auto weihtview_678 = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_weightview_678),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_weightview_678")
//		);
//	weihtview_678->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(weihtview_678.cast<Resource>());
//
//	//
//	auto decalonly = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material_decalonly),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps_decal")
//		);
//	decalonly->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(decalonly.cast<Resource>());
//
//	//
//	Data->terrain_material = engine->add_resource<TerrainShader>(
//		engine->make_name(MapToolName::terrain_material),
//		make_unique<TerrainShader>(L"Terrain_vs", L"Terrain_ps")
//		);
//	Data->terrain_material->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
//	engine->load_resource(Data->terrain_material.cast<Resource>());
//
//	////
//	// skin material
//}
//
