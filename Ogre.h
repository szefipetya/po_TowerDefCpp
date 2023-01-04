#pragma once

#include "Unit.h"
#include "Entity.h"
#include <functional>

class Ogre : public Unit
{

public:
	Ogre(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y) : Unit(tick, death, x, y)
	{
		if (!canCreate()) {
			this->~Ogre();
			return;
		}
		this->hp = 350;
		this->maxhp = 350;
		this->dmg = 70;
		this->size = 1;
		this->virtual_scale = 2.5;
		this->speed = 0.3;
		this->aps = 0.5;
		this->type = Entity::Type::Ogre;
		status = Status::walk_north;
		count++;

	}

	Ogre(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y, UnitMetaData data[4][3], Entity::Difficulty diff) : Unit(tick, death, x, y)
	{
		if (!canCreate()) {
			this->~Ogre();
			return;
		}


		this->size = 1;
		this->virtual_scale = 2.5;
		
		int i = Entity::Type::Ogre - 1;
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
		
		this->type = Entity::Type::Ogre;
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
					t->setIntervalWithInit<Entity>(Entity::onTick, Entity::onTick, this, 1/aps*1000);
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
					t->setIntervalWithInit<Entity>(Entity::onTick, Entity::onTick, this, 1/speed*1000);
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

