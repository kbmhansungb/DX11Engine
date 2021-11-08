#pragma once

#include "_SubGraphics.h"

class Graphics : public SubGraphics
{
public:
	std::list<SafePtr<SubGraphics>> Sub_graphics;

	Graphics();
	Graphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	virtual ~Graphics();
	virtual void update() override;

	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(Graphics);