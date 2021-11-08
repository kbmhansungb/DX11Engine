#include "SubstancePipe_Animator.h"


void Animator::AnimPlay::draw_detail_view()
{
	ImGui::base_field(clip, "Clip");
	ImGui::float_field(&weight, "Weight");
	
	if (clip.is_vaild() == false) return;
	std::string time_name = "Time##" + StringHelper::ptr_to_string(&time);
	ImGui::SliderFloat(
		time_name.c_str(), 
		&time, 
		0.f, 
		floor(clip->duration)
	);
}

void Animator::update()
{
	apply_animation();
}

void Animator::apply_animation()
{
	if (bone_root.is_vaild() == false) return;

	auto bone = bone_root.get();

	// √ ±‚»≠
	for (int index = 0; index < bone->bone_buffer.data.size(); ++index)
	{
		XMMATRIX zero_mat{};
		for (auto& elum : anim_play_list)
		{
			if (elum.clip.is_vaild() == false) continue;
			auto bone_index_map_pair = elum.clip->bone_index_map
				.find(bone_root->bone_structs[index].game_object->name);

			if (bone_index_map_pair == elum.clip->bone_index_map.end()) 
				continue;

			XMMATRIX mat;
			elum.clip->get_lerp(
				elum.time, 
				bone_index_map_pair->second,
				&mat
			);
			zero_mat += mat * elum.weight;
			//zero_mat *= bone->bone_structs[bone_root->skeleton->Bones[index].Parent_bone]
			//	.game_object->world.data.world;
		}

		bone->bone_structs[index].game_object->Adjust()
			.set_local_transform(zero_mat);
	}
}

void Animator::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Animator"))
	{
		ImGui::base_field(bone_root, "Bone root");
		ImGui::NewLine();
		ImGui::list_field_default(anim_play_list, "Anim play list",
			[=](AnimPlay& ap) 
			{
				ap.draw_detail_view();
			});
	}
}