#include "_SubGraphics.h"
#include "../Camera/_Camera.h"
#include "../RenderTarget/_RenderTarget.h"

using namespace std;

SubGraphics::SubGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight) :
	Camera_ptr(Camera_ptr),
	Render_target(Render_target),
	viewport(0.0f, 0.0f, resolutionWidth, resolutionHeight)
{
}

SubGraphics::~SubGraphics() {}

type_index SubGraphics::get_class_type()
{
	return typeid(SubGraphics);
}

//bool SubGraphics::awake_condition()
//{
//	return Camera_ptr.is_vaild() && Render_target.is_vaild();
//}

void SubGraphics::awake()
{
	Device = owner->this_scene->this_engine->Device.Get();
	Device_context = owner->this_scene->this_engine->Device_context.Get();
}

void SubGraphics::sleep()
{
}

void SubGraphics::only_draw()
{
	ID3D11DeviceContext* Device_context = get_owner()->this_scene->this_engine->Device_context.Get();

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 필요없는 코드")
	// Graphics에서 설정되는 것들
	Device_context->RSSetViewports(1, &viewport);

	texel_size.data.texel_size.x = 1.f / (float)this->viewport.Width;
	texel_size.data.texel_size.y = 1.f / (float)this->viewport.Height;
	texel_size.initialize(this->owner->this_scene->this_engine->Device.Get());
	texel_size.apply_changes(this->owner->this_scene->this_engine->Device_context.Get());
	texel_size.set_constant_buffer(Device_context);
	Camera_ptr->set_camera(this);

	for (auto it = Drawing_scenes.begin(); it != Drawing_scenes.end(); ++it)
	{
		if (it->is_vaild())
		{
			it->get()->draw_all_renderer(this);
		}
		else
		{
			// 유효하지 않을때는 어떻게 반응해야 하는가.
		}
	}
}

void SubGraphics::update_viewport_size(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth)
{
	this->viewport.TopLeftX = topLeftX;
	this->viewport.TopLeftY = topLeftY;
	this->viewport.Width = width;
	this->viewport.Height = height;
	this->viewport.MinDepth = minDepth;
	this->viewport.MaxDepth = maxDepth;
}

void SubGraphics::update_viewport_size(UINT window_width, UINT window_height)
{
	if (window_width < 1 && window_height < 1) return;

	update_viewport_size(
		0.0f,
		0.0f,
		static_cast<FLOAT>(window_width),
		static_cast<FLOAT>(window_height)
	);
}

void SubGraphics::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Sub graphics"))
	{
		ImGui::base_field(Camera_ptr, "Camera");
		ImGui::base_field(Render_target, "Render_target");

		static int index;
		index = 0;
		ImGui::list_field_default(Drawing_scenes, "Drawing scenes", 
			[=](SafePtr<Scene>& scene) 
			{
				ImGui::base_field(scene, std::to_string(index));
				++index;
			});
		//std::string drawing_scenes
		//	= "drawing scenes##"
		//	+ StringHelper::ptr_to_string(&Drawing_scenes);
		//if (ImGui::TreeNode(drawing_scenes.c_str()))
		//{
		//	ImGui::NewLine();
		//	std::string drawing_scenes_add_button
		//		= "add scene##"
		//		+ StringHelper::ptr_to_string(&Drawing_scenes);
		//	if (ImGui::RightAngleButton(drawing_scenes_add_button.c_str()))
		//	{
		//		Drawing_scenes.push_back(SafePtr<Scene>());
		//	}
		//	auto delete_target = Drawing_scenes.end();
		//	int index = 0;
		//	for (auto iter = Drawing_scenes.begin(); iter != Drawing_scenes.end(); ++iter)
		//	{
		//		std::string drawing_scenes_add_button
		//			= "X##"
		//			+ StringHelper::ptr_to_string(&*iter);
		//		if (ImGui::Button(drawing_scenes_add_button.c_str()))
		//		{
		//			delete_target = iter;
		//		}
		//		ImGui::SameLine();

		//		std::string scene_name = std::to_string(index);
		//		scene_name += "##" + StringHelper::ptr_to_string(&*iter);
		//		ImGui::base_field(*iter, scene_name.c_str());
		//		++index;
		//	}
		//	if (delete_target != Drawing_scenes.end())
		//		Drawing_scenes.erase(delete_target);
		//	ImGui::TreePop();
		//}
	}
}

//
//void SubGraphics::DrawMeshStruct::draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>* world_constant_buffer)
//{
//	if (this->mesh.is_vaild() == false) return;
//	if (this->shader.is_vaild() == false) return;
//
//	world_constant_buffer->set_constant_buffer(sub_graphics->Device_context);
//
//	shader->set_shader(sub_graphics->Device_context);
//	mesh->set_mesh(sub_graphics->Device_context, Offset);
//	mesh->draw(sub_graphics->Device_context, 0, 0);
//}
