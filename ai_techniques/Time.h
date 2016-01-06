#pragma once
#include "Behaviour.h"
#include <chrono>

class Time : public Behaviour
{
private:
	static Time* instance;
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point currentTime;
	std::chrono::high_resolution_clock::time_point lastTime;
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