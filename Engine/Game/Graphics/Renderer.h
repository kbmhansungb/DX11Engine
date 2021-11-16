#pragma once

#include "../_Include.h"

// 씬을 단위로 렌더함
// 21.11.16

class SubGraphics;
class RenderScope;
class RendererContainer;

/// <summary>
/// 씬에 상속되어 사용됨
/// 21.11.16
/// </summary>
class RendererContainer
{
protected:
	std::map<int, std::unordered_set<RenderScope*>> renderer_sorted;
public:
	void draw_all_renderer(SubGraphics* Sub_graphics);
	friend RenderScope;

	const std::map<int, std::unordered_set<RenderScope*>>& get_rednerer_sorted() { return renderer_sorted; }
};

/// <summary>
/// ViaRenderer에 상속되어 사용됨
/// 21.11.16
/// </summary>
class RenderScope
{
public:
	int Render_order = 0;
	bool VIsiblity = false;

	// RendererContainer에 있어야 맞는거 같기는 한데 상관없어서 그대로 방치.
	// 정리해야할일 있으면 RendererContainer로 옮기자.
	void registe_renderer(RendererContainer* renderer_container);
	void unregiste_renderer(RendererContainer* renderer_container);
	void set_visibility(bool new_visiblity, RendererContainer* renderer_container);
public:
	virtual void set_visibility(bool new_visiblity) = 0;
	virtual void draw(SubGraphics* Sub_graphics) = 0;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) = 0;
};