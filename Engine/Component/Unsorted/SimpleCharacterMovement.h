#pragma once

#include "../_Include.h"

#include "../Display/Display.h"
#include "../../Game/Pipeline/SubstancePipe/SubstancePipe_Animator.h"

/// <summary>
/// ĳ���͸� �����Ϳ��� �����̰� �ϱ����� �׽�Ʈ ������Ʈ
/// �����̰� �ϴ� �� �ܿ� ���ٸ� ����� ����.
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
	/// �� ������Ʈ ���� �ִϸ��̼��� �ð��� ������Ŵ.
	/// </summary>
	virtual void update() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(SimpleCharacterMovement);