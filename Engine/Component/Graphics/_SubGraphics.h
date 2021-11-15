#pragma once

#include "../_Include.h"

class Camera;
class RenderTarget;

class SubGraphics : public Component
{
public:
	SafePtr<Camera> Camera_ptr;
	SafePtr<RenderTarget> Render_target;
	CD3D11_VIEWPORT viewport;

	std::list<SafePtr<Scene>> Drawing_scenes;

	// 
	UINT Offset = 0; // 항상 0 이지 않을까 싶음.
	ID3D11Device* Device = NULL;
	ID3D11DeviceContext* Device_context = NULL;


	SubGraphics() {	}
	SubGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	~SubGraphics();

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