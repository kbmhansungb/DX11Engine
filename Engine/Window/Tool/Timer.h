#pragma once

#include <chrono>
//using namespace std;
//using namespace chrono;

template <typename friend_class>
class Timer final
{
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
#else
	time_point<system_clock> start;
	time_point<system_clock> stop;
#endif
	bool isRunning = false;
	float deleta_time = 0.0f;
	Timer()
	{
		this->stop = this->start = std::chrono::high_resolution_clock::now();
	}
	//
	double GetMilisecondsElapsed()
	{
		if (isRunning)
		{
			auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
			return elapsed.count();
		}
		else
		{
			auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
			return elapsed.count();
		}
	}
	void Restart()
	{
		isRunning = true;
		start = std::chrono::high_resolution_clock::now();
	}
	//
	bool Stop()
	{
		if (!isRunning)
		{
			return false;
		}
		else
		{
			stop = std::chrono::high_resolution_clock::now();
			isRunning = false;
			return true;
		}
	}
	bool Start()
	{
		if (isRunning)
		{
			return false;
		}
		else
		{
			start = std::chrono::high_resolution_clock::now();
			isRunning = true;
			return true;
		}
	}
	void make_delta_time()
	{
		this->deleta_time = static_cast<float>(GetMilisecondsElapsed()) / 1000.0f;
		Restart();
	}
	friend friend_class;
public:
	float get_deleta_time()
	{
		return deleta_time;
	}
};

