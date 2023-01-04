#pragma once
#include <iostream>
#include <SDL.h>
#include "Animation.h"
#include "customtimer.h"
#include "ThreadSafeMap.h"
#include "Turret.h"
class Sprite {
public:
	enum EntitySize{_1x1,_3x3,_2x3};
	Sprite() {

	}
	Sprite(Global* g) {
		global = g;
	}
	//copy-constructor
	Sprite(const Sprite &s){
		for(std::pair<Entity::Status,Animation *> pair: s.getAnimations())
		{

			this->animations.insert(std::pair<Entity::Status, Animation*>
				(std::pair<Entity::Status,Animation*>(pair.first,new Animation(*pair.second))));
			//this->animations.end()->second->t->setInterval<Animation>(Animation::nextFrame, this->animations.end()->second, 300);

		} 
		skinTextures = s.skinTextures;
		global = s.global;
	}
	Global* global;
	ThreadSafeMap<Entity::Status, Animation*> animations;
	Animation* currentAnimation;
	SDL_Rect* rect;
	SDL_Rect recthp;
	SDL_Rect rectmaxhp;
	SDL_Rect rectGround;
	bool selected=false;
	Entity* entity;
	bool alive = true;
	//methods
	SDL_Renderer* rend;
	void render(SDL_Renderer* rend) {
		if (this->rend != rend) {
			this->rend = rend;
		}
		if (this == nullptr) { return; }
		if (!this->alive) {
			this->~Sprite();
			return;
		}
		if (entity == nullptr)return;
	
		if (selected) {

			Utils::setRenderColor(rend, "select");
		}
		else {
			Utils::setRenderColor(rend, "gray");
		}
			SDL_RenderDrawRect(rend, &rectGround);
			if (entity == nullptr) return;

	//if (animations.find(entity->getStatus()) != animations.end()) {
		currentAnimation = animations[e_recentStatus];

		if(currentSkin != nullptr) {
			SDL_RenderCopy(rend, currentSkin, NULL, rect);
		}
		else if (currentAnimation) {
			 SDL_RenderCopy(rend, animations[e_recentStatus]->getCurrentSurface(), NULL, rect);
		}
		Utils::setRenderColor(rend, "red");
		SDL_RenderFillRect(rend, &rectmaxhp);
		Utils::setRenderColor(rend, "green");
		SDL_RenderFillRect(rend, &recthp);
		if (levelTextTexture != nullptr) {
			SDL_RenderDrawRect(rend, &levelRect);
			SDL_RenderCopy(rend, levelTextTexture, NULL, &levelRect);
		}
		if (e_hasTarget) {
			SDL_RenderDrawLine(rend, this->rect->x + this->rect->w / 2
				, this->rect->y + this->rect->h / 2
				, target_x
				, target_y);
		}
		//}
	}
	void addAnimation(Entity::Status s,Animation* a) {
		animations.insert(std::pair<Entity::Status, Animation*>(s, a));
	}
	std::map<std::string,SDL_Texture*> skinTextures;
	SDL_Texture* currentSkin=nullptr;
	void addSkin(std::string key,std::string globalImgKey) {
		skinTextures.insert(std::pair<std::string, SDL_Texture*>(key, global->imagesTexture[globalImgKey]));
	}
	void setCurrentSkin(std::string key){
		std::cout << "current skin is set!!!\n";
		currentSkin = skinTextures[key];
	}
	//getters
	const ThreadSafeMap<Entity::Status, Animation*> const getAnimations() const {
		const ThreadSafeMap<Entity::Status, Animation*> copy = animations;
		return copy;
	}
	//setters

