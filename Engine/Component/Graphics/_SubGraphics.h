#pragma once

#include "../_Include.h"

class Camera;
class RenderTarget;

/// <summary>
/// ������ ������ �����ϴ� ������Ʈ
/// 21.11.15
/// </summary>
class SubGraphics : public Component
{

#pragma region ConDestructor

public:
	SubGraphics() {	}
	SubGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	~SubGraphics();

#pragma endregion

	// ���� ������ ���� �ʿ��� ����
	// 21.11.15
#pragma region Val_SubGraphics

public:
	ID3D11Device* Device = NULL;
	ID3D11DeviceContext* Device_context = NULL;

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: viewport�� Camera���� �����ϵ��� �ϸ� �� ������������ ���ؼ� ����غ�����.")
	// viewport�� ����Ÿ�ٿ��� �����ϵ��� �ϸ�
	// update_viewport_size, update_viewport_size�� �Լ���
	// �����Ͽ� ���� �ܼ�ȭ ��ų �� ���� ������ ������
	// 
	// ������ ���ؼ��� ������ �ʿ���
	// 
	// ����Ʈ ������Ʈ�� ���ۿ� Į�� ������Ʈ�� ������ ũ�⸦ �ٸ��� �ϱ� ���ؼ�
	// �ʿ��ϴٰ� ������.
	// 21.11.15
	CD3D11_VIEWPORT viewport;
	SafePtr<Camera> Camera_ptr;
	SafePtr<RenderTarget> Render_target;

	std::list<SafePtr<Scene>> Drawing_scenes;

	/// <summary>
	/// draw �ʿ��� ������ ����, �ʿ��� �� �������̵� �Ͽ� �����.
	/// 21.11.15
	/// </summary>
	UINT Offset = 0;

#pragma endregion

	// SubGraphics�� �޼���
	// 21.11.15
#pragma region Func_SubGraphics

public:
	/// <summary>
	/// SubGraphics�� ������ �ִ� ������ �����ϰ�
	/// ����� �� ����Ʈ�� ���� draw_all_renderer�� ȣ����.
	/// 21.11.15
	/// </summary>
	void only_draw();

	/// <summary>
	/// viewport desc�� ���� ������Ʈ �ϴ� �޼���
	/// 21.11.15
	/// </summary>
	/// <param name="topLeftX"></param>
	/// <param name="topLeftY"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="minDepth"></param>
	/// <param name="maxDepth"></param>
	void update_viewport_size(
		FLOAT topLeftX,
		FLOAT topLeftY,
		FLOAT width,
		FLOAT height,
		FLOAT minDepth = D3D11_MIN_DEPTH,
		FLOAT maxDepth = D3D11_MAX_DEPTH
	);

	/// <summary>
	/// update_viewport_size�� width�� height������ ������Ʈ �ϴ� �޼���
	/// 21.11.15
	/// </summary>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	void update_viewport_size(
		UINT window_width,
		UINT window_height
	);

#pragma endregion

#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void draw_detail_view() override;

#pragma endregion

};