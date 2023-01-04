#pragma once
#include <vector>
#include <algorithm>
#include "Field.h"
#include "Orc.h"
#include "Ogre.h"
#include "Skeleton.h"
#include "Goblin.h"
#include "Economic.h"
#include "Laser.h"
#include "ThreadSafeVector.h"
#include "Turret.h"
#include "Artillery.h"
#include "TowerDefFileAccess.h"
#include "shopEventArgs.h"
class Entity;
class Field;
#include "EventArgs.h"
#include<atomic>


class Model
{
public:
	
	ThreadSafeVector<ThreadSafeVector<Field*> >matrix;
	int matrixSizeX = 0;
	int matrixSizeY=0;
	int ids;
	Entity::Difficulty diff = Entity::Medium;
	std::atomic<int> money; //Tracks the player's resources
	int playerHealth; // Tracks the remaining hitpoints of the player
	int waveCount = 1;
	int seconds = 0;
	int difficulty;
	bool bStopActivities;
	Entity::Type shopType;//ha -1, akkor nincs semmi kiválasztva (jobb klikk)
	ThreadSafeVector<Entity*> selectedOnes;
	CustomTimer* timer=nullptr;
	ThreadSafeVector<std::pair<int, int>> buildingCenters;


	Building* selectedBuilding;//táblára kattintás esetén, ha egy mezõben egyértelmûen csak egy épület látható, akkor az Entity* ide kerül.

	//Level up system:
	//LEVEL
	//[Entity::Type][Level] +5
	int im = 2;

	BuildingMetaData metaMapEasy[4][3] = {
		{{0,250,5*im,300},	{0,400,12 * im,600},	{0,750,25 * im,1000}},//economy range->income (3rd value)
		{{20,300,10,100},	{50,550,13,160},		{150,850,18,350}},//mage (turret)
		{{6,550,10,170,3},	{20,650,13,220,4},		{60,1500,18,550,5}},//artillery
		{{5,350,10,150},	{15,650,13,260},		{25,1200,18,420}},//laser
	};

	BuildingMetaData metaMapMedium[4][3] = {
		{{0,200,4 * im,300},{0,300,7 * im,600},		{0,550,14 * im,1000}},//economy range->income (3rd value)
		{{16,200,10,100},	{50,450,13,160},		{150,850,18,350}},//mage (turret)
		{{5,440,10,170,3},	{15,650,13,220,4},		{50,1500,18,550,5}},//artillery
		{{4,300,10,150},	{12,500,13,260},		{20,1050,18,420}},//laser
	};
		
	BuildingMetaData metaMapHard[4][3] = {
		{{0,150,3 * im,300},{0,250,6 * im,600},		{0,400,10 * im,1000}},//economy range->income (3rd value)
		{{12,160,10,100},	{35,360,13,160},		{110,600,18,350}},//mage (turret)
		{{4,400,10,170,3},	{10,550,13,220,4},		{40,850,18,550,5}},//artillery
		{{3,200,10,150},	{9,300,13,260},			{15,900,18,420}},//laser
	};

	//   1.					2.						3.

	//enum Type { Orc = 1, Goblin = 2, Ogre = 3, Skeleton = 4
	UnitMetaData EnemyEasy[4][3] = {
		{{50,150,1.1,1.0,1},	{50,150,1.1,1.0,1},	{50,150,1.1,1.0,1}},//orc
		{{80,90,1.8,1.3,1},		{80,90,1.8,1.3,1},	{80,90,1.8,1.3,1}},//goblin
		{{70,350,0.3,0.5,1},	{70,350,0.3,0.5,1},	{70,350,0.3,0.5,1}},//ogre
		{{40,120,1.5,1.1,3},	{40,120,1.5,1.1,3},	{40,120,1.5,1.1,3}},//skeleton
	};

	/*struct UnitMetaData
	{
		int dmg;
		int maxhp;
		float speed;
		float aps;
		int range;
	};*/

	Model() {
		//resizeMatrix(20, 20);
		init();
	}
	Model(int x,int y) {
		resizeMatrix(y,x);
		init();
	}

