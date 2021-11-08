#pragma once

#include "../../Component/Graphics/Graphics.h"
#include "../../Component/Camera/_Camera.h"
#include "../../Component/Unsorted/SimpleMoveTransform.h"
#include "../../Component/RenderTarget/RenderToTexture.h"

//class Editor_FreeView : public GameObject
//{
//public:
//	SafePtr<RenderToDeferred>			render_to_deferred;
//
//	SafePtr<GameObject>					camera_object;
//	SafePtr<Camera3D>					camera_3D;
//	SafePtr<Graphics>					graphics;
//
//	SafePtr<GameObject>					free_view_object;
//	SafePtr<ViaRenderer>				renderer;
//	
//	SafePtr<Editor_ImageViewObject>		editor_image_view_object;
//
//	Editor_FreeView();
//};

namespace EDITOR
{
	struct Editor_add_3d_camera
	{
		SafePtr<GameObject>					camera_object;
		SafePtr<RenderToDeferred>			render_to_deferred;
		SafePtr<Camera>						camera_3D;
		SafePtr<Graphics>					graphics;

		static Editor_add_3d_camera* get_instance();
	};
	typedef Editor_add_3d_camera EA3C;

	extern void editor_add_3d_camera(SafePtr<GameObject> parent, SafePtr<Scene> drawing_scene);
}