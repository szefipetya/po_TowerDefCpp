#pragma once


#include "Unit.h"
class Building : public Entity
{
protected:
	int cost;
	int level=1;
	int range=5;
	Unit* target = nullptr;
public:
	Unit* getTarget() {
		return target;
	}

	int getCost() const {
		return cost;
	}
	int getLevel() const {
		return level;
	}
	void setLevel(const int argLevel)
	{
		level = argLevel;
	}

	std::string ProduceSaveString() override
	{
		std::string returnString;
		returnString = std::to_string(getType()) + "-" + std::to_string(getHp()) + "-" + std::to_string(getLevel());
		return returnString;
	}	

	virtual void initTimer() override {
		try {
			if (aps != 0.0f)
				t->setIntervalWithInit<Entity>(Entity::onSpawn, Entity::onTick, this, 1 / aps * 1000);
			else {

			}
		}
		catch (std::exception e) {
			std::cout << e.what() << "\n";
		}
	}
	virtual void upradge(BuildingMetaData data) {
		this->dmg = data.dmg;
		this->maxhp = data.hp;
		this->hp = data.hp;
		this->range = data.range;
	}
	
	//returns the new level
};

