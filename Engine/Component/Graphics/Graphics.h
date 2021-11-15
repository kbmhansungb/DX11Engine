#pragma once

#include "_SubGraphics.h"

/// <summary>
/// 매 업데이트마다 SubGraphics를 그리도록 작동하도록 하는 컴포넌트
/// SubGraphics를 상속받음
/// 21.11.15
/// </summary>
class Graphics : public SubGraphics
{

#pragma region ConDestruct

public:
	Graphics();
	Graphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	~Graphics();

#pragma endregion

public:
	/// <summary>
	/// update에서 only draw를 호출하기전 먼저 only draw를 호출하는 SubGraphics의 참조
	/// 주의. 엑티비티가 true인 경우 두번 업데이트 하는 문제가 있음.
	/// 21.11.15
	/// </summary>
	std::list<SafePtr<SubGraphics>> Sub_graphics;

#pragma region Component

public:
	virtual void update() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(Graphics);