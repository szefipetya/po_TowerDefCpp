#pragma once

#include "Field.h"
#include "Building.h"
#include "ThreadSafeMap.h"



class Turret : public Building
{
protected:
	int explosion;
	int range = 10;
	bool loaded; //has ammo
	//int size=3;

public:
	Turret(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y)
	{
		if (!canCreate())
		{
			std::cout << "could not create (turret)" << std::endl;
			this->~Turret();
			return;
		}
		modelOnTickHandler = tick;
		modelOnDeath = death;
		this->hp = 300;
		this->maxhp = 300;
		this->dmg = 40;
		this->status = Entity::Status::idle;
		this->speed = 0;
		this->size = 3;
		this->cost = 60;
		this->type = Entity::Type::Turret;
		this->x = x;
		this->y = y;
		this->aps = 1.0f;
		t = new CustomTimer();
		count++;
	}
	Turret(){}

	Entity* findEnemy(int x, int y, ThreadSafeVector<ThreadSafeVector<Field*> >& matrix)
	{
		
		if (x < matrix.size() && x >= 0
			&& y < matrix[0].size() && y >= 0)
		{
			std::vector<Entity*> ens = matrix[x][y]->getEntities();

			for (int i = 0; i < ens.size(); i++)
			{
				//std::cout << "type out: " << ens[i]->getType() << std::endl;
				if (1<=ens[i]->getType()&& ens[i]->getType()<=4)
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
						//if this return statement is removed it will shoot all enemies in range
						return true;
					}
				}				
			}
		}
	}

	

	virtual int levelUp() {
		return 0;
	}
};

