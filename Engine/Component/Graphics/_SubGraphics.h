#pragma once

#include "../_Include.h"

class Camera;
class RenderTarget;

namespace BTYPE {
	struct CB_TEXEL_SIZE
	{
		XMFLOAT2 texel_size = { 1.f, 1.f };

		static UINT T_get_constant_buffer_slot() { return 13; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

class SubGraphics : public Component
{
public:
	SafePtr<Camera> Camera_ptr;
	SafePtr<RenderTarget> Render_target;

	ConstantBuffer<BTYPE::CB_TEXEL_SIZE> texel_size;
	CD3D11_VIEWPORT viewport;

	std::list<SafePtr<Scene>> Drawing_scenes;

	// 
	UINT Offset = 0; // �׻� 0 ���� ������ ����.
	ID3D11Device* Device = NULL;
	ID3D11DeviceContext* Device_context = NULL;

	// �ϴ� �̰Ŵ� ȣ���ϸ� �ȵȴ�..
	// base field�� ���ؼ� ����..
	SubGraphics() {	}
	SubGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	virtual ~SubGraphics();
	// Component��(��) ���� ��ӵ�
	virtual std::type_index get_class_type() override;

	//virtual bool awake_condition() override;
	virtual void awake() override;
	virtual void sleep() override;
	void only_draw();

	void update_viewport_size(
		FLOAT topLeftX,
		FLOAT topLeftY,
		FLOAT width,
		FLOAT height,
		FLOAT minDepth = D3D11_MIN_DEPTH,
		FLOAT maxDepth = D3D11_MAX_DEPTH
	);

	void update_viewport_size(
		UINT window_width,
		UINT window_height
	);

	virtual void draw_detail_view() override;
};