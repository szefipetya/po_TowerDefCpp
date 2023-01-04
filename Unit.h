#pragma once
#include "Entity.h"
class Unit :public Entity {
protected:
public:
	Unit() :Entity() {
		aps = 1.0f;
	}
	Unit(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y) : Entity(tick, death, x, y) {}

	virtual void action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix) {
	}
	std::string ProduceSaveString() override
	{
		std::string returnString;
		returnString = std::to_string(getType()) + "-" + std::to_string(getHp());
		return returnString;
	}
};
