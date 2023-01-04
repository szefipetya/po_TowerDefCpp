#pragma once

#include "Building.h"

class Economic : public Building
//class Economic : public Entity
{
private:
	int interest;

public:
	Economic(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y)
	{
		if (!canCreate()) 
		{
			std::cout << "could not create (economic)" << std::endl;
			this->~Economic();
			return;
		}

		modelOnTickHandler = tick;
		modelOnDeath = death;
		this->hp = 550;
		this->maxhp = 550;
		this->dmg = 0;
		this->status = Entity::Status::idle;
		this->speed = 0.0f;
		this->size = 3;
		this->aps =1.0f;
		this->cost = 40;
		this->type = Entity::Type::Economic;
		this->x = x;
		this->y = y;
		interest = 5;
		t = new CustomTimer();
		count++;
	}	

	
	virtual bool action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix, std::atomic<int>& money)
	{
		money += interest;
		return true;
	}
	int getInterest() {
		return interest;
	}
	virtual void upradge(BuildingMetaData data)override {
		this->dmg = data.dmg;
		this->maxhp = data.hp;
		this->hp = data.hp;
		this->interest = data.range;
	}
};

