#pragma once

#include "_load_resource.h"

class MapToolImGui : public ViaRenderer
{
public:
	enum class MapToolButton
	{
		CREATE_STATE, HEIGHT_BRUSH, TEXTURE_BRUSH, FOLIAGE_BRUSH, BRUSH_SHAPE, SAVE_AND_LOAD,
	};
	virtual ~MapToolImGui() {}
	MapToolButton Brush_state;

	// set size
	float square_size = 100.0f;
	int num_of_splits = 99;
	int Max_uv_x = 10;
	int Max_uv_y = 10;

	// brush
	float brush_size = 10.0f;
	float brush_weight = 1.0f;

	// only height brush
	int now_selected_brush_type = 0;
	float amount = 1.0f;
	float height = 0;

	// only texture brush
	float texture_weight = 0;
	int Texture_index = 0;

	// only foliage brush
	int terrain_foliage_index = 0;
	int max_folliage_count = 10;

	// widget state
	virtual void draw(SubGraphics* Sub_graphics) override;
	virtual void get_hit_result(RayTracingLib::Ray* ray, RayTracingLib::HitResult* hit_result, float ray_max_distance) override {}
};