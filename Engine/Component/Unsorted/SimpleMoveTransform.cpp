#include "SimpleMoveTransform.h"

using namespace std;

#define DEFAULT_MOVE_SPEED 3.f
#define DEFAULT_ROT_SPEED 30.f

SimpleMoveTransform::SimpleMoveTransform() :
	Move_speed(DEFAULT_MOVE_SPEED),
	Rot_speed(DEFAULT_ROT_SPEED)
{
}

SimpleMoveTransform::SimpleMoveTransform
(
	SafePtr<Display> display
) :
	display(display),
	Move_speed(DEFAULT_MOVE_SPEED),
	Rot_speed(DEFAULT_ROT_SPEED)
{
}

//bool SimpleMoveTransform::awake_condition()
//{
//	return Window_input_container.is_vaild();
//}

void SimpleMoveTransform::awake()
{
	if (display.is_vaild() == false)
	{
		this->active = false;
		return;
	}
	if (display->active == false)
	{
		this->active = false;
		return;
	}

	display->renderWindow->Delegate_mouse_event.AddInvoker(this, &SimpleMoveTransform::Bind_mouse_event);
}

void SimpleMoveTransform::sleep()
{
	if (display.is_vaild() && display->renderWindow.get() != nullptr)
		display->renderWindow->Delegate_mouse_event.RemoveInvoker(this, &SimpleMoveTransform::Bind_mouse_event);
}

void SimpleMoveTransform::Bind_mouse_event(MouseEvent Mouse_event)
{
	if (active == false) return;

	GameObject* Owner = owner.get();
	Engine* engine = Owner->this_scene->this_engine.get();
	auto WIC = display->renderWindow.get();

	if (WIC->input.mouse.IsRightDown())
	{
		if (Mouse_event.GetType() == MouseEventType::RAW_MOVE)
		{
			Owner->Adjust().AdjustRotation((float)Mouse_event.GetPosY() * engine->timer.get_deleta_time() * Rot_speed, (float)Mouse_event.GetPosX() * engine->timer.get_deleta_time() * Rot_speed, 0.0f);
		}
	}
}

void SimpleMoveTransform::update()
{
	GameObject* Owner = owner.get();
	Engine* engine = Owner->this_scene->this_engine.get();
	auto WIC = display->renderWindow.get();

	XMVECTOR result{};
	if (WIC->input.keyboard.KeyIsPressed('W'))
	{
		result += Owner->GetForwardVector() * engine->timer.get_deleta_time() * Move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('S'))
	{
		result += -Owner->GetForwardVector() * engine->timer.get_deleta_time() * Move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('D'))
	{
		result += Owner->GetRightVector() * engine->timer.get_deleta_time() * Move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('A'))
	{
		result += -Owner->GetRightVector() * engine->timer.get_deleta_time() * Move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('E'))
	{
		result += Owner->GetUpVecctor() * engine->timer.get_deleta_time() * Move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('Q'))
	{
		result += -Owner->GetUpVecctor() * engine->timer.get_deleta_time() * Move_speed;
	}
	Owner->Adjust().AdjustPosition(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

void SimpleMoveTransform::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("SimpleMoveTransform"))
	{
		ImGui::InputFloat("Move speed", &this->Move_speed);
		ImGui::InputFloat("Rot speed", &this->Rot_speed);
	}
}

