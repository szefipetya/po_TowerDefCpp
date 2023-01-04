#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include"ThreadSafeVector.h"
#include<string>
#include"Utils.h"
#include"Global.h"
#include"customtimer.h"
class CustomTimer;
class Animation {
public:

	Animation(std::string source, Global* g,SDL_Renderer *r) {
		global = g;
		sourceImgKey = source;
		//t = new CustomTimer();
		totalSurface = global->imagesSurface[source];
		rend = r;

	}
	Animation(const Animation& a) {
		this->FPS = a.FPS;
		name = a.name;
		global = a.global;
		sourceImgKey = a.sourceImgKey;
		frameRects = a.frameRects;
		frameSurfaces = a.frameSurfaces;
		frameTextures = a.frameTextures;
		totalSurface = a.totalSurface;

		t = new CustomTimer();
		t->setInterval<Animation>(Animation::nextFrame, this, a.timeout);
	}
	int timeout = 200;
	void setTimer(int ms) {
		timeout = ms;

	}
	Global* global;
	SDL_Renderer* rend;
	SDL_Surface* totalSurface;
	int FPS = 30;
	int index = 0;
	std::string name;
	std::string sourceImgKey;
	CustomTimer *t;
	std::vector<SDL_Rect*> frameRects;
	std::vector<SDL_Surface*> frameSurfaces;
	std::vector<SDL_Texture*> frameTextures;
	int frameCount = 0;
	long lastUpdate = 0;

	/*int horizontalCount=0;
	int verticalCount=0;
	int horizontalStartIndex = 0;
	int verticalStartIndex = 0;
	Orientation FrameOrientation;*/
	//methods
	void initFrames() {
		for (int i = 0; i < frameCount; i++) {
			frameSurfaces.push_back(SDL_CreateRGBSurface(NULL, frameRects[i]->w, frameRects[i]->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000));
			int a = SDL_BlitSurface(totalSurface, frameRects[i], frameSurfaces[i], NULL);
		}
	}
	void initTextures() {
		for (int i = 0; i < frameCount; i++) {
			frameTextures.push_back(SDL_CreateTextureFromSurface(rend, frameSurfaces[i]));
		}
	}
	void setup(int sourceHorizolntalFramesCount, int sourceVerticalFramesCount, Orientation frameOrientation = vertical,
		int frameCount=0 ,int horizontalstartIndex=0,int verticalStartIndex=0) {
		this->frameCount = frameCount;
		if (frameCount == 0) {
			if (frameOrientation == horizontal) { this->frameCount = sourceHorizolntalFramesCount; }
			else if (frameOrientation == vertical) { this->frameCount = sourceVerticalFramesCount; }
		}

		if (frameOrientation == vertical)
			for (int i = 0; i < this->frameCount; i++) {
				SDL_Rect* r = new SDL_Rect();
				r->y = totalSurface->h / sourceVerticalFramesCount * (i+ verticalStartIndex);
				r->x = totalSurface->w / sourceHorizolntalFramesCount * horizontalstartIndex;
				r->h = totalSurface->h / sourceVerticalFramesCount;
				r->w = totalSurface->w / sourceHorizolntalFramesCount;
				frameRects.push_back(r);
			}
		else if (frameOrientation == horizontal)
			for (int i = 0; i < this->frameCount; i++) {
				SDL_Rect* r = new SDL_Rect();
				r->y = totalSurface->h / sourceVerticalFramesCount * verticalStartIndex;
				r->x = totalSurface->w / sourceHorizolntalFramesCount * (i+ horizontalstartIndex);
				r->h = totalSurface->h / sourceVerticalFramesCount;
				r->w = totalSurface->w / sourceHorizolntalFramesCount;
				frameRects.push_back(r);
			}

		initFrames();
		initTextures();

	}
	void init() {
		
	}
	void nextFrame() {
		//SDL_FreeSurface(currentFrameSurface);
		if (!this->alive) { this->~Animation(); return; }
				try {
					index++;
					if (index >= frameRects.size())
						index = 0;
					Utils::updateEvent();

				}
				catch (std::exception ex) {

					std::cout << ex.what() << "probably \"this\" has been deleted by another process";
				}		
	}

	static void nextFrame(Animation* a) {
		a->nextFrame();
	}
	
	//getters
	std::string getName() {
		return name;
	}
	SDL_Texture* getCurrentSurface() {
		if (this == nullptr) return nullptr;
		if (!this->alive) { this->~Animation(); return nullptr; }
		if (index >= frameTextures.size())
			index = 0;
		return frameTextures[index];
	}
	//setters
	bool alive = true;
	void setFrameCount(int a) { frameCount = a; }
	void setGlobal(Global* g) {
		global = g;
	}
	//csak az app bezárásakor kéne meghívni, mert minden animation példány erre a 3 vektorra hivatkozik.
	void cleanUp() {
		for (SDL_Surface* t : frameSurfaces) {
			SDL_FreeSurface(t);
		}
		for (SDL_Texture* t : frameTextures) {
			SDL_DestroyTexture(t);
		}
	}
	~Animation() {
		t->stop();
	}
};