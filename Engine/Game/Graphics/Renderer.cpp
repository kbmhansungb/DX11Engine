#include "Renderer.h"

using namespace std;

void RenderScope::set_visibility(bool new_visiblity, RendererContainer* renderer_container)
{
	if (this->VIsiblity != new_visiblity)
	{
		this->VIsiblity = new_visiblity;
		if (this->VIsiblity)
		{
			this->registe_renderer(renderer_container);
		}
		else
		{
			this->unregiste_renderer(renderer_container);
		}
	}
}

void RenderScope::registe_renderer(RendererContainer* renderer_container)
{
	auto sorted_by_order_iter
		= renderer_container->renderer_sorted.find(Render_order);
	if (sorted_by_order_iter == renderer_container->renderer_sorted.end())
	{
		// 없을 경우엔 새로 하나 만들어서 넣는다.
		pair<int, unordered_set<RenderScope*>> Pair =
			pair<int, unordered_set<RenderScope*>>(Render_order, { this });
		renderer_container->renderer_sorted.insert(Pair);
	}
	else
	{
		// 있을 경우에는 추가한다.
		sorted_by_order_iter->second.insert(this);
	}
}

void RenderScope::unregiste_renderer(RendererContainer* renderer_container)
{
	auto sorted_by_order_iter
		= renderer_container->renderer_sorted.find(Render_order);
	if (sorted_by_order_iter == renderer_container->renderer_sorted.end())
	{
		// 없을 경우엔 아무 작동도 하지 않는다.
	}
	else
	{
		// 있다면 해당되는 것을 없앤다.
		sorted_by_order_iter->second.erase(this);
	}
}

void RendererContainer::draw_all_renderer(SubGraphics* Sub_graphics)
{
	for (const auto& var : renderer_sorted)
		for (const auto& render : var.second)
		{
			render->draw(Sub_graphics);
		}
}
