#pragma once

#include "Entity.h"
#include <functional>
#include "Unit.h"
class Orc : public Unit
{
public:

	Orc(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y) : Unit(tick, death,  x, y)
	{
		if (!canCreate()) {
			this->~Orc();
			return;
		}
		this->hp = 150;
		this->maxhp = 150;
		this->dmg = 50;
		this->size = 1;
		this->virtual_scale = 1.5;
		this->speed = 1.1;
		this->aps = 1;
		this->type = Entity::Type::Orc;
		status = Status::walk_north;
		count++;
		
	}

	Orc(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y, UnitMetaData data[4][3], Entity::Difficulty diff) : Unit(tick, death, x, y)
	{
		if (!canCreate()) {
			this->~Orc();
			return;
		}

		int i = Entity::Type::Orc - 1;
		if (diff == 0)
		{
			this->dmg = data[i][0].dmg;
			this->maxhp = data[i][0].maxhp;
			this->hp = data[i][0].maxhp;
			this->speed = data[i][0].speed;
			this->aps = data[i][0].aps;
		}
		else if (diff == 1)
		{
			this->dmg = data[i][1].dmg;
			this->maxhp = data[i][1].maxhp;
			this->hp = data[i][1].maxhp;
			this->speed = data[i][1].speed;
			this->aps = data[i][1].aps;
		}
		else if (diff == 2)
		{
			this->dmg = data[i][2].dmg;
			this->maxhp = data[i][2].maxhp;
			this->hp = data[i][2].maxhp;
			this->speed = data[i][2].speed;
			this->aps = data[i][2].aps;
		}
		this->size = 1;
		this->virtual_scale = 1.5;
		this->type = Entity::Type::Orc;
		status = Status::walk_north;
		count++;

	}

	

	virtual bool action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix, std::atomic<int>& money)
	{
		
		//still inside the play area
		if (getY() > 0 && getY() < matrix[0].size()
			&& getX() >= 0 && getX() < matrix.size())
		{
			Entity* e = matrix[getX()][getY() - 1]->searchBuilding();
			if (e != NULL)
			{
				//std::cout << "type: " << e->getType() << std::endl;
				//std::cout << "damaging building" << std::endl;

				if (status == Status::walk_north) {
					t->stop();

					t = new CustomTimer();
					t->setIntervalWithInit<Entity>(Entity::onTick, Entity::onTick, this,1/aps*1000);
					this->status = Entity::Status::attack_north;
					
				}
				//if damage was lethal ->true
				if (e->damage(dmg)) {
					
					this->status = Entity::Status::walk_north;
				}
				return true;
			}
			else
			{
				if (status == Status::attack_north) {
					t->stop();
					t = new CustomTimer();
					t->setIntervalWithInit<Entity>(Entity::onTick, Entity::onTick, this,1/speed*1000);
					this->status = Entity::Status::walk_north;
				}
				--y;
				return true;
			}
		}
		//leaving the play area
		else 
		{
			status = death;
			return false;
		}
	}


};

