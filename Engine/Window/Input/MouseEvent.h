#pragma once
struct MousePoint
{
	int x;
	int y;
};

enum class MouseEventType
{
	LPRESS,
	LRELEASE,
	MPRESS,
	MRELEASE,
	RPRESS,
	RRELEASE,
	WHELLUP,
	WHELLDOWN,
	MOVE,
	RAW_MOVE,
	INVALID
};

class MouseEvent
{
public:
	MouseEvent();
	MouseEvent(const MouseEventType type, const int x, const int y);
	bool IsValid() const;
	MouseEventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

private:
	MouseEventType type;
	int x;
	int y;
};
