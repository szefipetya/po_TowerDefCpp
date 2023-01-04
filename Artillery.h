#pragma once

#include "Turret.h"

class Artillery : public Turret
{
public:
	Artillery(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y)
	{
		if (!canCreate())
		{
			std::cout << "could not create (artillery)" << std::endl;
			this->~Artillery();
			return;
		}
		modelOnTickHandler = tick;
		modelOnDeath = death;
		this->hp = 300;
		this->maxhp = 300;
		this->dmg = 30;
		this->status = Entity::Status::idle;
		this->speed = 0;
		this->size = 3;
		this->aps = 0.5f;
		this->range = 10;
		this->cost = 60;
		this->type = Entity::Type::Artillery;
		this->x = x;
		this->y = y;
		t = new CustomTimer();
		//t->setIntervalWithInit<Entity>(Entity::onTick, Entity::onTick, this, aps);
		count++;
	}

	virtual bool action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix, std::atomic<int>& money)override
	{
		int maxx = this->x + range;
		int maxy = this->y + range;

		for (int i = 0; i < maxx; i++)
		{
			for (int j = 0; j < maxy; j++)
			{
				if (x - (range / 2) + i < matrix[0].size() && x - (range / 2) + i >= 0
					&& y - (range / 2) + j < matrix.size() && y - (range / 2) + j >= 0)
				{
					Entity* en = findEnemy(this->x - (range / 2) + i, this->y - (range / 2) + j, matrix);

					//shoot
					this->target = (Unit*)en;
					if (en != NULL)
					{
						std::cout << "found enemy" << std::endl;
						bomb(en->getX(), en->getY(), 7, matrix);

						return true;
					}
				}
			}
		}		
	}

	void bomb(int x, int y, int size, ThreadSafeVector<ThreadSafeVector<Field*> >& matrix)
	{
		int maxx = x + (size / 2);
		int maxy = y + (size / 2);

		for (int i = 0; i < size - 1; i++)
		{
			for (int j = 0; j < size - 1; j++)
			{
				int itx = x - (size / 2) + i;
				int ity = y - (size / 2) + j;

				if (itx < matrix[0].size() && itx >= 0
					&& ity < matrix.size() && ity >= 0)
				{
					Entity* en = findEnemy(itx, ity, matrix);

					//shoot
					if (en != NULL)
					{
						std::cout << "boom: " << itx << " " << ity << std::endl;
						en->damage(this->dmg);
						//j++;
						//break;
					}
				}
			}
		}
	}

	Entity* findEnemy(int x, int y, ThreadSafeVector<ThreadSafeVector<Field*> >& matrix)
	{
		if (x < matrix.size() && x >= 0
			&& y < matrix[0].size() && y >= 0)
		{
			std::vector<Entity*> ens = matrix[x][y]->getEntities();
			for (int i = 0; i < ens.size(); i++)
			{
				if (1 <= ens[i]->getType() && ens[i]->getType() <= 4)
				{
					return ens[i];
				}
			}
		}

		return NULL;
	}
	
};

