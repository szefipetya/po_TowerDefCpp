#pragma once
#include "Controller.h"
class Input : public Controller{
	public:
		Input() : Controller() {

		}
		Input(int x, int y, int w, int h, int lvl = 1) :Controller(x, y, w, h, lvl) {

		}
		virtual void init() {
			border = true;
		}
		void addItem() {

		}
		virtual void triggerClickEnd(SDL_Point* p = nullptr) override {
			if (!visible) {
				return;
			}
			for (int i = 0; i < clickEventListeners.size(); i++) {
				clickEventListeners[i](EventArgs(0));
			}

			click = false;
		}
		virtual void triggerClickStart(SDL_Point* p = nullptr)override {
			if (!visible) {
				return;
			}
			click = true;
		}
		virtual void mouseMove(SDL_Point* p = nullptr) {


		}
};

