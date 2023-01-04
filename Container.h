#pragma once
#ifndef CONTAINER_H
#define CONTAINER_H
#include"PresentableObject.h"
#include<SDL.h>
#include <vector>
#include "Button.h"
#include "Label.h"
#include"Input.h"
#include <numeric>
#pragma warning(disable : 4996);

class Container : public PresentableObject
{
public:
	//Orientation orientation=horizontal;
	Container():PresentableObject(){}
	Container(int x,int y,int w,int h,int lvl=1):PresentableObject(x,y,w,h,lvl) {
	
	}
	
	virtual void render() override {
		if (!visible) return;
		Utils::setRenderColor(rend, "black");	//SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, 0xcc);
		if (!transparent) { 
			SDL_RenderFillRect(rend, this->rect); renderBackgroundImg();
		}	
		if (border) { 
			Utils::setRenderColor(rend, "white"); 
			SDL_RenderDrawRect(rend, this->rect); 
		}
		for (PresentableObject* o : objects) {
			o->render();
		}
	}
	virtual PresentableObject* add(std::string type, std::string text = {}, int m = 10, int fontSize = -1) override{
		if (type == "container" || type == "Container"||type=="c") {
			PresentableObject* c = new Container(
				this->rect->x + getMarginLeft(),
				this->rect->y + getMarginTop(),
				this->rect->w - getMarginRight(),
				this->rect->h - getMarginBottom(),
				this->level + 1
			);
			c->setGlobal(global);
			c->setMargins(m);
			c->setRenderer(rend);
			c->setModel(model);
			objects.push_back(c);
			updateObjectPositions();
			return c;
		}
		else if (type == "button" || type == "Button"||type=="b") {
			Button* b = new Button(
				this->rect->x + getMarginLeft(),
				this->rect->y + getMarginTop(),
				this->rect->w - getMarginRight(),
				this->rect->h - getMarginBottom(),
				this->level + 1
			);
			b->setGlobal(global);
			b->addHitBoxToRegister();
			b->setRenderer(rend);
			b->setModel(model);
			b->setFontSize(this->fontSize);
			b->setText(Utils::str_tochar(text));
			b->setMargins(m);
			if (fontSize != -1)
				b->setFontSize(fontSize);
			objects.push_back(b);
			updateObjectPositions();
			return b;
		}
		else if (type == "input" || type == "Input" || type == "i") {
			Input* b = new Input(
				this->rect->x + getMarginLeft(),
				this->rect->y + getMarginTop(),
				this->rect->w - getMarginRight(),
				this->rect->h - getMarginBottom(),
				this->level + 1
			);
			b->setGlobal(global);
			b->addHitBoxToRegister();
			b->setRenderer(rend);
			b->setModel(model);
			b->setFontSize(this->fontSize);
			b->setText(Utils::str_tochar(text));
			b->setMargins(m);
			if (fontSize != -1)
				b->setFontSize(fontSize);
			objects.push_back(b);
			updateObjectPositions();
			return b;
		}
		else if (type == "label" || type=="l") {
			Label* l = new Label(0,0,0,0,this->level + 1);
			l->setGlobal(global);
			l->setRenderer(rend);
			l->setModel(model);
			l->text = Utils::str_tochar(text);
			l->setMargins(m);
			if (fontSize != -1)
				l->setFontSize(fontSize);
			objects.push_back(l);
			updateObjectPositions();
			return l;
		}
	}
	virtual void updateObjectPositions() override{
			int i = 0;
			int len = objects.size();
			int flexSum = 0; //= std::accumulate(objects.begin(), objects.end(), 0);
			for (PresentableObject* o : objects) {
				flexSum += o->getFlexSize();
			}
			int currentFlex = 0;
		if (orientation == horizontal) {
			for (PresentableObject* o : objects) {
				o->rect->x = currentFlex * ((float)this->rect->w / flexSum)+o->getMarginLeft()*o->level+this->rect->x-o->getMarginLeft()*level;
				o->rect->y = ((float)this->rect->y)+o->getMarginTop();
				o->rect->w = (o->getFlexSize()*(float)this->rect->w / flexSum) - o->getMarginRight() * 2;
				o->rect->h = ((float)this->rect->h) - o->getMarginBottom()*2;
				o->updateObjectPositions();
				currentFlex+=o->getFlexSize();
			}
		}
		if (orientation == vertical) {
			for (PresentableObject* o : objects) {
				o->rect->y = currentFlex * ((float)this->rect->h / flexSum) + o->getMarginTop() * o->level+ this->rect->y-o->getMarginTop()*level;
				o->rect->x = ((float)this->rect->x) + o->getMarginLeft();
				o->rect->h = (o->getFlexSize()*(float)this->rect->h / flexSum) - o->getMarginBottom() * 2;
				o->rect->w = ((float)this->rect->w) - o->getMarginRight() * 2;
				currentFlex+=o->getFlexSize();
				o->updateObjectPositions();	
			}
		}
	}
	virtual Button* addButton(std::string text = {}, int m = 10,int fontSize=-1)override {
			Button* b = new Button(
				this->rect->x + getMarginLeft(),
				this->rect->y + getMarginTop(),
				this->rect->w - getMarginRight(),
				this->rect->h - getMarginBottom(),
				this->level + 1
			);
			b->setGlobal(global);
			b->addHitBoxToRegister();
			b->setRenderer(rend);
			b->setModel(model);
			b->setFontSize(this->fontSize);
			b->setText(Utils::str_tochar(text));
			b->setMargins(m);
			if(fontSize!=-1)
				b->setFontSize(fontSize);
			objects.push_back(b);
			b->updateObjectPositions();
		updateObjectPositions();
		return b;
	}
	virtual void setFontSize(int s) override {
		this->fontSize = s;
		for (PresentableObject* p : objects) {
			p->fontSize = s;
		}
	}
	virtual Container* addContainer(int m=10) {
		Container* c = new Container(
				this->rect->x + getMarginLeft(),
				this->rect->y + getMarginTop(),
				this->rect->w - getMarginRight(),
				this->rect->h - getMarginBottom(),
				this->level + 1
			);
			c->setGlobal(global);
			c->setRenderer(rend);
			c->setModel(model);
			c->setMargins(m);

			objects.push_back(c);
			updateObjectPositions();
			return c;
	}
	void setVisible(bool l) {
		visible = l;
		for (PresentableObject* o : objects) {
			o->setVisible(l);
		}
	}
	virtual void addHitBoxToRegister() {
	}
};
#endif
