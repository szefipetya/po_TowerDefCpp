#pragma once
#ifndef CONT_H
#define CONT_H
#include<SDL.h>
#include <string>
#include"PresentableObject.h"
#include<SDL_ttf.h>
#include<functional>
#include <stdlib.h>
#include <atomic>
#include "EventArgs.h"
#include <mutex>
#define _CRT_SECURE_NO_WARNINGS

class Controller : public PresentableObject {
public:
	Controller():PresentableObject() {
		init();
	}
	Controller(int x, int y, int w, int h, int lvl = 1) :PresentableObject(x, y, w, h, lvl) {
		init();
	}
	Controller(char* t, int m = 5):PresentableObject() {
		text = t;
		textLength = strlen(t);
		setMargins(m);
		init();
	}
	char* text;
	std::atomic<char*> queuedtext = (char*)"-";
	std::atomic_bool updateReady;
	int textLength=0;
	std::vector<std::function<void(EventArgs)>> clickEventListeners;
	//std::map<std::string,std::function<void(EventArgs)>> clickEventListeners;
	std::vector<std::function<void(EventArgs)>> EventHandlers;
	SDL_Texture* Message;
	SDL_Rect trect;
	SDL_Surface* surfaceMessage = nullptr;
	SDL_Color* col=&Utils::White;
	bool firstRender = true;
	bool autoFontMargin = true;
	float hoverFontSize = 14;
	Align textAlign = Auto;
	std::string iconImgKey = "";
	SDL_Rect iconRect;
	void addHitBoxToRegister() {
		this->global->objectHitBoxRegister.push_back(this);
	}
	void init() {
		//this->bgImgKey = (char*)"";
		setBorder(true);
	}
	void updateTextColor() {
		SDL_Color *col;
		if (click) col = &Utils::Black;
		else { col = &Utils::White; }
		if (this->col != col) {
			SDL_DestroyTexture(Message);
			if (orientation == horizontal) {
				surfaceMessage = TTF_RenderText_Blended(this->global->fonts["roboto"], this->text, *col); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
			}
			else if (orientation == vertical) {
				surfaceMessage = TTF_RenderText_Blended_Wrapped(this->global->fonts["roboto"], this->text, *col, rect->w);
			}
			Message = SDL_CreateTextureFromSurface(rend, surfaceMessage); //now you can convert it into a texture
			SDL_FreeSurface(surfaceMessage);
			this->col = col;
		}
	}

