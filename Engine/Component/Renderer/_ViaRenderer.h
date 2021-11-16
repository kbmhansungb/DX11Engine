#pragma once

#include "../_Include.h"

/// <summary>
/// 렌더링을 결정하는 컴포넌트.
/// 씬에 리스트 형식으로 저장되어 렌더링 됨.
/// 자세한건 기술 문서 참조
/// 21.11.16
/// </summary>
class ViaRenderer : public Component, public RenderScope
{

	// 렌더 하는 함수들 모음
	// 21.11.16
#pragma region Func_ViaRenderer

public:
	/// <summary>
	/// 비지빌리티를 설정하는 메서드
	/// 21.11.16
	/// </summary>
	/// <param name="new_visiblity"></param>
	virtual void set_visibility(bool new_visiblity) override;

	/// <summary>
	/// 파이프라인의 드로우를 호출하는 메서드,
	/// 머테리얼을 이용함
	/// 21.11.16
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void draw(SubGraphics* sub_graphics) override = 0;

	/// <summary>
	/// 머테리얼을 외부에서 설정하고 드로우 하는 메서드
	/// 21.11.16
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override = 0;

#pragma endregion

#pragma region Component

public:
	virtual void draw_detail_view();

#pragma endregion


};