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
	// SubGraphics���� ������ ���������� ���� 21.11.15
	Render_target.get()->set_render_target(this);
	Render_target.get()->ClearRenderTarget(this);
	Camera_ptr.get()->set_camera(this);
	Device_context->RSSetViewports(1, &viewport);

	// ����� �� ����Ʈ�� ���� �������. 21.11.15
	for (auto it = Drawing_scenes.begin(); it != Drawing_scenes.end(); ++it)
	{
		if (it->is_vaild())
		{
			it->get()->draw_all_renderer(this);
		}
	}

	// ����ü���� ��� ���۸� ������.
	Render_target.get()->Present(this);
}

void SubGraphics::update_viewport_size(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth)
{
	// viewport desc ������Ʈ
	this->viewport.TopLeftX = topLeftX;
	this->viewport.TopLeftY = topLeftY;
	this->viewport.Width = width;
	this->viewport.Height = height;
	this->viewport.MinDepth = minDepth;
	this->viewport.MaxDepth = maxDepth;
}

void SubGraphics::update_viewport_size(UINT window_width, UINT window_height)
{
	// ��ȿ���� ���� ���� ��� �����ϵ��� ����
	if (window_width < 1 && window_height < 1) return;

	// update_viewport_size ����
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
		// ī�޶�� ����Ÿ�� ����
		ImGui::base_field(Camera_ptr, "Camera");
		ImGui::base_field(Render_target, "Render_target");

		// ����� �� ���
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