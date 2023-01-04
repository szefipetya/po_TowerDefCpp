#pragma once
#include "Container.h"
#include<functional>
#include <chrono>
#include <mutex>
#include <thread>
#include<atomic>
#include <iostream>
#include <random>
#include "ThreadSafeMap.h"
#include"Sprite.h"
#define quote(x) #x

class TableContainer :public Container
{
public:
	TableContainer():Container() {
		init();
	}
	TableContainer(int x, int y, int w, int h, int lvl = 1) :Container(x, y, w, h, lvl) {
		init();
	}
	void setParentRect(SDL_Rect* r) {
		parent_rect = r;
	}
	std::atomic<bool> can_render = true;
	bool grid = false;
	SDL_FRect* hoverRect;
	std::mutex m_;
	ThreadSafeMap<std::thread::id, std::shared_ptr<Sprite>> sprites;
	std::unordered_map<std::thread::id, Entity*> selectedThreads;
	SDL_Rect* parent_rect;
	SDL_Rect shopRect;
	

	ThreadSafeMap<Entity::Type,Sprite> spriteBank;
	//App needs to get notified on tableclick to pass field for infopanel
	std::function<void(Field*)> onTableClickEvent;
	std::function<void(Entity*)> onEntityUpdateEvent;
	std::function<void(Entity*)> onEntityDeathEvent;
	void setOnTableClickEvent(std::function<void(Field*)> fun) {
		onTableClickEvent = fun;
	}
	void setOnEntityUpdateEvent(std::function<void(Entity*)> fun) {
		onEntityUpdateEvent = fun;
	}
	void setOnEntityDeathEvent(std::function<void(Entity*)> fun) {
		onEntityDeathEvent = fun;
	}
	
	virtual void render()override {
		//SDL_RenderSetViewport(rend, rect);
		renderBackgroundImg();
		Utils::setRenderColor(rend, "black");
		SDL_RenderFillRect(rend, rect);
		SDL_Rect hi = SDL_Rect();
		hi.x = (int)hoverRect->x;
		hi.y = (int)hoverRect->y;
		hi.w = (int)hoverRect->w;
		hi.h = (int)hoverRect->h;
		if (grid) renderGrid();

		if (hover) {
		Utils::setRenderColor(rend, "yellow",255);
			SDL_RenderDrawRect(rend, &hi);
			if (model->shopType != Entity::Type::Undefined) {
				if (can_build) {
					Utils::setRenderColor(rend, "green", 120);
					SDL_RenderFillRect(rend, &shopRect);
					Utils::setRenderColor(rend, "green", 240);
					SDL_RenderDrawRect(rend, &shopRect);
				}
				else {
					Utils::setRenderColor(rend, "red", 120);
					SDL_RenderFillRect(rend, &shopRect);
					Utils::setRenderColor(rend, "red", 240);
					SDL_RenderDrawRect(rend, &shopRect);
				}
			}
		}
		renderEntities();
		//renderEntities();
		// show content:
		//SDL_RenderSetViewport(rend, parent_rect);

			//SDL_RenderCopy(rend, (*it).second-> , NULL, (*it).second.first);
	}
	void init() {
		hoverRect = new SDL_FRect();
	}
	void renderGrid() {
		Utils::setRenderColor(rend,"white");
		if(border)SDL_RenderDrawRect(rend, rect);
		for (int i = 0; i < model->matrixSizeX; i++) {
				SDL_RenderDrawLine(rend,
					rect->x + ((float)rect->w / model->matrixSizeX * i),
					rect->y ,
					rect->x + ((float)rect->w / model->matrixSizeX * i),
					rect->y+rect->h)
					;
		}
		for (int j = 0; j < model->matrixSizeY; j++) {
			SDL_RenderDrawLine(rend,
				rect->x,
				rect->y + (int)(((float)rect->h / model->matrixSizeY * j)),
				rect->x + rect->w,
				rect->y +(int)(((float)rect->h / model->matrixSizeY * j)))
				;
		}
	}

