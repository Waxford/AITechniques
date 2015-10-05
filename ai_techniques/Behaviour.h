#pragma once

class Behaviour
{
protected:
	static int nextId;
	int id;

public:
	Behaviour(void);
	~Behaviour(void);
	virtual void Update();
	int GetID();
};