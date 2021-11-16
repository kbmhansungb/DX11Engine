#pragma once

#include "../_Include.h"
#include "../Display/Display.h"

/// <summary>
/// ���ӿ�����Ʈ�� ��ǲ�� ���� �����̵��� �ϴ� ������Ʈ,
/// ������ ���� �����ϴ� ī�޶� ����.
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
	/// ��������Ʈ ���ε�� �޼���
	/// ���콺 �̺�Ʈ�� ������ Owner������Ʈ�� �����̼��� ������
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