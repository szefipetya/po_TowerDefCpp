#pragma once
#include "PresentableObject.h"
class Label :	public Controller
{
public:
	Label():Controller() {

	}
	Label(int x, int y, int w, int h, int lvl = 1) :Controller(x, y, w, h, lvl) {
		border = false;
	}


};

