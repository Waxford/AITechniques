#pragma once
#include "Behaviour.h"
#include <chrono>

class Time : public Behaviour
{
private:
	static Time* instance;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> currentTime;
	std::chrono::time_point<std::chrono::steady_clock> lastTime;
public:
	Time(void);
	~Time(void);
	static Time* Instance()
	{
		if (instance == nullptr)
		{
			instance = new Time();
		}
		return instance;
	}

	static float GetTime();
	static float GetDeltaTime();

	void Update();
};