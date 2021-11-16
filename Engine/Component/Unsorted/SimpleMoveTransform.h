#pragma once

#include "../_Include.h"
#include "../Display/Display.h"

/// <summary>
/// 게임오브젝트를 인풋에 의해 움직이도록 하는 컴포넌트,
/// 에디터 씬을 렌더하는 카메라에 사용됨.
/// 21.11.16
/// </summary>
class SimpleMoveTransform : public Component
{
#pragma region ConDestruct

public:
	SimpleMoveTransform();
	SimpleMoveTransform(SafePtr<Display> display);

#pragma endregion

#pragma region Val

public:
	SafePtr<Display> display;

	float Move_speed;
	float Rot_speed;

#pragma endregion

#pragma region Func

	/// <summary>
	/// 델리게이트 바인드용 메서드
	/// 마우스 이벤트를 가져와 Owner오브젝트의 로테이션을 변경함
	/// 21.11.16
	/// </summary>
	void Bind_mouse_event(MouseEvent Mouse_event);

#pragma endregion


#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void update() override;
	virtual void draw_detail_view() override;

#pragma endregion


};
REGIST_COMPONENT(SimpleMoveTransform);