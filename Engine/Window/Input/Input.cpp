#include "Input.h"

void Input::clear()
{
	// InputÀ» ºñ¿ò.
	while (keyboard.CharBufferIsEmpty() == false)
	{
		unsigned char cha = keyboard.ReadChar();
	}
	while (keyboard.KeyBufferIsEmpty() == false)
	{
		KeyboardEvent ke = keyboard.ReadKey();
	}
	while (mouse.EventBufferIsEmpty() == false)
	{
		MouseEvent me = mouse.ReadEvent();
	}
}
