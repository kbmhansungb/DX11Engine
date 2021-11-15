#pragma once

#include "_SubGraphics.h"

/// <summary>
/// �� ������Ʈ���� SubGraphics�� �׸����� �۵��ϵ��� �ϴ� ������Ʈ
/// SubGraphics�� ��ӹ���
/// 21.11.15
/// </summary>
class Graphics : public SubGraphics
{

#pragma region ConDestruct

public:
	Graphics();
	Graphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	~Graphics();

#pragma endregion

public:
	/// <summary>
	/// update���� only draw�� ȣ���ϱ��� ���� only draw�� ȣ���ϴ� SubGraphics�� ����
	/// ����. ��Ƽ��Ƽ�� true�� ��� �ι� ������Ʈ �ϴ� ������ ����.
	/// 21.11.15
	/// </summary>
	std::list<SafePtr<SubGraphics>> Sub_graphics;

#pragma region Component

public:
	virtual void update() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(Graphics);