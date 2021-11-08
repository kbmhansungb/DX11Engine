#pragma once

#include "../_Include.h"

class SubGraphics;
class RenderScope;
class RendererContainer;

class RendererContainer
{
protected:
	std::map<int, std::unordered_set<RenderScope*>> renderer_sorted;
public:
	void draw_all_renderer(SubGraphics* Sub_graphics);
	friend RenderScope;

	const std::map<int, std::unordered_set<RenderScope*>>& get_rednerer_sorted() { return renderer_sorted; }
};

class RenderScope
{
public:
	int Render_order = 0;
	bool VIsiblity = false;

	// RendererContainer�� �־�� �´°� ����� �ѵ� ������ �״�� ��ġ.
	// �����ؾ����� ������ RendererContainer�� �ű���.
	void registe_renderer(RendererContainer* renderer_container);
	void unregiste_renderer(RendererContainer* renderer_container);
	void set_visibility(bool new_visiblity, RendererContainer* renderer_container);
public:
	virtual void set_visibility(bool new_visiblity) = 0;
	virtual void draw(SubGraphics* Sub_graphics) = 0;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) = 0;
};