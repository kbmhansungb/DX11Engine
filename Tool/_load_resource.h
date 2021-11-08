#pragma once

#include "__static.h"

#define MakeMapToolName(string_name) \
static std::string string_name = #string_name;

namespace MapToolName
{
	MakeMapToolName(terrain_material_wireframe);
	MakeMapToolName(terrain_material_normal);
	MakeMapToolName(terrain_material_weightview_012);
	MakeMapToolName(terrain_material_weightview_345);
	MakeMapToolName(terrain_material_weightview_678);
	MakeMapToolName(terrain_material_decalonly);
	MakeMapToolName(terrain_material);

	MakeMapToolName(line_material);
	MakeMapToolName(line_mesh);
}
using namespace MapToolName;

namespace Tool
{
	void load_resource();
}