#pragma once

class SubGraphics;
class Interface_ResponseGraphics
{
public:
	virtual void draw(SubGraphics* Sub_graphics) = 0;
};