	void setUpdateMoneyEventListener(std::function<void(EventArgs)> fun) {
		this->updateMoneyEvent = fun;
	}
	void setUpdateWaveEventListener(std::function<void(EventArgs)> fun) {
		this->updateWaveEvent = fun;
	}
	void setUpdateHealthEventListener(std::function<void(EventArgs)> fun) {
		this->updateHealthEvent = fun;
	}
	void setUpdateTImeEventListener(std::function<void(EventArgs)> fun) {
		this->updateTimeEvent = fun;
	}

	void setUpdateEntityEventListener(std::function<void(Entity*)> fun) {
		this->updateEntityEvent = fun;
	}
	void setSpawnEntityEventListener(std::function<void(Entity*)> fun) {
		this->spawnEntityEvent = fun;
	}
	void setOnEntityDeathEventListener(std::function<void(Entity*)> death) {
		this->viewEntityDeath = death;
	}

	void setOnBuildingUpradgeEventListener(std::function<void(Building*)> fun) {
		this->buildingUpradgeEvent = fun;
	}
	void setShopMode(EventArgs& type) {
		shopType = dynamic_cast<ShopEventArgs*> (&type)->shopType;
	}
	void init() {				
		
		setplayerResource(0);
		setPlayerHealth(20);
		waveCount = 1;
		shopType = Entity::Type::Undefined;
		initMatrix();
		timer = new CustomTimer();
		//	time->setInterval()
	}
	void resizeMatrix(int y, int x) {
		matrixSizeX = x;
		matrixSizeY = y;
		matrix.resize(matrixSizeX);
		//matrix = std::vector<std::vector<Field*> >(x);
		for (int i = 0; i < x; i++)
		{
			matrix[i].resize(matrixSizeY);
		}
	}
	//EVENTS
	std::function<void(EventArgs)> updateMoneyEvent = nullptr;
	std::function<void(EventArgs)> updateTimeEvent = nullptr;
	std::function<void(EventArgs)> updateHealthEvent = nullptr;
	std::function<void(EventArgs)> updateWaveEvent = nullptr;

	std::function<void(Entity*)> updateEntityEvent = nullptr;
	std::function<void(Entity*)> spawnEntityEvent = nullptr;
	std::function<void(Entity*)> viewEntityDeath = nullptr;
	std::function<void(Building*)> buildingUpradgeEvent = nullptr;



	void initMatrix() {
		for (int i = 0; i < matrixSizeX; i++) {
			for (int j = 0; j < matrixSizeY; j++) {
				matrix[i][j] = new Field(i, j);
			}
		}
	}
private:
	void onEntityTick(Entity* e) {
		if (e == nullptr) return;
		//if the Entity is capable of moving (ex: orc, skeleton...)
		if (1<=e->getType() && e->getType()<=4)
		{			
			//move			
			int prevx = e->getX();
			int prevy = e->getY();
			if (e->action(matrix, money)) {
				if (e->getStatus() == Entity::Status::attack_north) {
					if (updateEntityEvent != nullptr)
						updateEntityEvent(e);
				}
				if (e->getX() != prevx || e->getY() != prevy) {
					matrix[prevx][prevy]->removeEntity(e);
					matrix[e->getX()][e->getY()]->addEntity(e);
					updateEntityEvent(e);
				}
			}
			else {
				//beért a bázisra
				if (e->getY() >= 0)
				{
					matrix[e->getX()][e->getY()]->removeEntity(e);
					if (updateEntityEvent != nullptr)
						updateEntityEvent(e);
					e->~Entity();
					e = nullptr;
					setPlayerHealth(playerHealth - 1);
				}
			}

			//updateEvent();
		}
		//if the Entity is not capable of moving (ex: turret, economic...)
		else
		{
			//exception: write access violation
		

			e->action(matrix, money);
			if(updateMoneyEvent!=nullptr)
				updateMoneyEvent(EventArgs(money));
			//debug
			if(updateEntityEvent!=nullptr)
			updateEntityEvent(e);
			
		}
	}
	void onEntityDeath(Entity* e) 
	{		
		//matrix[e->getX()][e->getY()]->removeEntity(e);

		//Buildings
		if (e->getType() >= 5)
		{
			cleanUp(e);
		}
		else //Units
		{
			matrix[e->getX()][e->getY()]->removeEntity(e);
		}

		if (viewEntityDeath != nullptr)
			viewEntityDeath(e);
		//e = nullptr;
		//delete(e);
	}
	//innentõl ez a függvény spawnoltatja az egységeket a térképre, miután elkészült a szál a timerjükhöz.
	void onEntitySpawn(Entity* e) {
		int s = e->getSize();
		if (e->getType() >=5)
		{
			buildingCenters.push_back(std::pair<int, int>(e->getX(), e->getY()));
		}
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < s; j++)
			{
				matrix[e->getX() - (s / 2) + i][e->getY() - (s / 2) + j]->addEntity(e);
			}
		}		
			try {
				if (spawnEntityEvent != nullptr)
					spawnEntityEvent(e);
			}
			catch (std::exception ex) {
				std::cout << "error: " << ex.what();
			}
	}

