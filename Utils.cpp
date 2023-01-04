#include "Utils.h"
const int Utils::MIN_UPDATE_MS = 100;
SDL_Color Utils::White = { 255,255,255 };
SDL_Color Utils::Yellow = { 0xdd, 0xdd, 0x22 };
SDL_Color Utils::Red = { 0xff, 0x33, 0x33};
SDL_Color Utils::Black = { 0,0,0 };

std::atomic<long> Utils::lastUpdate = 0;