	void onBuildingUpradge(Building* b) {
		if (b->getLevel() == 2) {
			sprites[b->getThread_id()]->setCurrentSkin("level_2");
			sprites[b->getThread_id()]->upradgeLevelText();
		}
		if (b->getLevel() == 3) {
			sprites[b->getThread_id()]->setCurrentSkin("level_3");
			sprites[b->getThread_id()]->upradgeLevelText();
		}
	}
	void onEntityDeath(Entity* e) {
		std::unique_lock<decltype(m_)> lock(m_);
	
		try {
			can_render = false;
			//ki kell törölni a választott egységek közül 
			try
			{
				if (selectedThreads.size() > 0) {
					//std::unordered_map<std::thread::id, Entity*>::iterator  it;
					//it = selectedThreads.find(std::this_thread::get_id());
					//if (it != selectedThreads.end()) {
						selectedThreads.erase(e->getThread_id());
					//}
				}
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
			}



			ThreadSafeMap<std::thread::id, std::shared_ptr<Sprite>>::const_iterator  it;
			it = sprites.find(e->getThread_id());
			if (it != sprites.end()) {
				onEntityDeathEvent(e);
				sprites[e->getThread_id()]->alive=false;
				sprites.erase(e->getThread_id());
			}
			//e = nullptr;
			can_render = true;
		//	updateEvent();
		}
		catch (std::exception e) {
			std::cerr<<e.what();
			std::cerr << "catch\n";
		}
	}
	void onEntityUpdate(Entity* e) {
		std::scoped_lock lock(m_);

		//std::cout << "thread " << this_id << "\n--------\n"
		//ThreadSafeMap<std::thread::id, std::shared_ptr<Sprite>>::const_iterator it;
		if (sprites.find(e->getThread_id()) != sprites.end()) {
			SDL_Rect* r = sprites[e->getThread_id()]->rect;
			r->x = rect->x + ((float)rect->w / model->matrixSizeX) * e->getX(); // % (int)((float)(rect->w+rect->x) / (model->matrixSizeX-1)));
			r->y = rect->y + ((float)rect->h / model->matrixSizeY) * e->getY();

			r->w = rect->w / model->matrixSizeX + 1;
			r->h = rect->h / model->matrixSizeY + 1;
			sprites[e->getThread_id()]->setRect(r,rect);
			//Utils::updateEvent();
		}
		if (selectedThreads.find(e->getThread_id()) != selectedThreads.end()) {
			onEntityUpdateEvent(e);
		}
		//passing entity to infopanel
	}
	
	void onEntitySpawn(Entity* e) {
		std::scoped_lock lock(m_);

		SDL_Rect* r = new SDL_Rect();
		r->x = rect->x + ((float)rect->w / model->matrixSizeX) * e->getX(); // % (int)((float)(rect->w+rect->x) / (model->matrixSizeX-1)));
		r->y = rect->y + ((float)rect->h / model->matrixSizeY) * e->getY();

		r->w = rect->w / model->matrixSizeX + 1;
		r->h = rect->h / model->matrixSizeY + 1;
		std::shared_ptr<Sprite> s = std::shared_ptr<Sprite>(new Sprite(spriteBank[e->getType()]));
		s->setEntity(e);
		s->setRect(r,rect);
		
		sprites.insert(std::pair<std::thread::id, std::shared_ptr<Sprite>>(e->getThread_id(), s));
		
		if (Building* b = dynamic_cast<Building*>(e)) {
			onBuildingUpradge(b);
		}
		Utils::updateEvent();


	}
	void renderEntities() {
		try {
			if (!can_render) return;
			ThreadSafeMap<std::thread::id, std::shared_ptr<Sprite>>::iterator it;
			for (it = sprites.m_begin();
				can_render&& &it!=nullptr&&
				 it != sprites.m_end()
				&& (*it).second != nullptr; ++it)
			{
				if(can_render)
					if ((*it).second != nullptr)
						(*it).second->render(rend);
				
			}
		}
		catch (std::exception e) {
			std::cout << e.what();
			std::cout << "failed to read value";
		}
	}
	virtual void triggerClickEnd(SDL_Point* p = nullptr) override {
		if (!visible) {return;}
		click = false;
	}
	void selectSprites(bool parSelect) {
		std::scoped_lock lock(m_);
		std::unordered_map<std::thread::id, Entity*>::iterator it;
		ThreadSafeMap<std::thread::id, Sprite*>::const_iterator its;
		int c = 0;
		for (it = selectedThreads.begin(); it != selectedThreads.end(); it++)
		{
			if (can_render) {
				if (sprites.find((*it).first) != sprites.m_end()) {
					sprites[(*it).first]->selected = parSelect;
				}
			}
		}
	}


