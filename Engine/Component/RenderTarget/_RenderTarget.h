#pragma once

#include "../_Include.h"

class RenderTarget : public Component
{

#pragma region ConDestruct

public:
	RenderTarget();
	~RenderTarget();

#pragma endregion

	// render target�� ���õ� �޼���
	// 21.11.16
#pragma region Func_RenderTarget

	virtual void set_render_target(SubGraphics* sub_graphics);
	virtual void ClearRenderTarget(SubGraphics* sub_graphics);
	virtual void Present(SubGraphics* sub_graphics);

#pragma endregion

};