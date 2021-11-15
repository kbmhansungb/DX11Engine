#pragma once

#include "_SubGraphics.h"
class RenderToSwapChain;

/// <summary>
/// ImGui를 그리기 위한 SubGraphics의 자식 클래스,
/// 이 컴포넌트는 하나만 있어야함.
/// 21.11.15
/// </summary>
class ImGuiGraphics : public SubGraphics
{

#pragma region ConDestruct

public:
	ImGuiGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderToSwapChain> Render_target, float resolutionWidth, float resolutionHeight);
	~ImGuiGraphics();

#pragma endregion

public:
	std::list<SafePtr<Base>> ImGui_draw_list;

#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void update() override;

#pragma endregion

};