#pragma once

#include "../_Include.h"

#include "../Display/Display.h"
#include "../../Game/Pipeline/SubstancePipe/SubstancePipe_Animator.h"

/// <summary>
/// 캐릭터를 에디터에서 움직이게 하기위한 테스트 컴포넌트
/// 움직이게 하는 것 외에 별다른 기능은 없음.
/// 21.11.16
/// </summary>
class SimpleCharacterMovement : public Component
{
#pragma region ConDestruct

public:
	SimpleCharacterMovement();

#pragma endregion

#pragma region Val_

	SafePtr<Display> display;
	SafePtr<Animator> animator;

	SafePtr<GameObject> translation_target;
	SafePtr<GameObject> rotation_target;

	float animation_speed;
	float move_speed;
	XMVECTOR axis{};
	XMVECTOR move_vec{};

	SafePtr<ANIMATION::Clip> move_idle;
	SafePtr<ANIMATION::Clip> move_forward;

#pragma endregion

#pragma region Component

	virtual void awake() override;

	/// <summary>
	/// 매 업데이트 마다 애니메이션의 시간을 증가시킴.
	/// </summary>
	virtual void update() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(SimpleCharacterMovement);