#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : type(KeyboardEventType::INVALID),key(0u)
{
    
}

KeyboardEvent::KeyboardEvent(const KeyboardEventType type, const unsigned char key) : type(type),key(key)
{    
}

bool KeyboardEvent::IsPress() const
{      
    return this->type == KeyboardEventType::PRESS;
}

bool KeyboardEvent::IsRelease() const
{
    return this->type == KeyboardEventType::RELEASE;
}

bool KeyboardEvent::IsValid() const
{
    return this->type != KeyboardEventType::INVALID;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
    return this->key;
}

KeyboardEventType KeyboardEvent::GetEventType()
{
    return this->type;
}
