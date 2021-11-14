#pragma once

#include "../../../Game/_Exlude.h"
#include "../../../Component/RenderTarget/RenderToTexture.h"
#include "../../../Component/Graphics/_SubGraphics.h"

namespace BTYPE {
	struct CB_LightProcess
	{
		UINT sample_count = 0;

		static UINT T_get_constant_buffer_slot() { return 13; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

class LightProcess : public DeferredProcess
{
public:
	ConstantBuffer<BTYPE::CB_LightProcess> light_process;
	LightProcess();
	virtual void draw_detail_view() override;
	virtual void deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics);
};
REGIST_RESOURCE(LightProcess);