#include "Coroutine.h"

std::queue<Coroutine*> que;

void COROUTINE::step()
{
	Coroutine* coroutine;
	for (size_t roop = 0; roop < que.size(); ++roop)
	{
		coroutine = que.front();
		que.pop();

		coroutine->co_handle.resume();
		if (!coroutine->co_handle.done()) 
			que.push(coroutine);
		else
			coroutine->~Coroutine();
	}
}

void COROUTINE::StartCoroutine(Coroutine&& coroutine)
{
	coroutine.co_handle.resume();
	que.push(&coroutine);
}
