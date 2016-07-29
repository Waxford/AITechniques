#pragma once
#include<atomic>

class Behaviour
{
protected:
	static std::atomic<int> nextId;
	int id;
	int order;

public:
	Behaviour(void);
	virtual ~Behaviour(void);
	virtual void Update();
	virtual void LateUpdate();
	int GetID();
	void SetOrder(int);
	int GetOrder();

	size_t HashValue() const;
	bool Equals(const Behaviour&) const;
};