	virtual void triggerClickStart(SDL_Point* p = nullptr)override {
		if (!visible) { return; }
		int y = (int)((float)(p->y - rect->y) / ((float)rect->h / model->matrixSizeY));
		int x = (int)((float)(p->x - rect->x) / ((float)rect->w / model->matrixSizeX));
		//std::cout << (int)((float)(p->x - rect->x) / ((float)rect->w / model->matrixSizeX)) << "-";
		//std::cout << (int)((float)(p->y - rect->y) / ((float)rect->h / model->matrixSizeY)) << "\n";
		click = true;
		std::vector<Entity*>::iterator it;
		std::cout << "click in view " << x << " " << y << "\n";
		selectSprites(false);
		selectedThreads = model->tableClickEvent(x, y).getMap();
		selectSprites(true);
		onTableClickEvent(model->matrix[x][y]);
	}
	int hover_x=0;
	int hover_y=0;
	bool can_build=false;
	virtual void mouseMove(SDL_Point* p = nullptr)override {
		if (!visible) { return; }
		//std::cout << "mmove
		////A KÉPLET
		//std::cout <<(int) ((float)(p->x - rect->x) / ((float)rect->w/model->matrixSizeX))<<"\n";
		///A KÉPLET
		hoverRect->x =rect->x+ ((float)rect->w / model->matrixSizeX)*(int)((float)(p->x - rect->x) / ((float)rect->w / model->matrixSizeX)); // % (int)((float)(rect->w+rect->x) / (model->matrixSizeX-1)));
		hoverRect->y = rect->y + ((float)rect->h / model->matrixSizeY) * (int)((float)(p->y - rect->y) / ((float)rect->h / model->matrixSizeY)); 

		hoverRect->w = rect->w/model->matrixSizeX+1;
		hoverRect->h = rect->h / model->matrixSizeY+1;


		int y = (int)((float)(p->y - rect->y) / ((float)rect->h / model->matrixSizeY));
		int x = (int)((float)(p->x - rect->x) / ((float)rect->w / model->matrixSizeX));

		if (hover_x != x || hover_y != y) {
			hover_x = x;
			hover_y = y;
			if (model->shopType != Entity::Type::Undefined) {

				int size;
				if (model->diff == 0)
				{
					size = model->metaMapEasy[model->shopType - 5][0].size;
				}
				else if (model->diff == 1)
				{
					size = model->metaMapMedium[model->shopType - 5][0].size;
				}
				else if (model->diff == 2)
				{
					size = model->metaMapHard[model->shopType - 5][0].size;
				}

				//int size = model->metaMap[model->shopType-5][0].size;
				shopRect.x = hoverRect->x-(int)hoverRect->w*(size/2);
				shopRect.y = hoverRect->y-(int)hoverRect->h*(size/2);
				shopRect.w = (int)hoverRect->w + (int)hoverRect->w * (size - size / 2);
				shopRect.h = (int)hoverRect->h + (int)hoverRect->h * (size - size / 2);
				can_build = model->canBuild(x, y, size);
			}
		}
		
	}
	virtual void setModel(Model* m)override {
		model = m;
		//model->setUpdateEventListener(std::bind(&TableContainer::renderEntities, this));
		using std::placeholders::_1;
		model->setUpdateEntityEventListener(std::bind(&TableContainer::onEntityUpdate, this, _1));
		model->setSpawnEntityEventListener(std::bind(&TableContainer::onEntitySpawn, this, _1));
		model->setOnEntityDeathEventListener(std::bind(&TableContainer::onEntityDeath, this, _1));
		model->setOnBuildingUpradgeEventListener(std::bind(&TableContainer::onBuildingUpradge, this, _1));
	}
	void setSpriteForType(Entity::Type t,Sprite* s) {
		spriteBank.insert(std::pair<Entity::Type, Sprite>(t,*s));
	}
};

