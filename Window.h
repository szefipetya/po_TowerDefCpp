#pragma once
#include "Container.h"
class Window : public Container
{
public:
	Window(int x, int y, int w, int h, int lvl = 1) :Container(x, y, w, h, lvl) {
	}
	int dx=0;
	int dy=0;
	int z = 1;
	int getZ()const {
		return z;
	}
	void setZ(int a) {
		z = a;
	}
	char* text;
	virtual void triggerClickEnd(SDL_Point* p = nullptr) override {
		for (PresentableObject* o : objects) {
			if (SDL_PointInRect(p, o->rect)) {
				o->setClick(false);
				o->triggerClickEnd(p);
			}
		}
		click = false;
	}
	virtual void triggerClickStart(SDL_Point* p) override {
		for (PresentableObject* o : objects) {
			if (SDL_PointInRect(p, o->rect)) {
				o->setClick(true);
				o->triggerClickStart(p);
			}
		}

		if (p != nullptr) {
			dx = p->x - rect->x;
			dy = p->y - rect->y;
		}
		click = true;
	}
	virtual void mouseMove(SDL_Point* p)override {
		if (click) {
			rect->x = p->x - dx;
			rect->y = p->y - dy;
			updateObjectPositions();
			for (PresentableObject* o: objects)
			{
				o->updateObjectPositions();
			} 
		}
	}
	virtual void addHitBoxToRegister() override{
		this->global->objectHitBoxRegister.push_back(this);
	}
};

