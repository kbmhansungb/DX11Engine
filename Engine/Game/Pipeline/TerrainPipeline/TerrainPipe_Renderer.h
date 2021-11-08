#pragma once

#include "TerrainPipe_Source.h"

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
	/// <param name="keys"> clear된 후 할당된다. </param>
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
	/// 이거 코드 간결하게 해야하는데,, ???? 어지럽네.
	/// </summary>
	/// <param name="key"></param>
	/// <param name="position"></param>
#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 코드 안정성 검사 안되어 있음.")
#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 코드 최적화 해야 되는 문제 있음.")
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
};
REGIST_COMPONENT(TerrainRenderer);