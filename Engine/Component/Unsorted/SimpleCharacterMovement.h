#pragma once

#include "../_Include.h"

#include "../Display/Display.h"
#include "../../Game/Pipeline/SubstancePipe/SubstancePipe_Animator.h"

class SimpleCharacterMovement : public Component
{
public:
	SimpleCharacterMovement();

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

	virtual void awake() override;
	virtual void update() override;

	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(SimpleCharacterMovement);