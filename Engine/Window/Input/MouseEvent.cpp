#include "MouseEvent.h"

MouseEvent::MouseEvent():type(MouseEventType::INVALID),x(0),y(0)
{
}

MouseEvent::MouseEvent(const MouseEventType type, const int x, const int y)
	:type(type),x(x),y(y)
{
}

bool MouseEvent::IsValid() const
{
	return this->type != MouseEventType::INVALID;
}

MouseEventType MouseEvent::GetType() const
{
	return this->type;
}

MousePoint MouseEvent::GetPos() const
{	
	return { this->x, this->y };
}

int MouseEvent::GetPosX() const
{
	return this->x;
}

int MouseEvent::GetPosY() const
{
	return this->y;
}
