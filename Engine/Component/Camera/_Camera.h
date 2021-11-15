#pragma once

#include "../_Include.h"

// ����ĳ��Ʈ�� ���� �ʿ���
#include "../Physics/_Collider.h"

class SubGraphics;
namespace DirectX { struct XMMATRIX; }

/// <summary>
/// ������ ���������ο���
/// ViewProjection ��Ʈ���� ���۸� �����ϴ�������Ʈ
/// 21.11.15
/// </summary>
class Camera : public Component
{
	// ī�޶� ���Ǵ� ��Ʈ��Ʈ ����
	// 21.11.15
#pragma region Struct

public:
	/// <summary>
	/// �������� Ÿ�� ����, 
	/// ����(����)�� �۽���Ʈ(���)�� ����.
	/// </summary>
	enum class PROJECTION_TYPE
	{
		ORTHO, PERSPECT
	};

#pragma endregion

#pragma region ConDestructor

public:
	Camera();
	Camera(UINT width, UINT height, float near_z, float far_z);
	Camera(float fov_dgree, UINT width, UINT height, float near_z, float far_z);
	~Camera() {};

#pragma endregion
	
	// VS, PS���� ����ϱ� ���� ���ۿ� ��Ʈ���� ����
	// 21.11.15
#pragma region Val_BufferMatrix

public:
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection_buffer;
	XMMATRIX view;
	XMMATRIX projection;

#pragma endregion

	// ī�޶� ��Ʈ������ ����� ���� ����
	// 21.11.15
#pragma region Val_CameraProperty

public:
	PROJECTION_TYPE projection_type;
	
	// 2D, 3D ��� ���� ���Ǵ� ������Ƽ 21.11.15
	float width;
	float height;

	float near_z;
	float far_z;

	// ī�޶� 2D �϶� ���Ǵ� ������Ƽ 21.11.15
	
	// ī�޶� 3D �϶� ���Ǵ� ������Ƽ 21.11.15
	float fov_dgree;

#pragma endregion

	// ī�޶�� ���õ� �Լ���
	// 21.11.15
#pragma region Func_CameraFunction

public:

	/// <summary>
	/// Val_BufferMatrix�� ������Ʈ��
	/// 21.11.15
	/// </summary>
	virtual void update_view_projection();

	/// <summary>
	/// ����̽� ���ؽ�Ʈ�� ViewProjection���۸� ������.
	/// 21.11.15
	/// </summary>
	/// <param name="sub_graphics">for device context</param>
	void set_camera(SubGraphics* sub_graphics);

	/// <summary>
	/// ���ӿ�����Ʈ�� Ʈ������ ������Ʈ ��������Ʈ�� ���ε��Ͽ�
	/// ���ӿ�����Ʈ�� Ʈ�������� ����Ǿ��� ��
	/// update_view_projection�� ȣ����
	/// 21.11.15
	/// </summary>
	/// <param name="game_object">���ε�� ���� ������Ʈ</param>
	void Delegate_response_update_matrix(SafePtr<GameObject> game_object);

	/// <summary>
	/// ī�޶� ������ �Լ� �Ķ���͸� ��������
	/// ī�޶� ������Ʈ�� ���������� �������� ����.
	/// 21.11.15
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="viewport_w"></param>
	/// <param name="viewport_h"></param>
	/// <param name="mouse_x"></param>
	/// <param name="mouse_y"></param>
	void make_ray_by_screen_pos(
		Line& ray,
		int viewport_w, int viewport_h,
		float mouse_x, float mouse_y
	);

	/// <summary>
	/// �����쿡 ���ε�Ǵ� �Լ���
	/// ������ ũ�Ⱑ ����Ǿ��� �� width�� height�� ����
	/// 21.11.15
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void change_camera_wh(UINT width, UINT height);

#pragma endregion

#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(Camera);