	int size = 1;
	float virtual_scale = 1;
	void setEntity(Entity* e) {
		entity = e;
		size = entity->getSize();
		virtual_scale = entity->getVirtualScale();

		if (Building* b = dynamic_cast<Building*>(entity))
		{
		
				SDL_DestroyTexture(levelTextTexture);

				levelTextSurface = TTF_RenderText_Blended(this->global->fonts["roboto"], Utils::intToChar(b->getLevel()), Utils::White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
				levelTextTexture = SDL_CreateTextureFromSurface(this->rend, levelTextSurface); //now you can convert it into a texture
				SDL_FreeSurface(levelTextSurface);
			
		}
	}
	void setGlobal(Global* g) {
		global = g;
	}
	Entity::Status e_recentStatus=Entity::Status::idle;
	int e_recentLevel = 1;
	SDL_Texture* levelTextTexture;
	SDL_Surface* levelTextSurface = nullptr;
	SDL_Rect levelRect;
	void upradgeLevelText() {
		if (Building* b = dynamic_cast<Building*>(entity))
		{
			if (e_recentLevel != b->getLevel()) {
				e_recentLevel = b->getLevel();
				SDL_DestroyTexture(levelTextTexture);
				levelTextSurface = TTF_RenderText_Blended(this->global->fonts["roboto"], Utils::intToChar(e_recentLevel), Utils::White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
				levelTextTexture = SDL_CreateTextureFromSurface(this->rend, levelTextSurface); //now you can convert it into a texture
				SDL_FreeSurface(levelTextSurface);

			}
		}


	}

	void setRect(SDL_Rect* r,SDL_Rect* tableRect) {

		e_recentStatus = entity->getStatus();
		
		rect = r;
		if (size > 1) {
			rect->x -= (size / 2) * rect->w;
			rect->w = size * rect->w;
			rect->y -= size / 2 * (rect->h);
			rect->h = size * rect->h;
		}
		rectGround = *rect;
		//nagyobb ogre

		//rect->x -= (size / 2) * rect->w;
		//rect->w = size * rect->w;
	
		rect->y -= ((size/2<1?1:size/2)*rect->h);
		rect->h *=2;
		//scaleing
		if (virtual_scale != 1) {
			rect->x -= rect->w * (virtual_scale-1) / 2;
			rect->w += rect->w * (virtual_scale - 1);
			rect->y -= rect->h * (virtual_scale - 1) - (virtual_scale - 1) / 2 * rect->h / 2;
			rect->h += rect->h * (virtual_scale - 1);
		}
		

		rectmaxhp.w = rect->w * 0.8;
		rectmaxhp.x = rect->x+0.1*rect->w;
		rectmaxhp.y = rect->y-rect->h*0.15;
		rectmaxhp.h = 3;

		recthp.w = rect->w * 0.8;
		recthp.x = rect->x + 0.1 * rect->w;
		recthp.y = rect->y - rect->h * 0.15;
		recthp.h = 3;
		recthp.w *= (float)entity->getHp()/(float)entity->getMaxHp();

		levelRect.w = 14;
		levelRect.x = rectmaxhp.x + rectmaxhp.w / 2 - levelRect.w/2;
		levelRect.y = rectmaxhp.y - 25;
		levelRect.h = 20;
		
		if (Building* b = dynamic_cast<Building*>(entity))
		{
			if (Unit* target=b->getTarget()) {
				e_hasTarget = true;
				std::cout << "x: " << target->getX()<<std::endl;
				std::cout << "y: " << target->getY()<<std::endl;
				target_x = tableRect->x+ this->rectGround.w/size * (target->getX());
				target_y =  tableRect->y+this->rectGround.h/size * (target->getY());
			}
			else {
				e_hasTarget = false;
			}
		}
	}
	bool e_hasTarget = false;;
	int target_x;
	int target_y;

	~Sprite() {
		alive = false;
		//std::cout << "sprite destructed\n";
		//std::thread t([=]() {
			for (std::pair< Entity::Status, Animation* > par : animations.getMap()) {
				if (par.second != nullptr) {
					par.second->alive = false;
				}
			}
		//	});
		
		
	}
};