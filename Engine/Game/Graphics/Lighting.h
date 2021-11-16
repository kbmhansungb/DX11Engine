#pragma once

#include "../_Include.h"

// 씬을 단위로 라이팅 함.
// 21.11.16

class SubGraphics;
class Lighting;

/// <summary>
/// 씬에 상속되어 사용됨
/// 21.11.16
/// </summary>
class LightingContainer
{
protected:
	std::unordered_set<Lighting*> lighting_uor_set;
public:
	const std::unordered_set<Lighting*>& get_lighting_uor_set() { return lighting_uor_set; }
	void lighting_all(SubGraphics* sub_graphics);
	friend Lighting;
};

/// <summary>
/// 라이팅 컴포넌트에 상속되어 사용됨.
/// 21.11.16
/// </summary>
class Lighting
{
public:
	bool affect_world = false;
	void regist_lighting(LightingContainer* lcon);
	void unregist_lighting(LightingContainer* lcon);
	void put_affect_world(bool affect_world, LightingContainer* lcon);
public:
	virtual void update_stencil(SubGraphics* sub_grahpics) = 0;
	virtual void lighting(SubGraphics* sub_graphics) = 0;
	void draw_lighting_detail_view(LightingContainer* lcon);
};