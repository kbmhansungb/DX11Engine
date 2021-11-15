#pragma once

#include "../_Include.h"

// 레이캐스트를 위해 필요함
#include "../Physics/_Collider.h"

class SubGraphics;
namespace DirectX { struct XMMATRIX; }

/// <summary>
/// 렌더링 파이프라인에서
/// ViewProjection 매트릭스 버퍼를 관리하는컴포넌트
/// 21.11.15
/// </summary>
class Camera : public Component
{
	// 카메라에 사용되는 스트럭트 선언
	// 21.11.15
#pragma region Struct

public:
	/// <summary>
	/// 프로젝션 타입 정의, 
	/// 오소(원근)과 퍼스펙트(평면)이 있음.
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
	
	// VS, PS에서 사용하기 위한 버퍼와 메트릭스 변수
	// 21.11.15
#pragma region Val_BufferMatrix

public:
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection_buffer;
	XMMATRIX view;
	XMMATRIX projection;

#pragma endregion

	// 카메라 메트릭스를 만들기 위한 변수
	// 21.11.15
#pragma region Val_CameraProperty

public:
	PROJECTION_TYPE projection_type;
	
	// 2D, 3D 상관 없이 사용되는 프로퍼티 21.11.15
	float width;
	float height;

	float near_z;
	float far_z;

	// 카메라가 2D 일때 사용되는 프로퍼티 21.11.15
	
	// 카메라가 3D 일때 사용되는 프로퍼티 21.11.15
	float fov_dgree;

#pragma endregion

	// 카메라와 관련된 함수들
	// 21.11.15
#pragma region Func_CameraFunction

public:

	/// <summary>
	/// Val_BufferMatrix를 업데이트함
	/// 21.11.15
	/// </summary>
	virtual void update_view_projection();

	/// <summary>
	/// 디바이스 컨텍스트에 ViewProjection버퍼를 설정함.
	/// 21.11.15
	/// </summary>
	/// <param name="sub_graphics">for device context</param>
	void set_camera(SubGraphics* sub_graphics);

	/// <summary>
	/// 게임오브젝트의 트렌스폼 업데이트 델리게이트에 바인드하여
	/// 게임오브젝트의 트렌스폼이 변경되었을 때
	/// update_view_projection을 호출함
	/// 21.11.15
	/// </summary>
	/// <param name="game_object">바인드된 게임 오브젝트</param>
	void Delegate_response_update_matrix(SafePtr<GameObject> game_object);

	/// <summary>
	/// 카메라 정보와 함수 파라메터를 바탕으로
	/// 카메라 컴포넌트의 시작점으로 반직선을 만듬.
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
	/// 윈도우에 바인드되는 함수로
	/// 윈도우 크기가 변경되었을 때 width와 height를 변경
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