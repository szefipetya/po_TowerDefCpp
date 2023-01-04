class Model;
#pragma once
#include<string>
//#include <SDL.h>
#include "customtimer.h"
#include "ThreadSafeVector.h"
#include <functional>
#include<atomic>
class Model;
class Field;

class Entity
{
public:
	enum Difficulty { Easy=0, Medium=1, Hard=2 };
	enum Status{walk_north, walk_south, walk_east, walk_west,
		attack_north, attack_south, attack_east, attack_west,
		death, idle
	};
	//Added ints for easier persistence operation
	
	enum Type {Undefined=0, Orc = 1, Goblin = 2, Ogre = 3, Skeleton = 4, Economic = 5 ,Turret = 6, Artillery = 7,Laser=8};
protected:
	static int count;
	int hp;
	int maxhp;
	float speed;
	int dmg;
	int x;
	int y;
	bool selected = false;
	int size = 1;
	float virtual_scale = 1.0f;
	float aps;
	CustomTimer* t;
	std::function<void(Entity*)> modelOnTickHandler=nullptr;
	std::function<void(Entity*)> modelOnSpawn= nullptr;
	std::function<void(Entity*)> modelOnDeath = nullptr;
	std::function<void(Entity*)> modelOnUpdate = nullptr;
	std::thread::id thread_id;
	static const int MAX_COUNT;
	//std::atomic<Entity::Status> status=Status::walk_north;
	Entity::Status status=Status::walk_north;
	
	Type type;
public:
	float getAps() {
		return aps;
	}
	Entity(std::function<void(Entity*)> tick, std::function<void(Entity*)> death, int x, int y) {
		if (!canCreate()) {
			this->~Entity();
			return;
		}
		aps = 1.0f;
		modelOnTickHandler = tick;
		modelOnDeath = death;
		this->x = x;
		this->y = y;
		t = new CustomTimer();
	}
	//Ezt majd elmondom szóban, hogy miért csináltam, mert hosszú
	virtual void initTimer() {
		try {
			if(speed!=0.0f)
				t->setIntervalWithInit<Entity>(Entity::onSpawn,Entity::onTick, this,1/speed*1000);
			else {

			}
		}
		catch (std::exception e) {
			std::cout << e.what() << "\n";
		}
	}
	Entity() {}
	
	//methods
	void setSpawnHandler(std::function<void(Entity*)> fn) {
		modelOnSpawn = fn;
	}
	void setUpdateHandler(std::function<void(Entity*)> fn) {
		modelOnUpdate = fn;
	}
	bool virtual action(ThreadSafeVector<ThreadSafeVector<Field*> >& matrix, std::atomic<int>& money) { return true; }
	void onTick() {
		if(this!=nullptr)
			modelOnTickHandler(this);
	}
	void onSpawn() {
		this->thread_id = std::this_thread::get_id();
		modelOnSpawn(this);
	}
	//STATIC METHODS//
	static bool canCreate() {
		return count < MAX_COUNT;
	}
	static void onSpawn(Entity* e) {
		e->onSpawn();
	}
	static void onTick(Entity* e) {
		if(e!=nullptr)
		e->onTick();
	 }
	int getSize() {
		return size;
	}	
	float getVirtualScale() {
		return virtual_scale;
	}
	//setters//
	void setSelected(bool l) {
		try {
			selected = l;
		}
		catch (std::exception ex) {
			std::cout << ex.what();
			std::cout << "probably writing access denied";
		}
	}
	void setX(int x)
	{
		this->x = x;
	}
	void setY(int y)
	{
		this->y = y;
	}
	void setStatus(Status s) {
		status = s;
	}
	void setHealth(const int argHealth)
	{
		this->hp = argHealth;
		maxhp = argHealth;

	}
	//getters//
	bool isSelected() {
		try {
			if (this != nullptr)
				return selected;
			else {
				return false;
			}
		}
		catch (std::exception ex) {
			ex.what();
			std::cout << "probably read access violation";
		return false;
		}
	}
	int getHp() {
		return hp;
	}int getMaxHp() {
		return maxhp;
	}
	int getDmg() {
		return dmg;
	}
	
	float getSpeed() {
		return speed;
	}
	static int getCount()
	{
		return count;
	}
	std::thread::id getThread_id() const
	{
		return thread_id;
	}
	Type getType() {
		return type;
	}
	int getHp() const {
		return hp;
	}
	int getX() const
	{
		return x;
	}
	int getY() const
	{
		return y;
	}
	CustomTimer* getTimer() {
		return t;
	 }
	Status getStatus() {
		if (this != nullptr)
			return status;

		else return Status::idle;
	}
	
	//destructor//
	 virtual ~Entity() {
		 count--;
			 t->stop();
			 if (modelOnDeath != nullptr) {
				 modelOnDeath(this);
				 //std::cout << "deleting from model"<<type<<"\n";
			 }
	 }
	 bool damage(int dmg)
	 {
		 if (this->hp - dmg <= 0)
		 {
			 this->hp -= dmg;
			// std::cout << "[Entity] an entity just died" << std::endl;
			 //DES
			 this->status = death;
			 this->~Entity();
			 return true;
		 }
		 else
		 {
			 this->hp -= dmg;
			 //std::cout << "[Entity] an Entity was just shot; new hp: " << this->hp << std::endl;
			 modelOnUpdate(this);
			 return false;
		 }
	 }

	 virtual std::string ProduceSaveString()
	 {
		 return "Nothing";
	 }

	 void setMaxHp(int arghp) {
		 maxhp = arghp;
	 }

};


struct BuildingMetaData
{
	int dmg;
	int hp;
	int range;
	int cost;
	int explosion = 0;
	int size = 3;
};

struct UnitMetaData
{
	int dmg;
	int maxhp;
	float speed;
	float aps;
	int range;
};