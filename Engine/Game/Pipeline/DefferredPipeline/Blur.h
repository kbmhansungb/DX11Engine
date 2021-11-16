#pragma once

#include "../../../Game/_Exlude.h"
#include "../../../Component/RenderTarget/RenderToTexture.h"
#include "../../../Component/Graphics/_SubGraphics.h"

namespace BTYPE {
	struct CB_BLUR
	{
		float min = 0.1f;
		float middle = 100.f;
		float max = 1000.f;
		UINT sample = 0;

		static UINT T_get_constant_buffer_slot() { return 13; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 최적화 문제")
// 셈플 카운트를 조금만 높여도 심각한 렉이 유발된다.
// 21.11.16

/*
* 블러 포스트 프로세스
*/
class Blur : public DeferredProcess
{
public:
	ConstantBuffer<BTYPE::CB_BLUR> blur;
	Blur();
	virtual void draw_detail_view() override;
	virtual void deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics);
};
REGIST_RESOURCE(Blur);