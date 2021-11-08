#pragma once

#include "SubstancePipe_Renderer.h"

class Animator : public Component
{
public:
	SafePtr<BoneRoot> bone_root;

	struct AnimPlay
	{
		SafePtr<ANIMATION::Clip> clip;
		float weight = 1.f;
		float time = 0.f;
		void draw_detail_view();
	};
	std::list<AnimPlay> anim_play_list;

	virtual void update() override;
	void apply_animation();
	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(Animator);