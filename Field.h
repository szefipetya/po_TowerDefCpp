#pragma once
#include <vector>
#include"Entity.h"
//#include"ThreadSafeVector.h"
#include"ThreadSafeMap.h"
#include<functional>
#include<algorithm>
class Field
{
public:
	int x;
	int y;
	ThreadSafeMap<std::thread::id,Entity*> entities;
	std::mutex _mut;
	Field(int x,int y) {
		this->x = x;
		this->y = y;
	}
	ThreadSafeMap<std::thread::id, Entity*> getRawMap() {
		return entities;
	}
	
	void removeEntity(Entity* e)
	{
		std::unique_lock<decltype(_mut)> lock(_mut);
		 std::unordered_map<std::thread::id, Entity*>::const_iterator it = entities.find(e->getThread_id());
		 if(it!=entities.end())
			 entities.erase(it);
	}
	Entity* searchEntityById(std::thread::id id) {
		std::unique_lock<decltype(_mut)> lock(_mut);

		std::unordered_map<std::thread::id, Entity*>::const_iterator it = entities.find(id);
		if (it != entities.end())
			return (*it).second;
		return NULL;
	}

	Entity* searchEntity(Entity::Type type)
	{
		std::unique_lock<decltype(_mut)> lock(_mut);

		std::unordered_map<std::thread::id, Entity*>::iterator it;
		for (it = entities.m_begin(); it != entities.m_end(); ++it) {
			if (it->second->getType() == type)
			{
				return it->second;
			}
		}

		return NULL;
	}


	Entity* searchBuilding()
	{
		std::unique_lock<decltype(_mut)> lock(_mut);
		std::unordered_map<std::thread::id, Entity*>::iterator it;
		for (it = entities.m_begin(); it != entities.m_end(); ++it) {
			if (it->second->getType() == Entity::Type::Economic || it->second->getType() == Entity::Type::Turret 
				|| it->second->getType() == Entity::Type::Artillery || it->second->getType() == Entity::Type::Laser)
			{
				return it->second;
			}
		}
		return NULL;
	}

	bool occupied()
	{
		std::unique_lock<decltype(_mut)> lock(_mut);
		if (entities.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::vector<Entity*> getEntities()
	{
		std::unique_lock<decltype(_mut)> lock(_mut);
		std::vector<Entity*> v;

		for (std::unordered_map<std::thread::id, Entity*>::iterator it = entities.m_begin(); it != entities.m_end(); ++it) {
			v.push_back(it->second);
		}

		return v;
	}
	void addEntity(Entity* ent)
	{
		std::unique_lock<decltype(_mut)> lock(_mut);
		entities.insert(std::pair<std::thread::id, Entity*>(ent->getThread_id(),ent));
	}

	ThreadSafeMap<std::thread::id, Entity*> onClick_GetThreadIds() {
		return entities;
	}
};

