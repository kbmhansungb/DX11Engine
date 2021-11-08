#pragma once
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "../Tool/Delegate.h"
//using namespace Delegate;

class Input
{
	friend class WindowInputContainer;
public:
	KeyboardClass keyboard;
	MouseClass mouse;

	void clear();
};