#include "Editor_FreeView.h"

using namespace std;

void EDITOR::editor_add_3d_camera(SafePtr<GameObject> parent, SafePtr<Scene> drawing_scene)
{
	auto& result = *EA3C::get_instance();

	result.camera_object = parent->add_child("Camera object");
	result.camera_object->Adjust().AdjustPosition(0.0f, 0.0f, -0.3f);
	
	result.render_to_deferred = result.camera_object->add_component(new RenderToDeferred(300, 300));
	//result.render_to_deferred->Clear_color = XMFLOAT4(0.22f, 0.22f, 0.22f, 1.0f);
	result.render_to_deferred->active = true;

	result.camera_3D = result.camera_object->add_component<Camera>();
	result.camera_3D->active = true;
	result.graphics = result.camera_object
		->add_component(
			new Graphics(result.camera_3D.cast<Camera>(), result.render_to_deferred.cast<RenderTarget>(), 300, 300)
		);
	result.graphics->active = true;
	
	if (drawing_scene.is_vaild())
		result.graphics->Drawing_scenes.push_back(drawing_scene);
}

std::unique_ptr<EDITOR::Editor_add_3d_camera> ea3c;

EDITOR::Editor_add_3d_camera* EDITOR::Editor_add_3d_camera::get_instance()
{
	if (ea3c.get() == nullptr)
		ea3c = make_unique<EDITOR::Editor_add_3d_camera>();

	return ea3c.get();
}
