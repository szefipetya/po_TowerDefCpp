#pragma once
#include <map>
#include<string>
#include<atomic>
#include<SDL_ttf.h>
#include "Entity.h"
#include<string.h>
#include<cstdio>
#include<cstdlib>
#include<random>
#include<iostream>
enum Align {
	Auto=-1,Left=-2,Right=-3,Center=-1
};
enum Orientation {
	horizontal,vertical
};
class Utils
{

	public:
		static const int MIN_UPDATE_MS;
		static SDL_Color White;
		static SDL_Color Yellow;
		static SDL_Color Black;
		static SDL_Color Red;
		static void setRenderColor(SDL_Renderer* rend, std::string str,int alpha=0xaa) {
			if (str == "red") {
				SDL_SetRenderDrawColor(rend, 0xff, 0x33, 0x33, alpha);
			}
			if (str == "orange") {
				SDL_SetRenderDrawColor(rend, 0xff, 0xcc, 0x55, alpha);
			}
			if (str == "white") {
				SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, alpha);
			}
			if (str == "black") {
				SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, alpha);
			}
			if (str == "select") {
				SDL_SetRenderDrawColor(rend, 0x44, 0xff, 0x22, alpha);
			}
			if (str == "green") {
				SDL_SetRenderDrawColor(rend, 0x00, 0xff, 0x00, alpha);
			}
			if (str == "yellow") {	SDL_SetRenderDrawColor(rend, 0xdd, 0xdd, 0x22, alpha);}
			if (str == "gray") {	SDL_SetRenderDrawColor(rend, 0x88, 0x88, 0x88, alpha);}
			if (str == "grey") {	SDL_SetRenderDrawColor(rend, 0x88, 0x88, 0x88, alpha);}

		}
		static char* str_tochar(std::string str) {
			char* out = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), out);
			out[str.size()] = '\0';
			return out;
		}
		static std::atomic<long> lastUpdate;
		static void updateEvent() {
			long current = SDL_GetTicks();
			if (current - lastUpdate > Utils::MIN_UPDATE_MS) {
				SDL_Event ev;
				ev.type = SDL_USEREVENT;
				SDL_PushEvent(&ev);
				lastUpdate = current;
			}
		}
		static void updateEvent(bool l) {
			if (l) {
				long current;
			current = SDL_GetTicks();
			SDL_Event ev;
			ev.type = SDL_USEREVENT;
			SDL_PushEvent(&ev);
			lastUpdate = current;
		}
		}
		static char* intToChar(int n) {
			char* str = new char[20]; //you will have to think about 
									 //the size yourself
			_itoa(n, str, 10);
			return str;
		}
		static char* EntityTypeToChars(Entity::Type t) {
			switch (t) {
			case Entity::Type::Orc: return str_tochar("Orc"); break;
			case Entity::Type::Economic: return str_tochar("Economic"); break;
			case Entity::Type::Turret: return str_tochar("Mage"); break;
			case Entity::Type::Laser: return str_tochar("Laser Mage"); break;
			case Entity::Type::Ogre: return str_tochar("Ogre"); break;
			case Entity::Type::Goblin: return str_tochar("Goblin"); break;
			case Entity::Type::Skeleton: return str_tochar("Death King"); break;
			case Entity::Type::Artillery: return str_tochar("Artillery"); break;
			default: return str_tochar("Other");
			}
		}

		static int random(int min,int max)
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [1, 6]
			return dist(rng);
		}
};

