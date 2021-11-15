#include "Graphics.h"

#include "../RenderTarget/RenderToSwapChain.h"
#include "../Camera/_Camera.h"

using namespace std;

Graphics::Graphics() :
	SubGraphics(SafePtr<Camera>(), SafePtr<RenderTarget>(), 300, 300)
{
}

Graphics::Graphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight) :
	SubGraphics(Camera_ptr, Render_target, resolutionWidth, resolutionHeight)
{}

Graphics::~Graphics()
{}

void Graphics::update()
{
	// test code
	{
	//	// Graphics가 작동하는지 확인하기 위한 작업.
	//	MESH::SpriteMesh sprite_mesh;
	//	sprite_mesh.sprite_vertex.emplace_back(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_index.emplace_back(0);
	//	sprite_mesh.sprite_index.emplace_back(1);
	//	sprite_mesh.sprite_index.emplace_back(2);
	//	//sprite_mesh.sprite_index.emplace_back(0);
	//	//sprite_mesh.sprite_index.emplace_back(2);
	//	//sprite_mesh.sprite_index.emplace_back(3);
	//	SHADER::SpriteShader sprite_shader(L"Sprite_vs_shader", L"Sprite_ps_shader");
	//	sprite_shader.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//	// load
	//	sprite_mesh.load_resource(this->owner->this_scene->this_engine.get());
	//	sprite_shader.load_resource(this->owner->this_scene->this_engine.get());
	//	//sprite_shader.set_sprite_shader(this);
	//	//sprite_mesh.set_sprite_mesh(this);
	//	//sprite_mesh.draw_sprite_mesh(this);
	//	// 벡터를 직접 입력
	//	//vector<BTYPE::Vertex_default> vec;
	//	//vec.emplace_back(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	//vec.emplace_back(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	//vec.emplace_back(1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	//	//vec.emplace_back(-1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	//	//VertexBuffer<BTYPE::Vertex_default> vertex_default;
	//	//vertex_default.initialize(this->Device, vec.data(), vec.size());
	//	//UINT offset = 0;
	//	//this->Device_context->IASetVertexBuffers(0, 1, vertex_default.GetAddressOf(), vertex_default.StridePtr(), &offset);
	//	//vector<BTYPE::IB_Default> in;
	//	//this->Device_context->DrawIndexed(1, 0, 0);
	//	SpritePipe pipe;
	//	pipe.sprite_mesh = sprite_mesh.get_safe_ptr().cast<MESH::SpriteMesh>();
	//	pipe.sprite_shader = sprite_shader.get_safe_ptr().cast<SHADER::SpriteShader>();
	//	pipe.draw(this);
	}

	// 이 서브그레픽을 드로잉 하기전 먼저 드로우할 서브그레픽 설정
	// 만일 업데이트 되어 있는 경우
	for (auto it = Sub_graphics.begin(); it != Sub_graphics.end(); ++it)
	{
		if (it->is_vaild())
			it->get()->only_draw();
	}

	// 드로우 함수 호출
	only_draw();
}

void Graphics::draw_detail_view()
{
	SubGraphics::draw_detail_view();

	if (ImGui::CollapsingHeader("Graphics"))
	{
		static int index;
		index = 0;
		ImGui::list_field_default(Sub_graphics, "Sub graphics",
			[=](SafePtr<SubGraphics>& sub_graphics)
			{
				ImGui::base_field(sub_graphics, std::to_string(index));
			});
	}
}
