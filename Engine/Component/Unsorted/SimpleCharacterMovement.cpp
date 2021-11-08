#include "SimpleCharacterMovement.h"

SimpleCharacterMovement::SimpleCharacterMovement() :
	animation_speed(1.f),
	move_speed(1.f)
{
}

void SimpleCharacterMovement::awake()
{
	__super::awake();

	// condition
	if (display.is_vaild() == false || 
		animator.is_vaild() == false ||
		move_idle.is_vaild() == false ||
		move_forward.is_vaild() == false)
	{
		active = false;
		return;
	}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 그냥 만들자..")
	animator->anim_play_list.clear();
	animator->anim_play_list.resize(2);

	auto iter = animator->anim_play_list.begin();
	iter->clip = move_idle;

	++iter;
	iter->clip = move_forward;
	iter->weight = 0.f;
}

void SimpleCharacterMovement::update()
{
	__super::update();
	float deleta_time =  this->owner->this_scene->this_engine->timer.get_deleta_time();
	auto WIC = display->renderWindow.get();

	// Axis를 업데이트 함.
	axis.m128_f32[0] = 0;
	axis.m128_f32[1] = 0;
	axis.m128_f32[2] = 0;
	axis.m128_f32[3] = 0;
	if (WIC->input.keyboard.KeyIsPressed('W'))
	{
		axis.m128_f32[0] += move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('S'))
	{
		axis.m128_f32[0] -= move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('D'))
	{
		axis.m128_f32[2] -= move_speed;
	}
	if (WIC->input.keyboard.KeyIsPressed('A'))
	{
		axis.m128_f32[2] += move_speed;
	}

	axis = XMVector4Normalize(axis);
	move_vec = XMVectorLerp(axis, move_vec, 0.9f);

	if (move_vec.m128_f32[0] > 0.01f || 
		move_vec.m128_f32[0] < 0.01f || 
		move_vec.m128_f32[2] > 0.01f ||
		move_vec.m128_f32[2] < 0.01f)
	{
		// 룩엣 설정
		XMFLOAT3 look_at_pos;
		XMStoreFloat3(&look_at_pos, move_vec);

		XMVECTOR nor_move = XMVector4Normalize(move_vec);
		float dot = XMVector3Dot(nor_move, TRANSFORM_DEFAULT::FORWARD_VECTOR).m128_f32[0];
		float axi = XMVector3Dot(nor_move, TRANSFORM_DEFAULT::RIGHT_VECTOR).m128_f32[0];
		float yaw = DirectX::XMConvertToDegrees(std::acos(dot));
		if (axi < 0.f) yaw = -yaw;

		owner->Adjust().SetRotation(0.f, yaw, 0.f)
			->AdjustPosition(look_at_pos.x * move_speed * deleta_time, 0.f, look_at_pos.z * move_speed * deleta_time);
	}

	float length = std::min(1.f, std::abs(move_vec.m128_f32[0]) + std::abs(move_vec.m128_f32[2]));

	auto iter = animator->anim_play_list.begin();
	iter->weight = 1.f - length;
	iter->time += animation_speed * deleta_time;

	++iter;
	iter->weight = length;
	iter->time += animation_speed * deleta_time;
}

void SimpleCharacterMovement::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Simple character movement"))
	{
		ImGui::base_field(display);
		ImGui::base_field(animator);
		ImGui::NewLine();
		ImGui::base_field(move_idle);
		ImGui::base_field(move_forward);
		ImGui::NewLine();
		ImGui::float_field(&animation_speed, "animation speed");
		ImGui::float_field(&move_speed, "move speed");
	}
}
