#pragma once

enum class KeyboardEventType
{
	PRESS,
	RELEASE,
	INVALID
};

class KeyboardEvent
{
public:
	KeyboardEvent();
	KeyboardEvent(const KeyboardEventType type, const unsigned char key);
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;
	KeyboardEventType GetEventType();

private:
	KeyboardEventType type;
	unsigned char key;	
};

