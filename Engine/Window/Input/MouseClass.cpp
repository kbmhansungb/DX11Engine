#include "MouseClass.h"
using namespace std;

void MouseClass::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEventType::LPRESS, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	MouseEvent me(MouseEventType::LRELEASE, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	this->middleIsDown = true;
	MouseEvent me(MouseEventType::MPRESS, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	this->middleIsDown = true;
	MouseEvent me(MouseEventType::MRELEASE, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	MouseEvent me(MouseEventType::RPRESS, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	MouseEvent me(MouseEventType::RRELEASE, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnWheelUp(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::WHELLUP, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::WHELLDOWN, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEventType::MOVE, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
	return this->leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return this->middleIsDown;
}

bool MouseClass::IsRightDown()
{
	return this->rightIsDown;
}

int MouseClass::GetPosX()
{
	return this->x;
}

int MouseClass::GetPosY()
{
	return this->y;
}

MousePoint MouseClass::GetPos()
{
	return { this->x, this->y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent me = this->eventBuffer.front();
		this->eventBuffer.pop();
		return me;
	}	
}
