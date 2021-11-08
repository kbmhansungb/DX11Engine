#pragma once

#include "../_Include.h"
#include "../Physics/_Collider.h"

class SubGraphics;
namespace DirectX { struct XMMATRIX; }

class Camera : public Component
{
public:
	enum class PROJECTION_TYPE
	{
		ORTHO, PERSPECT
	};
	Camera();
	Camera(UINT width, UINT height, float near_z, float far_z);
	Camera(float fov_dgree, UINT width, UINT height, float near_z, float far_z);
	virtual ~Camera() {};

	PROJECTION_TYPE projection_type;
	XMMATRIX view;
	XMMATRIX projection;

	// 2D, 3D 상관 없이 사용되는 프로퍼티
	float width;
	float height;

	float near_z;
	float far_z;

	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection_buffer;

	// 카메라가 2D 일때 사용되는 프로퍼티
	// 카메라가 3D 일때 사용되는 프로퍼티
	float fov_dgree;

	virtual void update_view_projection();
	void set_camera(SubGraphics* sub_graphics);
	virtual void awake() override;
	virtual void sleep() override;

	// Link
	void Delegate_response_update_matrix(SafePtr<GameObject> game_object);
	void make_ray_by_screen_pos(
		Line& ray, 
		int viewport_w, int viewport_h, 
		float mouse_x, float mouse_y
	);
	void change_camera_wh(UINT width, UINT height);

	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(Camera);