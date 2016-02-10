#include "Time.h"
#include <chrono>

Time* Time::instance;
std::chrono::high_resolution_clock::time_point startTime;
std::chrono::high_resolution_clock::time_point currentTime;
std::chrono::high_resolution_clock::time_point lastTime;

Time::Time(void) 
{
	startTime = std::chrono::high_resolution_clock::now();
	currentTime = std::chrono::high_resolution_clock::now();
	lastTime = std::chrono::high_resolution_clock::now();
}
Time::~Time(void) 
{
}

float Time::GetTime()
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(Instance()->currentTime - Instance()->startTime).count();
}

float Time::GetDeltaTime()
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(Instance()->currentTime - Instance()->lastTime).count();
}

void Time::Update()
{
	Behaviour::Update();
	lastTime = currentTime;
	currentTime = std::chrono::high_resolution_clock::now();
}
/*
public:
	Time(void);
	~Time(void);
	static Time* Instance();
	float GetTime();
	float GetDeltaTime();*/