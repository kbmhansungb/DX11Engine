#pragma once

#include "../../Component/_Exclude.h"

namespace EDITOR
{
	struct Construct_main_scene
	{
		SafePtr<Scene> main_scene;
		static Construct_main_scene* get_instance();
	};
	typedef Construct_main_scene CMS;
	void construct_main_scene(Engine* engine);
}