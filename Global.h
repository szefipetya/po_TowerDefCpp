#pragma once
//#include "PresentableObject.h"
#include<map>
#include<SDL.h>
#include<SDL_ttf.h>
#include <vector>
#include<string>
#include<functional>
#include"Entity.h"
#include"EventArgs.h"
class PresentableObject;
class Global
{
public:
	std::vector<PresentableObject*> objectHitBoxRegister;
	std::map<std::string, TTF_Font*> fonts;
	std::map<SDL_Keycode, std::function<void(EventArgs)>> keyMap;
	std::map<std::string, SDL_Texture*> imagesTexture;
	std::map<std::string, SDL_Surface*> imagesSurface;
	std::map<Entity::Type, SDL_Texture*> portraitTexture;

	//std::vector<Window*> 
	Global() {
		objectHitBoxRegister = std::vector<PresentableObject*>();
		keyMap = std::map<SDL_Keycode, std::function<void(EventArgs)>>();
		fonts = std::map<std::string, TTF_Font*>();
		imagesTexture = std::map<std::string, SDL_Texture*>();
	}
	 

};

