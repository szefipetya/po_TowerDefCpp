#pragma once
#include "Controller.h"
class Button : public Controller{
public:
	Button() : Controller() {

	}
	Button(int x, int y, int w, int h, int lvl = 1) :Controller(x, y, w, h, lvl) {

	}
	
	
};

