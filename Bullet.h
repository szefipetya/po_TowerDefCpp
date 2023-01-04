#pragma once
#include "Sprite.h"
class Bullet : public Sprite
{
	Bullet() : Sprite() {

	}
	Bullet(Global* g):Sprite(g) {
	}
	//copy-constructor
	Bullet(const Sprite& s):Sprite(s) {

	}
	float ammoSpeed = 1.0f;//milliseconds to reach target
	SDL_Rect *sourceRect;
	SDL_Rect *targetRect;


};

