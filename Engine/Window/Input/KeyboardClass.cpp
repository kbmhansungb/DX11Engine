#include "KeyboardClass.h"
using namespace std;

KeyboardClass::KeyboardClass()
{
    for (int i = 0; i < 256; ++i)
    {
        this->keyStates[i] = false;
    }
}
// 아니요. 네. 네.
bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
    return this->keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
    return this->keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
    return this->charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
    if (this->keyBuffer.empty())
    {
        return KeyboardEvent();
    }
    else
    {
        KeyboardEvent ke = this->keyBuffer.front();
        this->keyBuffer.pop();
        return ke;
    }       
}

unsigned char KeyboardClass::ReadChar()
{
    if (this->charBuffer.empty())
    {
        return 0u;
    }
    else
    {
        unsigned char e = this->charBuffer.front();
        this->charBuffer.pop();
        return e;
    }    
}
void KeyboardClass::OnKeyPressed(const unsigned char key)
{
    this->keyStates[key] = true;
    this->keyBuffer.push(KeyboardEvent(KeyboardEventType::PRESS, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
    this->keyStates[key] = false;
    this->keyBuffer.push(KeyboardEvent(KeyboardEventType::RELEASE, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
    this->charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
    this->autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
    this->autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
    this->autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
    this->autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
    return this->autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
    return this->autoRepeatChars;
}