public:

	void build (Entity::Type type, int x, int y)
	{		
		if (shopType != Entity::Type::Undefined) {
			
			if (diff == Entity::Easy)
			{
				if (metaMapEasy[type - 5][0].cost > money)
				{
				}
				else
				{
					if (canBuild(x, y, metaMapEasy[type - 5][0].size))
					{
						addEntity(type, x, y);
						money -= metaMapEasy[type - 5][0].cost;
						if (updateMoneyEvent != nullptr)
							updateMoneyEvent(EventArgs(money));
					}
				}
			}
			else if (diff == Entity::Medium)
			{
				if (metaMapMedium[type - 5][0].cost > money)
				{
				}
				else
				{
					if (canBuild(x, y, metaMapMedium[type - 5][0].size))
					{
						addEntity(type, x, y);
						money -= metaMapMedium[type - 5][0].cost;
						if (updateMoneyEvent != nullptr)
							updateMoneyEvent(EventArgs(money));
					}
				}
			}
			else if (diff == Entity::Hard)
			{
				if (metaMapHard[type - 5][0].cost > money)
				{
				}
				else
				{
					if (canBuild(x, y, metaMapHard[type - 5][0].size))
					{
						addEntity(type, x, y);
						money -= metaMapHard[type - 5][0].cost;
						if (updateMoneyEvent != nullptr)
							updateMoneyEvent(EventArgs(money));
					}
				}
			}
			
		}
	}

	bool canBuild(int x, int y, int size)
	{
		int itx = x;
		int ity = y;
		int maxx = itx + size;
		int maxy = ity + size;

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (x - (size / 2) + i < matrixSizeX && x - (size / 2) + i >= 0
					&& y - (size / 2) + j < matrixSizeY && y - (size / 2) + j >= 0) 
				{
					if (matrix[x - (size / 2) + i][y - (size / 2) + j]->getRawMap().size() > 0) 
						return false;
				}
				else 
				{ 
					return false;
				}

			}
		}
		return true;
	}


	void onEntityUpdateFn(Entity* e) {
		updateEntityEvent(e);
	}
	//az initTImer azért kell, hogy spawnoláskor el tudja magát küldeni a nézetnek, de ezt csak az után teheti meg, miután lefutott a konstruktora
	Entity* addEntity(Entity::Type en, int x, int y, int arghp=0, int arglevel = 1) {
		switch (en)
		{
		case Entity::Type::Economic:
		{
				using std::placeholders::_1;
				class Economic* ec = new class Economic(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y);
				ec->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
				ec->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
				ec->initTimer();
				
				if (arglevel != 0) {
					ec->setLevel(arglevel);

					if (diff == 0)
					{
						ec->upradge(metaMapEasy[ec->getType() - 5][arglevel - 1]);
					}
					else if (diff == 1)
					{
						ec->upradge(metaMapMedium[ec->getType() - 5][arglevel - 1]);
					}
					else if (diff == 2)
					{
						ec->upradge(metaMapHard[ec->getType() - 5][arglevel - 1]);
					}

					
					if (arghp != 0)
					{
						ec->setHealth(arghp);
					}
				}

			break;
		}

		case Entity::Type::Laser:
		{
			using std::placeholders::_1;
			class Laser* ec = new class Laser(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y);
			ec->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			ec->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
			
			if (arglevel != 0) {

				ec->setLevel(arglevel);

				if (diff == 0)
				{
					ec->upradge(metaMapEasy[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 1)
				{
					ec->upradge(metaMapMedium[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 2)
				{
					ec->upradge(metaMapHard[ec->getType() - 5][arglevel - 1]);
				}

				
				if (arghp != 0)
				{
					ec->setHealth(arghp);
				}
				//buildingUpradgeEvent(ec);
			}
			ec->initTimer();

			break;
		}

		case Entity::Type::Orc:
		{
			using std::placeholders::_1;
			//class Orc* e = new class Orc(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y); //10 is the damage
			class Orc* e = new class Orc(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y, EnemyEasy, diff);
			e->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			e->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
			e->initTimer();
			if (arghp != 0)
			{
				e->setHealth(arghp);
			}
			break;
		}

		case Entity::Type::Goblin:
		{
			using std::placeholders::_1;
			class Goblin* e = new class Goblin(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y, EnemyEasy, diff); //10 is the damage
			e->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			e->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
			e->initTimer();
			if (arghp != 0)
			{
				e->setHealth(arghp);
			}
			break;
		}

		case Entity::Type::Ogre:
		{
			using std::placeholders::_1;
			//class Ogre* e = new class Ogre(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y); //10 is the damage
			class Ogre* e = new class Ogre(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y, EnemyEasy, diff); //10 is the damage
			e->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			e->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
			e->initTimer();
			if (arghp != 0)
			{
				e->setHealth(arghp);
			}
			break;
		}

		case Entity::Type::Skeleton:
		{
			using std::placeholders::_1;
			class Skeleton* e = new class Skeleton(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y, EnemyEasy, diff); //10 is the damage
			e->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			e->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));
			e->initTimer();
			if (arghp != 0)
			{
				e->setHealth(arghp);
			}
			//updateEntityEvent(e);
			return e;
			break;
		}

		case Entity::Type::Turret:
		{
			using std::placeholders::_1;
			class Turret* ec = new class Turret(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y); //10 is the damage
			ec->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			ec->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));

			if (arglevel != 0) {

				ec->setLevel(arglevel);
				if (diff == 0)
				{
					ec->upradge(metaMapEasy[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 1)
				{
					ec->upradge(metaMapMedium[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 2)
				{
					ec->upradge(metaMapHard[ec->getType() - 5][arglevel - 1]);
				}
				if (arghp != 0)
				{
					ec->setHealth(arghp);
				}
				//buildingUpradgeEvent(ec);
			}
			ec->initTimer();
			break;
		}

		case Entity::Type::Artillery:
		{
			using std::placeholders::_1;
			class Artillery* ec = new class Artillery(std::bind(&Model::onEntityTick, this, _1), std::bind(&Model::onEntityDeath, this, _1), x, y); //10 is the damage
			ec->setSpawnHandler(std::bind(&Model::onEntitySpawn, this, _1));
			ec->setUpdateHandler(std::bind(&Model::onEntityUpdateFn, this, _1));

			if (arglevel != 0) {

				ec->setLevel(arglevel);
				if (diff == 0)
				{
					ec->upradge(metaMapEasy[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 1)
				{
					ec->upradge(metaMapMedium[ec->getType() - 5][arglevel - 1]);
				}
				else if (diff == 2)
				{
					ec->upradge(metaMapHard[ec->getType() - 5][arglevel - 1]);
				}
				if (arghp != 0)
				{
					ec->setHealth(arghp);
				}
				//buildingUpradgeEvent(ec);
			}
			ec->initTimer();
			return ec;
			break;
		}
		}

	}

	void newWave(int enemyCount, int lines)
	{
		std::cout << "wave: " << getWaveCount() << std::endl;
		std::cout << "enemies: " << enemyCount << std::endl;
		std::cout << "lines: " << lines << std::endl;
		//waves
		for (int i = 0; i < lines; i++)
		{		
			//lines
			for (int j = 0; j < enemyCount; j++)
			{
				int posx = Utils::random(0, matrixSizeX - 1);
				int random = Utils::random(0, 3);

				if (random == 0)
				{
					addEntity(Entity::Orc, posx, matrixSizeY - 1);
				}
				else if (random == 1)
				{
					addEntity(Entity::Goblin, posx, matrixSizeY - 1);
				}
				else if (random == 2)
				{
					addEntity(Entity::Ogre, posx, matrixSizeY - 1);
				}
				else if (random == 3)
				{
					addEntity(Entity::Skeleton, posx, matrixSizeY - 1);
				}

			}

			std::this_thread::sleep_for(std::chrono::milliseconds(Utils::random(1000, 3000)));	//TODO: set this hgher for the final ver
		}

		setWaveCount(getWaveCount() + 1);	
		updateWaveEvent(EventArgs(getWaveCount()));
	}
	int ngpress_count = 0;
	void newGame(EventArgs e) {
		ngpress_count++;

		std::thread t([=]()
		{
				waveCount = 1;
		bool init = true;
			deleteEntities();
			int ngpress_initial = ngpress_count;
			while (getPlayerHealth() > 0)
			{
				int enemyCount;
				float lines = 1;
				if (diff == 0)
				{
					enemyCount = getWaveCount();
					//lines do not increase
					if (init) {
						this->setplayerResource(600);
						this->setPlayerHealth(50);
						init = false;
					}
				}
				else if (diff == 1)
				{
					enemyCount = getWaveCount() * 1.5;
					lines *= 1.2;
					lines += lines * 0.2;

					if (init) {
						this->setplayerResource(500);
						this->setPlayerHealth(20);
						init = false;
					}
				}
				else if (diff == 2)
				{
					enemyCount = getWaveCount() * 2;
					lines *= 1.5;
					if (init) {
						this->setplayerResource(300);
						this->setPlayerHealth(1);
						init = false;
					}
				}

				std::cout << "lines bef: " << lines << std::endl;
				if (ngpress_count != ngpress_initial) return;
				newWave(enemyCount, lines);

				std::this_thread::sleep_for(std::chrono::milliseconds(Utils::random(10000, 15000)));	//TODO: set this hgher for the final ver
			}
		});
		t.detach();
	}

	void cleanUp(Entity* e) //remove building from the matrix
	{

		int x = e->getX();
		int y = e->getY();
		
		int size = e->getSize();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (x - (size / 2) + i < matrixSizeX && x - (size / 2) + i >= 0
					&& y - (size / 2) + j < matrixSizeY && y - (size / 2) + j >= 0) {
					matrix[x - (size / 2) + i][y - (size / 2) + j]->removeEntity(e);
				}
				else {
					std::cout << "out of map\n";
					return;
				}

			}
			std::cout << "\n";
		}
	}

	// Returns the player's current expendable reasources
	int getPlayerResource() const {
		return money;
	}
	//Returns the player's remaining Hitpoints (If the player loses all hitpoints the game ends)
	int getPlayerHealth() const {
		return playerHealth;
	}
	int getWaveCount() const {
		return waveCount;
	}

	int getDifficulty() const {
		return diff;
	}
	//Sets the argument to be the new health value
	void setPlayerHealth(const int argHealth)
	{
		playerHealth = argHealth;
		if(updateHealthEvent!=nullptr)
			updateHealthEvent(EventArgs(playerHealth));
	}
	//Sets the argument to be the new money
	void setplayerResource(const int argResource)
	{
		money = std::clamp(argResource,0,100000);
		if (updateMoneyEvent != nullptr) {
			updateMoneyEvent(EventArgs(money));
		}
	}
	//!!!REDUNDANT!!!Adds the argument value to the health of the player 
	void addToPlayerHealth(const int argHealth)
	{
		playerHealth += argHealth;
	}
	//Sets the argument to be the new Difficulty
	void setDifficulty(const int argDifficulty)
	{
		difficulty = argDifficulty;
		diff = (Entity::Difficulty)argDifficulty;
	}
	//Sets the argument to be the new WaveCount
	void setWaveCount(const int argWaveCount)
	{
		waveCount = argWaveCount;
	}
	//Iterates through a string of formatted code and while doing so spawns the Entity and sets it's values
	void ProcessLoadLine(std::string loadline,int coordX, int coordY)
	{
		std::string str = loadline;
		int num = str[0] - 48;
		//std::cout << "NUM:" << num << std::endl;
		int iter_entity = 0;
		int pos = 2;
		for (int i = 0; i < num; i++)
		{
			//Spawn(str[pos]);
			int type = str[pos]-48;

			//SPAWN TYPE STR[POS] here to coordX, CoordY
			if (str[pos] - 48>=5)
			{
				pos = pos + 2;
				int health = std::stoi(str.substr(pos, str.find(" ") - pos)); //Get the Health
				//SetHealth(health);
				//matrix[coordX][coordY]->getEntities()[i]->setHealth(health);
				//std::cout << "Set the health to: " << health << std::endl;
				//SET HEALTH here
				str.replace(pos - 1, 1, "|");
				pos = str.find("-") + 1;
				//SetBuildingLevel(str[pos]);
				int level = str[pos]-48;
				addEntity((Entity::Type)type, coordX, coordY, health,level);
				//std::cout<<"Spawned a : "  << type << " with a health of: " << health << " at the level of: " << level << std::endl;
				//Building* TempBuilding = dynamic_cast<Building*>(matrix[coordX][coordY]->getEntities()[i]);
				//Building* TempBuilding = dynamic_cast<Building*>(temporaryentity);
				//TempBuilding->setLevel(str[pos]);
				//std::cout << "Set the bulding level to: " << str[pos] << std::endl;
				//SET BUILDINGLEVEL HERE
				pos = str.find(" ") + 1;
				str = str.substr(pos, str.length());
				pos = 0;
			}
			else
			{
				pos = pos + 2;
				if (i != num - 1)
				{
					int health = std::stoi(str.substr(pos, str.find(" ") - pos)); //Get the Health
					//SetHealth(health);
					addEntity((Entity::Type)type, coordX, coordY, health);
				//	std::cout << "Spawned a : " << type << " with a health of: "<< health << std::endl;
					//SET HEALTH here
					pos = str.find(" ") + 1;
					str = str.substr(pos, str.length());
					pos = 0;
				}
				else
				{
					int health = std::stoi(str.substr(pos, str.find(" ") - pos));
					//SetHealth(health);
					//matrix[coordX][coordY]->getEntities()[i]->setHealth(health);
					addEntity((Entity::Type)type, coordX, coordY, health);
				//	std::cout << "Spawned a : " << type << " with a health of: " << health << std::endl;
					//SET HEALTH HERE
				}
			}
		}
	}
	//Gathers the variables need to create a SaveFormat and returns it
	SaveFormat AssembleSaveData(const int argMatrixSizeX, const int argMatrixSizeY, const int argPlayerHealth, const int argPlayerResource, const int argWaveCount, const int argDifficulty, const int argElapsedTime)
	{

		struct SaveFormat ReturnStruct;
		ReturnStruct.MatrixX = argMatrixSizeX;
		ReturnStruct.MatrixY = argMatrixSizeY;
		ReturnStruct.PlayerHealth = argPlayerHealth;
		ReturnStruct.PlayerResource = argPlayerResource;
		ReturnStruct.WaveCount = argWaveCount;
		ReturnStruct.Difficulty = argDifficulty;
		ReturnStruct.ElapsedTime = argElapsedTime;
		//resize the representative matrix
		ReturnStruct.MatrixRep.resize(matrixSizeX);
		for  (int i = 0; i < matrixSizeX; i++)
		{
			ReturnStruct.MatrixRep[i].resize(matrixSizeY);
		}
		// Fills up the representative matrix values depending on the types of entities on a 
		for (int i = 0; i < matrixSizeX; i++)
		{
			for (int j = 0; j < matrixSizeY; j++)
			{
				if (matrix[i][j]->getEntities().size() > 0)
				{
					for (int k = 0; k < matrix[i][j]->getEntities().size(); k++)
					{
						if (matrix[i][j]->getEntities()[k]->getType() >=5)
						{
							bool bIsCenter = false;
							//Nezi, hogy a k-adik building egy cellában elpuletcentrum-e
							for (int l = 0; l < buildingCenters.size(); l++)
							{
								if (i == buildingCenters[l].first && j == buildingCenters[l].second)
								{
									bIsCenter = true;
								}
							}
							if (bIsCenter)
							{
								if (k == 0)
								{
									ReturnStruct.MatrixRep[i][j].push_back(std::to_string(matrix[i][j]->getEntities().size())+"|"+matrix[i][j]->getEntities()[k]->ProduceSaveString());
								}
								else
								{
									ReturnStruct.MatrixRep[i][j].push_back(matrix[i][j]->getEntities()[k]->ProduceSaveString());
								}
							}
							else
							{
								ReturnStruct.MatrixRep[i][j].push_back(std::to_string(-1));
							}
						}
						else
						{
							if (k == 0)
							{
								ReturnStruct.MatrixRep[i][j].push_back(std::to_string(matrix[i][j]->getEntities().size()) + "|" + matrix[i][j]->getEntities()[k]->ProduceSaveString());
							}
							else
							{
								ReturnStruct.MatrixRep[i][j].push_back(matrix[i][j]->getEntities()[k]->ProduceSaveString());
							}
							
						}
						//Building Center "filter"
						//ReturnStruct.MatrixRep[i][j].push_back(matrix[i][j]->getEntities()[k]->ProduceSaveString());
					}
				}
				else
				{
					ReturnStruct.MatrixRep[i][j].push_back(std::to_string(-1));
				}
			}
		}
		return ReturnStruct;
	}
	//Loads a New game from a LoadFormat
	void LoadNewGame(SaveFormat argSaveFormat)
	{
		//Clear the matrix
		//------
		//Set the data
		setPlayerHealth(argSaveFormat.PlayerHealth);
		std::cout << "Loaded Health: " << argSaveFormat.PlayerHealth << std::endl;
		setplayerResource(argSaveFormat.PlayerResource);
		std::cout << "Loaded Resource: " << argSaveFormat.PlayerResource << std::endl;
		setWaveCount(argSaveFormat.WaveCount);
		std::cout << "Loaded WaveCount: " << argSaveFormat.WaveCount << std::endl;
		setDifficulty(argSaveFormat.Difficulty);
		std::cout << "Loaded Difficulty: " << argSaveFormat.Difficulty << std::endl;
		//Spawn Entities
		for (int i = 0; i < argSaveFormat.MatrixX; i++)
		{
			for (int j = 0; j < argSaveFormat.MatrixY; j++)
			{
				if (argSaveFormat.MatrixRep[i][j][0] != "-1")
				{
					ProcessLoadLine(argSaveFormat.MatrixRep[i][j][0],i,j);
				}
			}
		}
	}
	//________TEMPORARILY BOUND FUNCTIONS 
	void TempSaveGameButtonFunction()
	{
		SaveGame(AssembleSaveData(GetMatrixSizeX(),GetMatrixSizeY(),getPlayerHealth(), getPlayerResource(), getWaveCount(), getDifficulty(), 333), "Test.txt");
	}
	void TempLoadGameButtonFunction()
	{
		//Stop Timer
		//Load the data
		deleteEntities();
		LoadNewGame(LoadGame("Test.txt"));
		//Start Timer
	}
	//________TEMPORARILY BOUND FUNCTIONS 

	//ON TABLE CLICK

	void onUpradgeClick() {
		if (selectedBuilding != NULL&&selectedBuilding->getLevel()<3) {
		std::cout << "upradgeclick\nlevel: "<<selectedBuilding->getLevel()<<"\n";
			std::cout << "selected yes building\n";

			if (diff == Entity::Easy)
			{
				if (money >= metaMapEasy[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost)
				{
					selectedBuilding->upradge(metaMapEasy[selectedBuilding->getType() - 5][selectedBuilding->getLevel()]);
					money -= metaMapEasy[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost;
					selectedBuilding->setLevel(selectedBuilding->getLevel() + 1);
					updateMoneyEvent(EventArgs(money));
					updateEntityEvent(selectedBuilding);
					buildingUpradgeEvent(selectedBuilding);
				}
				else {
					std::cout << "not enough money for upradge or reached max level.\n";
				}
			}
			else if (diff == Entity::Medium)
			{
				if (money >= metaMapMedium[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost)
				{
					selectedBuilding->upradge(metaMapEasy[selectedBuilding->getType() - 5][selectedBuilding->getLevel()]);
					money -= metaMapMedium[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost;
					selectedBuilding->setLevel(selectedBuilding->getLevel() + 1);
					updateMoneyEvent(EventArgs(money));
					updateEntityEvent(selectedBuilding);
					buildingUpradgeEvent(selectedBuilding);
				}
				else {
					std::cout << "not enough money for upradge or reached max level.\n";
				}
			}
			else if (diff == Entity::Hard)
			{
				if (money >= metaMapHard[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost)
				{
					selectedBuilding->upradge(metaMapEasy[selectedBuilding->getType() - 5][selectedBuilding->getLevel()]);
					money -= metaMapHard[selectedBuilding->getType() - 5][selectedBuilding->getLevel()-1].cost;
					selectedBuilding->setLevel(selectedBuilding->getLevel() + 1);
					updateMoneyEvent(EventArgs(money));
					updateEntityEvent(selectedBuilding);
					buildingUpradgeEvent(selectedBuilding);
				}
				else {
					std::cout << "not enough money for upradge or reached max level.\n";
				}
			}

			/*if (money >= metaMap[selectedBuilding->getType() - 5][selectedBuilding->getLevel()].cost) 
			{
				selectedBuilding->upradge(metaMap[selectedBuilding->getType() - 5][selectedBuilding->getLevel()]);
				selectedBuilding->setLevel(selectedBuilding->getLevel() + 1);
				money -= metaMap[selectedBuilding->getType() - 5][selectedBuilding->getLevel()].cost;
				updateMoneyEvent(EventArgs(money));
				updateEntityEvent(selectedBuilding);
				buildingUpradgeEvent(selectedBuilding);
			}
			else {
				std::cout << "not enough money for upradge or reached max level.\n";
			}*/
			/*building
			selectedBuilding->struct */
		}
		else {
			std::cout << "select a building first\n";
		}
	}

	ThreadSafeMap<std::thread::id, Entity*> tableClickEvent(int x,int y) {
		build(shopType, x, y);
		selectedBuilding = dynamic_cast<Building*>(matrix[x][y]->searchBuilding());
		return matrix[x][y]->onClick_GetThreadIds();
	}

	void onTimerTick() {
		seconds++;
		updateTimeEvent(seconds);
	}
	//static wrappers
	static void onTimerTickWrapper(Model* m) {
		m->onTimerTick();
	}
	int GetMatrixSizeX() const
	{
		return matrixSizeX;
	}
	int GetMatrixSizeY() const
	{
		return matrixSizeY;
	}
	void deleteEntities()
	{
		buildingCenters.clear();
		for (int i = 0; i < GetMatrixSizeX(); i++)
		{
			for (int j = 0; j < GetMatrixSizeY(); j++)
			{
				if (matrix[i][j]->getEntities().size() != 0)
				{
					for (int k = 0; k < matrix[i][j]->getEntities().size(); k++)
					{
						matrix[i][j]->getEntities()[k]->~Entity();
					}
					matrix[i][j]->getRawMap().m_getMap().clear();
				}
			}
		}
	}

};

