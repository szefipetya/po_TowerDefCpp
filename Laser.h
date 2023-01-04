#pragma once

#include "Field.h"
#include "Building.h"
#include "ThreadSafeMap.h"


class Laser : public Building
{
protected:
	int explosion;
	int range = 10;
	bool loaded; //has ammo

public:
	int stockdmg;
	Laser(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y)
	{
		if (!canCreate())
		{
			std::cout << "could not create (turret)" << std::endl;
			this->~Laser();
			return;
		}
		modelOnTickHandler = tick;
		modelOnDeath = death;
		this->hp = 350;
		this->maxhp = 300;
		this->stockdmg = 5;
		this->dmg = 5;
		this->status = Entity::Status::idle;
		this->speed = 0;
		this->size = 3;
		this->cost = 60;
		this->aps = 1.7;
		this->range = 8;
		this->type = Entity::Type::Laser;
		this->x = x;
		this->y = y;
		t = new CustomTimer();
		count++;
	}
	Laser() {}

	Entity* findEnemy(int x, int y, ThreadSafeVector<ThreadSafeVector<Field*> >& matrix)
	{

		if (x < matrix.size() && x >= 0
			&& y < matrix[0].size() && y >= 0)
		{
			std::vector<Entity*> ens = matrix[x][y]->getEntities();

			for (int i = 0; i < ens.size(); i++)
			{
				//std::cout << "type out: " << ens[i]->getType() << std::endl;
				if (1 <= ens[i]->getType() && ens[i]->getType() <= 4)
				{
					std::cout << "type in: " << ens[i]->getType() << std::endl;
					return ens[i];
				}
			}
		}

		//Entity* en;
		return NULL;
	}

	virtual bool action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix, std::atomic<int>& money)override
	{
		//std::cout << "my pos is: " << this->x << " " << this->y << std::endl;

		for (int i = 0; i < range * 2; i++)
		{
			for (int j = 0; j < range * 2; j++)
			{
				if (this->x - range + i >= 0 && this->y - range + j >= 0)//TODO: this is not complete
				{
					//if (matrix[this->x - range + i][this->y - range + j]->entities)
					//std::cout << "currently scanning: " << this->x - range + i << " " << this->y - range + j << std::endl;
					Entity* en = findEnemy(this->x - range + i, this->y - range + j, matrix);

					//shoot
						this->target = (Unit*)en;
					if (en != NULL)
					{
						//std::cout << "pewpew: " << this->x - range + i << this->y - range + j << std::endl;
						en->damage(this->dmg);

						//increase dmg
						if(dmg<=stockdmg*4)
						this->dmg *= 1.2;

						//if this return statement is removed it will shoot all enemies in range
						return true;
					}
					
				}
			}
		}
		this->dmg = stockdmg;
					
		return false;
	}
	virtual void upradge(BuildingMetaData data)override {
		this->dmg =this->stockdmg= data.dmg;
		this->maxhp = data.hp;
		this->hp = data.hp;
		this->range = data.range;
	}
};

