#pragma once

#include "../../../Game/_Exlude.h"
#include "../../../Component/RenderTarget/RenderToTexture.h"

class LightProcess : public DeferredProcess
{
public:
	LightProcess();
	virtual void draw_detail_view() override;
};
REGIST_RESOURCE(LightProcess);