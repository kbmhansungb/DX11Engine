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
		// ���� ��쿣 ���� �ϳ� ���� �ִ´�.
		pair<int, unordered_set<RenderScope*>> Pair =
			pair<int, unordered_set<RenderScope*>>(Render_order, { this });
		renderer_container->renderer_sorted.insert(Pair);
	}
	else
	{
		// ���� ��쿡�� �߰��Ѵ�.
		sorted_by_order_iter->second.insert(this);
	}
}

void RenderScope::unregiste_renderer(RendererContainer* renderer_container)
{
	auto sorted_by_order_iter
		= renderer_container->renderer_sorted.find(Render_order);
	if (sorted_by_order_iter == renderer_container->renderer_sorted.end())
	{
		// ���� ��쿣 �ƹ� �۵��� ���� �ʴ´�.
	}
	else
	{
		// �ִٸ� �ش�Ǵ� ���� ���ش�.
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
