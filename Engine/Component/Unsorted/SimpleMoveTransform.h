#pragma once

#include "../_Include.h"
#include "../Display/Display.h"

class SimpleMoveTransform : public Component
{
public:
	SafePtr<Display> display;

	float Move_speed;
	float Rot_speed;

	SimpleMoveTransform();
	SimpleMoveTransform(SafePtr<Display> display);

	//virtual bool awake_condition() override;
	virtual void awake() override;
	virtual void sleep() override;
	virtual void update() override;

	void Bind_mouse_event(MouseEvent Mouse_event);

	virtual void draw_detail_view() override;
	// Component을(를) 통해 상속됨
	virtual std::type_index get_class_type() override;
};
REGIST_COMPONENT(SimpleMoveTransform);