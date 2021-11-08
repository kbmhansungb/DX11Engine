#include "_Light.h"
#include "../Graphics/_SubGraphics.h"
#include "../RenderTarget/RenderToTexture.h"
//
// 라이트를 구현하기 위한 계획을 세워보자.
//

void Light::awake()
{
	HRESULT hr;
	auto con = EngineContext::get_instance();

	light.initialize(con->Device.Get());
	light.apply_changes(con->Device_context.Get());

	rtt_depth.load_resource(this->owner->this_scene->this_engine.get());
	rtt_LightColor.load_resource(this->owner->this_scene->this_engine.get());
	create_depth_stencil();
	light_pos.initialize(con->Device.Get());
	light.initialize(con->Device.Get());
	view_projection.initialize(con->Device.Get());
}

void Light::sleep()
{
}

void Light::update_stencil(SubGraphics* sub_graphics)
{
	if (this->active == false) return;
	// viewport 설정
	CD3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = this->light_pos.data.resource;
	viewport.Height = this->light_pos.data.resource;
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	sub_graphics->Device_context->RSSetViewports(1, &viewport);

	// 라이팅 스텐실을 렌더하도록 만든다.
	ID3D11RenderTargetView* ar[] = {
		rtt_depth.render_target_view.Get(),
		rtt_LightColor.render_target_view.Get()
	};
	sub_graphics->Device_context->OMSetRenderTargets(
		2,
		ar,
		depth_stencil_view.Get()
	);

	FLOAT CColor[4] = { 0.f, 0.f, 0.f, 1.f };
	sub_graphics->Device_context->ClearRenderTargetView(
		rtt_depth.render_target_view.Get(),
		CColor
	);
	sub_graphics->Device_context->ClearRenderTargetView(
		rtt_LightColor.render_target_view.Get(),
		CColor
	);
	sub_graphics->Device_context->ClearDepthStencilView(
		depth_stencil_view.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	default_depth_light_shader()->set_mesh_shader(sub_graphics);

	// view_projection을 만들고 업데이트 하자.

	// view 설정
	auto owner = this->owner;
	DirectX::XMVECTOR Position_vector = owner->GetPosition();
	DirectX::XMVECTOR Forward_vector = owner->GetForwardVector();
	DirectX::XMVECTOR Up_vector = owner->GetUpVecctor();
	DirectX::XMVECTOR Look_at_point = DirectX::XMVectorAdd(Position_vector, Forward_vector);
	XMMATRIX view = XMMatrixLookAtLH(Position_vector, Look_at_point, Up_vector);

	float width = 100.f;
	float height = 100.f;
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	float fov_dgree = 90.f;
	float fovRandians = (fov_dgree / 360.f) * XM_2PI;
	float near_z = 0.1f;
	float far_z = 1000.f;
	XMMATRIX projection
		= XMMatrixPerspectiveFovLH(fovRandians, aspectRatio, near_z, far_z);

	view_projection.data.Viewprojection = view * projection;

	view_projection.apply_changes(sub_graphics->Device_context);
	view_projection.set_constant_buffer(sub_graphics->Device_context);
	
	light_pos.data.light_pos.x = owner->pos.m128_f32[0];
	light_pos.data.light_pos.y = owner->pos.m128_f32[1];
	light_pos.data.light_pos.z = owner->pos.m128_f32[2];
	light_pos.data.dir = owner->vecForward;
	light_pos.apply_changes(sub_graphics->Device_context);
	light_pos.set_constant_buffer(sub_graphics->Device_context);
	// 라이팅 연산을 위한 정보들이 먼저 정의 되어야 한다.
	light.apply_changes(sub_graphics->Device_context);
	light.set_constant_buffer(sub_graphics->Device_context);

	// ps만 설정하고 vs는 메쉬에 따라 결정되도록 하자.
	for (auto& scene : lighting_scenes)
	{
		if (scene.is_vaild() == false) continue;

		auto& sorted = scene->get_rednerer_sorted();

		for (auto& pair : sorted)
			for (auto& elum : pair.second)
			{
				elum->draw_mesh_only(sub_graphics);
			}
	}
}

//static SHADER::PixelShader ps;
//static void smart_load(SubGraphics* sub_graphics)
//{
//	if (ps.GetShader() == nullptr)
//	{
//		ps.initialize(sub_graphics->Device, L"ps_Lighting");
//	}
//}
void Light::lighting(SubGraphics* sub_graphics)
{
	if (this->active == false) return;

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 테스트릴 위하여 여기서 선언해서 작동하는지 확인한다.")
	SafePtr<MESH::SpriteMesh> mesh = MESH::default_sprite_mesh(this->owner->this_scene->this_engine.get());
	SpritePipe::set_sprite_vs(sub_graphics);

	//smart_load(sub_graphics);
	//sub_graphics->Device_context->PSSetShader(
	//	ps.GetShader(), 0, 0
	//);
	default_lighting_shader()->set_mesh_shader(sub_graphics);

	light_pos.set_constant_buffer(sub_graphics->Device_context);
	view_projection.set_constant_buffer(sub_graphics->Device_context, 12);

	owner->world.set_constant_buffer(sub_graphics->Device_context);
	view_projection.set_constant_buffer(sub_graphics->Device_context, 13);

	ID3D11ShaderResourceView* srv[] = {
		this->rtt_depth.GetTextureResourceView(),
		this->rtt_LightColor.GetTextureResourceView()
	};
	sub_graphics->Device_context->PSSetShaderResources(
		0, 2, srv
	);
	ID3D11SamplerState* ss[] = {
		this->rtt_depth.sampler_state.Get(),
		this->rtt_LightColor.sampler_state.Get()
	};
	sub_graphics->Device_context->PSSetSamplers(
		0, 2, ss
	);

	mesh->set_sprite_mesh(sub_graphics);
	mesh->draw_sprite_mesh(sub_graphics);
}

SafePtr<SHADER::MeshShader> Light::default_depth_light_shader()
{
	Engine* engine = this->owner->this_scene->this_engine.get();

	Name default_light_shader = Name(&engine->name_container, "Default_light_depth");
	SafePtr<SHADER::MeshShader> sprite_shader
		= engine->resources.get_resource(default_light_shader).cast<SHADER::MeshShader>();
	if (sprite_shader.is_vaild() == false)
	{
		std::unique_ptr<SHADER::MeshShader> new_light_shader
			= std::make_unique<SHADER::MeshShader>();
		new_light_shader->raw_ms.pixel_shader_path = L"ps_LightDepth";
		sprite_shader = engine->resources
			.add_resource(default_light_shader, new_light_shader);
		sprite_shader->load_resource(engine);
	}
	return sprite_shader;
}

SafePtr<SHADER::MeshShader> Light::default_lighting_shader()
{
	Engine* engine = this->owner->this_scene->this_engine.get();

	Name default_light_shader = Name(&engine->name_container, "Default_lighting");
	SafePtr<SHADER::MeshShader> sprite_shader
		= engine->resources.get_resource(default_light_shader).cast<SHADER::MeshShader>();
	if (sprite_shader.is_vaild() == false)
	{
		std::unique_ptr<SHADER::MeshShader> new_light_shader
			= std::make_unique<SHADER::MeshShader>();
		//new_light_shader->raw_ms.rasterizer_desc.CullMode = D3D11_CULL_BACK;
		new_light_shader->raw_ms.pixel_shader_path = L"ps_Lighting";
		new_light_shader->raw_ms.depth_stencil_desc.DepthEnable = false;
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 이렇게 해줘야 비로소 블렌드가 된다.. 이해하기 어렵다.")
		new_light_shader->raw_ms.blend_describe.IndependentBlendEnable = true;
		sprite_shader = engine->resources
			.add_resource(default_light_shader, new_light_shader);
		sprite_shader->load_resource(engine);
	}
	return sprite_shader;
}


void Light::draw_detail_view()
{
	Component::draw_detail_view();

	auto con = EngineContext::get_instance();
	if (ImGui::CollapsingHeader("Light"))
	{
		Lighting::draw_lighting_detail_view(this->owner->this_scene.get());
		ImGui::NewLine();
		std::string color_name = "Light color##" + StringHelper::ptr_to_string(&light.data.light_color); 
		if (ImGui::ColorEdit3(color_name.c_str(), &light.data.light_color.x))
		{
			light.apply_changes(con->Device_context.Get());
		}
		if (ImGui::float_field(&light.data.light_strength, "Light strength"))
		{
			light.apply_changes(con->Device_context.Get());
		}

		ImGui::NewLine();
		ImGui::list_field_default(lighting_scenes, "Lighting scene",
			[=](SafePtr<Scene>& scene)
			{
				ImGui::base_field(scene);
			});
		if (this->active)
		{
			if (ImGui::float_field(&light_pos.data.resource, "Resource"))
			{
				rtt_depth.resize_texture(
					light_pos.data.resource, light_pos.data.resource,
					this->owner->this_scene->this_engine.get()
				);

				rtt_LightColor.resize_texture(
					light_pos.data.resource, light_pos.data.resource,
					this->owner->this_scene->this_engine.get()
				);

				this->light_pos.apply_changes(con->Device_context.Get());

				create_depth_stencil();
			}

			const char* text[] =
			{
				"Depth",
				"LightColor"
			};
			static int selected = 0;
			if (ImGui::BeginCombo("ImageSelect", text[selected]))
			{
				bool is_selected = false;
				ImGui::Selectable(text[0], &is_selected);
				if(is_selected)
				{
					selected = 0;
				}

				is_selected = false;
				ImGui::Selectable(text[1], &is_selected);
				if (is_selected)
				{
					selected = 1;
				}

				ImGui::EndCombo();
			}
			switch (selected)
			{
			case 0:
				ImGui::Image(rtt_depth.GetTextureResourceView(), 
					ImVec2(light_pos.data.resource, light_pos.data.resource));
				break;
			case 1:
				ImGui::Image(rtt_LightColor.GetTextureResourceView(), 
					ImVec2(light_pos.data.resource, light_pos.data.resource));
				break;
			}
		}
	}
}

void Light::create_depth_stencil()
{
	auto con = EngineContext::get_instance();
	depth_stencil_buf.Reset();
	depth_stencil_view.Reset();
	depth_srv.Reset();

	D3D11_TEXTURE2D_DESC depth_stencil_buf_desc
		= CD3D11_TEXTURE2D_DESC(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, 
			light_pos.data.resource,
			light_pos.data.resource);
	depth_stencil_buf_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	con->Device->CreateTexture2D(
		&depth_stencil_buf_desc,
		0,
		depth_stencil_buf.GetAddressOf()
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc
		= CD3D11_DEPTH_STENCIL_VIEW_DESC(
			depth_stencil_buf.Get(),
			D3D11_DSV_DIMENSION_TEXTURE2D
		);
	con->Device->CreateDepthStencilView(
		depth_stencil_buf.Get(),
		&dsvdesc,
		depth_stencil_view.GetAddressOf()
	);

	// 뎁스 스텐실의 쉐이더 리소스 뷰는 어떻게 만드는지 모르겠돠아.
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 모르는 것..")
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc
	//	= CD3D11_SHADER_RESOURCE_VIEW_DESC(
	//		D3D11_SRV_DIMENSION_TEXTURE2D,
	//		depth_stencil_buf_desc.Format
	//	);
	//srvdesc.Texture2D.MipLevels = 1;
	//con->Device->CreateShaderResourceView(
	//	depth_stencil_buf.Get(),
	//	&srvdesc,
	//	depth_srv.GetAddressOf()
	//);
}
