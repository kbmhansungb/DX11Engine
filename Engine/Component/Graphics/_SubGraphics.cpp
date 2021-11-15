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

void SubGraphics::awake()
{
	auto ec = EngineContext::get_instance();

	Device = ec->Device.Get();
	Device_context = ec->Device_context.Get();
}

void SubGraphics::sleep()
{
}

void SubGraphics::only_draw()
{
	// SubGraphics에서 렌더링 파이프라인 설정 21.11.15
	Render_target.get()->set_render_target(this);
	Render_target.get()->ClearRenderTarget(this);
	Camera_ptr.get()->set_camera(this);
	Device_context->RSSetViewports(1, &viewport);

	// 드로잉 씬 리스트에 따라 드로잉함. 21.11.15
	for (auto it = Drawing_scenes.begin(); it != Drawing_scenes.end(); ++it)
	{
		if (it->is_vaild())
		{
			it->get()->draw_all_renderer(this);
		}
	}

	// 스왑체인의 경우 버퍼를 스왑함.
	Render_target.get()->Present(this);
}

void SubGraphics::update_viewport_size(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth)
{
	// viewport desc 업데이트
	this->viewport.TopLeftX = topLeftX;
	this->viewport.TopLeftY = topLeftY;
	this->viewport.Width = width;
	this->viewport.Height = height;
	this->viewport.MinDepth = minDepth;
	this->viewport.MaxDepth = maxDepth;
}

void SubGraphics::update_viewport_size(UINT window_width, UINT window_height)
{
	// 유효하지 않은 값일 경우 종료하도록 설정
	if (window_width < 1 && window_height < 1) return;

	// update_viewport_size 전달
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
		// 카메라와 렌더타겟 설정
		ImGui::base_field(Camera_ptr, "Camera");
		ImGui::base_field(Render_target, "Render_target");

		// 드로잉 씬 등록
		static int index;
		index = 0;
		ImGui::list_field_default(Drawing_scenes, "Drawing scenes", 
			[=](SafePtr<Scene>& scene) 
			{
				ImGui::base_field(scene, std::to_string(index));
				++index;
			});
	}
}