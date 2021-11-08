#pragma once
#include <experimental/coroutine>
//#include <coroutine>
#include <queue>

class Coroutine
{
public:
	struct promise_type;
	using coHandle = std::experimental::coroutine_handle<promise_type>;

	struct promise_type
	{
		auto get_return_object() noexcept { return coHandle::from_promise(*this); }
		auto initial_suspend() noexcept { return std::experimental::suspend_always(); }
		void return_void() { return; }
		auto final_suspend() noexcept { return std::experimental::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
	};
	Coroutine(coHandle co_handle) : co_handle(co_handle) {}
	Coroutine(Coroutine&) = delete;
	Coroutine(Coroutine&&) = delete;
	~Coroutine()
	{
		if (co_handle.done())
		{
			co_handle.destroy();
		}
	}
	coHandle co_handle;
};

//Coroutine sample_routine()
//{
//	co_await std::experimental::suspend_always{};
//	co_return;
//}

namespace COROUTINE
{
	extern void step();
	extern void StartCoroutine(Coroutine&& coroutine);
};