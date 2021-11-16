#pragma once

#include "../_Include.h"

/// <summary>
/// �������� �����ϴ� ������Ʈ.
/// ���� ����Ʈ �������� ����Ǿ� ������ ��.
/// �ڼ��Ѱ� ��� ���� ����
/// 21.11.16
/// </summary>
class ViaRenderer : public Component, public RenderScope
{

	// ���� �ϴ� �Լ��� ����
	// 21.11.16
#pragma region Func_ViaRenderer

public:
	/// <summary>
	/// ��������Ƽ�� �����ϴ� �޼���
	/// 21.11.16
	/// </summary>
	/// <param name="new_visiblity"></param>
	virtual void set_visibility(bool new_visiblity) override;

	/// <summary>
	/// ������������ ��ο츦 ȣ���ϴ� �޼���,
	/// ���׸����� �̿���
	/// 21.11.16
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void draw(SubGraphics* sub_graphics) override = 0;

	/// <summary>
	/// ���׸����� �ܺο��� �����ϰ� ��ο� �ϴ� �޼���
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