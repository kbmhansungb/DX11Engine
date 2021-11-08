#pragma once
#include "_load_resource.h"

class ToolMainImGui : public ViaRenderer
{
public:
	bool Tool_setting = false;
	enum class TOOLMAINGUISTATE
	{
		TOOLMAINGUISTATE_NOTTHING, 
		TOOLMAINGUISTATE_MODEL_TOOL, 
		TOOLMAINGUISTATE_MAP_TOOL,
	};
	TOOLMAINGUISTATE State;

	virtual void draw(SubGraphics* Sub_graphics);
	void set_state(TOOLMAINGUISTATE New_state);
	virtual void get_hit_result(RayTracingLib::Ray* ray, RayTracingLib::HitResult* hit_result, float ray_max_distance) {};
};

class ToolSettingImGui : public ViaRenderer
{
public:
	virtual void draw(SubGraphics* Sub_graphics);
	virtual void get_hit_result(RayTracingLib::Ray* ray, RayTracingLib::HitResult* hit_result, float ray_max_distance) {};
};