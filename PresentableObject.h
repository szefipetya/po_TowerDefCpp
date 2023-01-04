#include "Global.h"
#pragma once
#include<SDL.h>
#include<vector>
#include<stdio.h>
#include<iostream>
#include"Utils.h"
#include <SDL_image.h>
#include "Model.h"

class Button;
class Container;
class PresentableObject
{
public:
	virtual void setHover(bool l) {
		hover = l;
	}
	PresentableObject() {
		rect = new SDL_Rect();
		init();
	}
	PresentableObject(int x, int y, int w, int h) {
		rect = new SDL_Rect();
		rect->x = x+marginLeft;
		rect->y = y+marginTop;
		rect->w = w-marginRight*2;
		rect->h = h-marginBottom*2;
		init();
	}
	PresentableObject(int x, int y, int w, int h,int lvl) {
	
		rect = new SDL_Rect();
		rect->x = x + marginLeft;
		rect->y = y + marginTop;
		rect->w = w - marginRight * 2;
		rect->h = h - marginBottom * 2;
		level = lvl;
		init();
	}
	void init() {
		orientation = horizontal;
	}
protected:
	int marginTop = 20;
	int marginLeft = 20;
	int marginRight = 20;
	int marginBottom = 20;
	bool visible = true;
	bool border = true;
	bool hover = false;
	bool click = false;
	int flexSize = 1;
public:
	bool transparent = false;
	char* bgImgKey = (char*)"btnbg";
	//char* text = (char*)"-";
	int level=1;
	int fontSize = 15;
	Model *model;
	SDL_Rect *rect;
	Global* global = nullptr;
	Orientation orientation;
	SDL_Renderer* rend = nullptr;
	std::vector<PresentableObject*>objects;
public:
	void setMargins(int m) {
		rect->x = rect->x + (m-marginLeft);
		rect->y = rect->y + (m-marginTop);
		rect->w = rect->w - (m-marginRight) * 2;
		rect->h = rect->h - (m-marginBottom) * 2;
		marginTop = m;
			marginLeft = m;
			marginRight = m;
			marginBottom = m;
		updateObjectPositions();
	}
	virtual void setOrientaion(Orientation o) {
		orientation = o;
		updateObjectPositions();
	}
	void setBorder(bool l) {
		border = l;
	}
	void setRenderer(SDL_Renderer* rend) {
		this->rend = rend;
	}
	void setBackgroundImg(std::string name) {
	//	if (this->global->imagesTexture.find(name) != this->global->imagesTexture.end())
			bgImgKey = Utils::str_tochar(name);
			textureMode = false;
	}
	void setBackgroundTexture(Entity::Type type) {
		bgTexture = global->portraitTexture[type];
		textureMode = true;
	}
	void setTextureMode(bool l) {
		textureMode = l;
	}
	bool textureMode = false;
	SDL_Texture* bgTexture;
	void setGlobal(Global* g) {
		this->global = g;
		;
	}
	virtual void render() {
		SDL_RenderDrawRect(rend, rect);
	}
	/**/virtual Button* addButton(std::string text = "", int m = 10,int fontSize=-1) { return nullptr; };
	virtual Container* addContainer() { return nullptr; }
	virtual PresentableObject* add(std::string type, std::string text = {}, int m = 10, int fontSize = -1) { return nullptr; }
	virtual void updateObjectPositions() { };
	virtual void setFirstLevelMargins(int m) {
		for (PresentableObject* o : objects) {
			o->setMargins(m);
		}
	};
	virtual void triggerClickStart(SDL_Point* p = nullptr) {}
	virtual void triggerClickEnd(SDL_Point* p=nullptr) {}
	virtual void mouseMove(SDL_Point* p = nullptr) {	}
	virtual void renderBackgroundImg() {
		if(textureMode==false)
		SDL_RenderCopy(rend, this->global->imagesTexture[bgImgKey], NULL, rect);
		else {
			SDL_RenderCopy(rend, bgTexture, NULL, rect);
		}
	}
	//SETTERS
	virtual void setMarginsY(int m) {
		marginTop = marginBottom = m;
		updateObjectPositions();
	}
	virtual void setMarginsX(int m) {
		marginLeft = marginRight = m;
		updateObjectPositions();

	}
	virtual void setModel(Model* m) {
		model = m;
	}
	virtual void setFontSize(int s) {
		this->fontSize = s;
	}
	virtual void setVisible(bool l) {
		visible = l;
	}
	void setClick(bool l) {
		click = l;
	}
	void setFlexSize(int s) {
		flexSize = s;
	}
	//getters
	int	getFlexSize() {
		return flexSize;
	}
	bool isVisible() {
		return visible;
	}
	bool isClick() {
		return click;
	}
	int getMarginTop() {
		return marginTop;
	}
	int getMarginBottom() {
		return marginBottom;
	}
	int getMarginRight() {
		return marginRight;
	}
	int getMarginLeft() {
		return marginLeft;
	}
	~PresentableObject() {
		//SDL_DestroyTexture(this->bgTexture);
		std::vector<PresentableObject*>::iterator it = std::find(
			this->global->objectHitBoxRegister.begin(), this->global->objectHitBoxRegister.end(), this);
		if (it != this->global->objectHitBoxRegister.end()) {
			this->global->objectHitBoxRegister.erase(it);
		}
		for (int i = 0; i < objects.size(); i++) {
			delete objects[i];
		}
	}
};

