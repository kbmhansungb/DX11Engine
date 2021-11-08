#pragma once

#include "../_Include.h"

class RenderTarget : public Component
{
public:
	RenderTarget() {}
	virtual ~RenderTarget()
	{
	}

	virtual void set_render_target(SubGraphics* sub_graphics) {}
	virtual void ClearRenderTarget(SubGraphics* sub_graphics) {}
	virtual void Present(SubGraphics* sub_graphics) {}
};