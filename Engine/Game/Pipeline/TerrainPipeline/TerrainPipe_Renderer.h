#pragma once

#include "TerrainPipe_Source.h"

namespace BTYPE
{
	struct CB_TERRAIN_MPOS
	{
		XMFLOAT3 m_pos;
		float b_size;

		static UINT T_get_constant_buffer_slot() { return 9; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

class TerrainRenderer : public ViaRenderer
{
public:
	TerrainPipe terrain_pipe;

	virtual void draw(SubGraphics* sub_graphics) override;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;
	void get_hit_result_by_editor_view(Hit& hit);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="keys"> clear�� �� �Ҵ�ȴ�. </param>
	void adjust_height(
		const std::vector<int>& key,
		const XMFLOAT3& position
	);
	void fit_height(
		const std::vector<int>& key,
		const XMFLOAT3& position,
		float fit_height
	);
	/// <summary>
	/// �̰� �ڵ� �����ϰ� �ؾ��ϴµ�,, ???? ��������.
	/// </summary>
	/// <param name="key"></param>
	/// <param name="position"></param>
#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �ڵ� ������ �˻� �ȵǾ� ����.")
#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �ڵ� ����ȭ �ؾ� �Ǵ� ���� ����.")
	void paint_texture(
		const std::vector<int>& key,
		const XMFLOAT3& position
	);
	void make_foliage(
		const XMFLOAT3& position,
		int foliage_index,
		float brush_size,
		int foliage_size
	);
	void set_brush_shape(XMFLOAT3& pos, float b_size, bool is_true = true);
};
REGIST_COMPONENT(TerrainRenderer);