	void addEventlistener(std::string str, std::function<void(EventArgs)> f ) {
		if (str == "click") {
			clickEventListeners.push_back(f);
		}
	}
	virtual void renderIconImg() {
		SDL_RenderCopy(rend, this->global->imagesTexture[iconImgKey], NULL, &iconRect);
	}
	void setIconImg(std::string name) {
	//if (this->global->imagesTexture.find(name) != this->global->imagesTexture.end())
			iconImgKey = Utils::str_tochar(name);
	}
	virtual void render()override {
		if (!visible) return;
		//surfaceMessage = nullptr;
		if(!transparent)
		renderBackgroundImg();
		
		if (hover) {
			Utils::setRenderColor(rend, "yellow");

			SDL_RenderDrawRect(rend, this->rect);
			SDL_Rect brect;
			brect.x = this->rect->x + 1;
			brect.y = this->rect->y + 1;
			brect.w = this->rect->w -2;
			brect.h = this->rect->h -2;
			SDL_RenderDrawRect(rend, &brect);
			brect;
			brect.x = this->rect->x + 2;
			brect.y = this->rect->y +2;
			brect.w = this->rect->w - 4;
			brect.h = this->rect->h - 4;
			SDL_RenderDrawRect(rend, &brect);
		}
		if (click) {
			Utils::setRenderColor(rend, "yellow");
			SDL_RenderFillRect(rend, this->rect);
		}
		else {
			Utils::setRenderColor(rend, "white");
			if(border)SDL_RenderDrawRect(rend, this->rect);
		}
		updateTextColor();
		if (updateReady) {
			text =queuedtext.load();
			setText(text);
			updateReady = false;
		}
		firstRenderfn(rend,false);
		renderIconImg();
		SDL_RenderCopy(rend, Message, NULL, &trect); 
		SDL_SetRenderDrawColor(rend, 0xcc, 0x33, 0x33, 0xcc);
	}
	virtual void triggerClickEnd(SDL_Point *p=nullptr) override {
		if (!visible) {
			return;
		}
			for (int i = 0; i < clickEventListeners.size(); i++) {
				clickEventListeners[i](NULL);
			}
			click = false;
			Utils::updateEvent(true);
	}
	virtual void triggerClickStart(SDL_Point* p = nullptr)override {
		if (!visible) {
			return;
		}
			click = true;
			Utils::updateEvent(true);

	}
	virtual void mouseMove(SDL_Point* p = nullptr) {
		
	
	}
	virtual void setMarginsX(int m)override {
		
	}
	virtual void updateObjectPositions() override{
		setTextAlign(textAlign);
	}
	//special methods
	void setTextAlign(Align m) {
		autoFontMargin = false;
		textAlign = m;
		if (text == nullptr) return;
		if (textAlign == Auto|| textAlign ==Center) {
			if (orientation == horizontal) {
				textLength = strlen(text);//sizeof(*text) / sizeof(char);
				trect.y = rect->y + rect->h / 2 - fontSize;
				trect.w = textLength * fontSize;
				trect.h = fontSize * 2;

				if (iconImgKey == "")
					trect.x = rect->x + rect->w / 2 - textLength * fontSize / 2;
				else {
					trect.x = rect->x + rect->w / 2 - ((textLength * fontSize) / 2)+fontSize;
					iconRect.x = trect.x - fontSize*3; //rect->x + rect->w / 2 - ((textLength * fontSize) / 2)+fontSize*3;
					iconRect.y = rect->y + rect->h / 2 - fontSize;
					iconRect.w = fontSize*2;
					iconRect.h = fontSize * 2;
				}
			}
			else if (orientation == vertical) {
				textLength = strlen(text);//sizeof(*text) / sizeof(char);
				trect.y = rect->y+ rect->h / 2 - textLength * fontSize / 2;
				trect.x = rect->x + rect->w / 2 - (fontSize / 2 + 1);
				trect.h = textLength * fontSize;
				trect.w = fontSize;
			}
		}
		firstRenderfn(this->rend, true);
	}
	void inline firstRenderfn(SDL_Renderer* rend,bool must)  {
		if (firstRender||must) {
			
			SDL_DestroyTexture(Message);
			hoverFontSize = fontSize * 1.2;
			if (this->rend == nullptr) { this->rend = rend; }
			if (this->text != nullptr) {
				if (orientation == horizontal) {
					surfaceMessage = TTF_RenderText_Blended(this->global->fonts["roboto"], this->text, Utils::White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_GetError();
				}
				else if (orientation == vertical) {
					surfaceMessage = TTF_RenderText_Blended_Wrapped(this->global->fonts["roboto"], this->text, Utils::White, rect->w);
				}
			}
			else {
			}
			Message = SDL_CreateTextureFromSurface(rend, surfaceMessage); //now you can convert it into a texture
			SDL_FreeSurface(surfaceMessage);
			firstRender = false;
			//setTextAlign(textAlign);

		}
	}
	void setText(char* t,SDL_Renderer * rend) {
		text = t;
		setTextAlign(textAlign);
	}
	void setText(char* t) {
		text = t;
		setTextAlign(textAlign);
	}
	
	void updateText(EventArgs e) {
			std::string s = std::to_string(e.data);
			char* t = Utils::str_tochar(s);
			queuedtext = t;
			updateReady = true;
			Utils::updateEvent(true);
	}

};
#endif // !APP_H