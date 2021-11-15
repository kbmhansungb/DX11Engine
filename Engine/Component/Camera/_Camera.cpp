
#include "_Camera.h"

#include "../Graphics/_SubGraphics.h"

Camera::Camera() :
	projection_type(PROJECTION_TYPE::PERSPECT),
	view(XMMatrixIdentity()),
	projection(XMMatrixIdentity()),
	width(300.f), height(300.f),
	near_z(0.1f), far_z(1000.f),
	fov_dgree(120.f)
{
}

Camera::Camera(UINT width, UINT height, float near_z, float far_z) :
	projection_type(PROJECTION_TYPE::ORTHO),
	view(XMMatrixIdentity()),
	projection(XMMatrixIdentity()),
	width((float)width), height((float)height),
	near_z(near_z), far_z(far_z),
	fov_dgree(120.f)
{
}

Camera::Camera(float fov_dgree, UINT width, UINT height, float near_z, float far_z) :
	projection_type(PROJECTION_TYPE::PERSPECT),
	view(XMMatrixIdentity()),
	projection(XMMatrixIdentity()),
	width((float)width), height((float)height),
	near_z(near_z), far_z(far_z),
	fov_dgree(fov_dgree)
{
}

void Camera::update_view_projection()
{
	auto owner = this->owner;
	// 1. view 설정 21.11.15
	XMVECTOR s, r, t;
	XMMatrixDecompose(&s, &r, &t, owner->world.data.world);
	DirectX::XMVECTOR Position_vector = t;
	DirectX::XMVECTOR Forward_vector = owner->GetForwardVector();
	DirectX::XMVECTOR Up_vector = owner->GetUpVecctor();
	DirectX::XMVECTOR Look_at_point = DirectX::XMVectorAdd(Position_vector, Forward_vector);
	view = XMMatrixLookAtLH(Position_vector, Look_at_point, Up_vector);
	
	// 2. projection 설정 21.11.15
	switch (this->projection_type)
	{
	case PROJECTION_TYPE::ORTHO:
	{
		projection
			= XMMatrixOrthographicOffCenterLH(
				-width / 2.f, width / 2.f,
				-height / 2.f, height / 2.f,
				near_z, far_z
			);
	}
		break;
	case PROJECTION_TYPE::PERSPECT:
	{

		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		float fovRandians = (fov_dgree / 360.f) * XM_2PI;
		this->projection = XMMatrixPerspectiveFovLH(fovRandians, aspectRatio, near_z, far_z);
	}
		break;
	}

	// 3. view projection 업데이트 21.11.15
	view_projection_buffer.data
		.Viewprojection = view * projection;
	view_projection_buffer
		.apply_changes(EngineContext::get_instance()->Device_context.Get());
}

void Camera::set_camera(SubGraphics* sub_graphics)
{
	view_projection_buffer.set_constant_buffer(sub_graphics->Device_context);
}

void Camera::awake()
{
	view_projection_buffer.initialize(this->owner->this_scene->this_engine->Device.Get());
	update_view_projection();

	owner->Delegate_update_matrix.AddInvoker(this, &Camera::Delegate_response_update_matrix);
}

void Camera::sleep()
{
	owner->Delegate_update_matrix.RemoveInvoker(this, &Camera::Delegate_response_update_matrix);
}

void Camera::Delegate_response_update_matrix(SafePtr<GameObject> game_object)
{
	update_view_projection();
}

void Camera::make_ray_by_screen_pos(Line& ray, int viewport_w, int viewport_h, float mouse_x, float mouse_y)
{
	// ray를 구하기 위한 메트릭스 선언 21.11.15
	XMMATRIX view_inverse = XMMatrixInverse(nullptr, view);
	XMMATRIX projection_inverse = XMMatrixInverse(nullptr, projection);
	XMMATRIX to_local_mat = XMMatrixMultiply(projection_inverse, view_inverse);

	// 반직선 값 설정 21.11.15
	ray.type = Line::TYPE::RAY;
	ray.origin = XMVectorSet(
		(mouse_x * 2.0f / static_cast<float>(viewport_w)) - 1.0f,
		(mouse_y * -2.0f / static_cast<float>(viewport_h)) + 1.0f,
		0.0f,
		1.0f
	);
	ray.vec = XMVectorSet(
		(mouse_x * 2.0f / static_cast<float>(viewport_w)) - 1.0f,
		(mouse_y * -2.0f / static_cast<float>(viewport_h)) + 1.0f,
		1.0f,
		1.0f
	);

	// 반직선 반환 21.11.15
	ray.origin = XMVector3TransformCoord(ray.origin, to_local_mat);
	ray.vec = XMVector3TransformCoord(ray.vec, to_local_mat);
	ray.vec = XMVector3Normalize(ray.vec - ray.origin);
}

void Camera::change_camera_wh(UINT width, UINT height)
{
	// 1. 유효성 검사 21.11.15
	if (width < 1 && height < 1) return;

	// 2. width와 height값 변경 21.11.15
	this->width = (float)width;
	this->height = (float)height;

	// 3. 메트릭스 업데이트 21.11.15
	this->update_view_projection();
}

#include "../Physics/_Collider.h"
void Camera::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Camera"))
	{
		// PROJECTION타입 설정 21.11.15
		std::string projection_type_combo_name
			= "Type##"
			+ StringHelper::ptr_to_string(&projection_type);
		const char* PROJECTION_TYPE_STR[] = {
			"ORTHO", "PERSPECT"
		};
		if (ImGui::BeginCombo(
			projection_type_combo_name.c_str(),
			PROJECTION_TYPE_STR[(int)projection_type]))
		{
			bool is_selected = false;
			for (int index = 0; index < IM_ARRAYSIZE(PROJECTION_TYPE_STR); index++)
			{
				if (ImGui::Selectable(PROJECTION_TYPE_STR[index], &is_selected))
				//if (is_selected)
				{
					projection_type = (PROJECTION_TYPE)index;
					this->update_view_projection();
				}
			}
			ImGui::EndCombo();
		}

		// 프로젝션외 다른 Val_CameraProperty 설정 21.11.15
		if (ImGui::float_field(&width, "width"))
			this->update_view_projection();
		if (ImGui::float_field(&height, "height"))
			this->update_view_projection();
		if (ImGui::float_field(&near_z, "near_z"))
			this->update_view_projection();
		if (ImGui::float_field(&far_z, "far_z"))
			this->update_view_projection();

		// 2D와 3D에 따라 달라지는 Val_CameraProperty 설정 21.11.15
		switch (this->projection_type)
		{
		case PROJECTION_TYPE::ORTHO:
			break;
		case PROJECTION_TYPE::PERSPECT:
		{
			if (ImGui::float_field(&fov_dgree, "Fov dgree"))
				this->update_view_projection();
		}
			break;
		}

		//// ray가 제대로 만들어 지는지 테스트를 위한 코드 21.11.15
		//Line line;
		//this->make_ray_by_screen_pos(line, 2, 2, 1, 1);
		//ImGui::XMVECTOR_field(line.origin, "origin");
		//ImGui::XMVECTOR_field(line.vec, "vec");
	}
}
