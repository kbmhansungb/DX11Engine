#include "MainScene.h"

EDITOR::Construct_main_scene cms;
EDITOR::Construct_main_scene* EDITOR::Construct_main_scene::get_instance()
{
	return &cms;
}

void EDITOR::construct_main_scene(Engine* engine)
{
	cms.main_scene = engine->root_scene.add_child("Main scene");
}
