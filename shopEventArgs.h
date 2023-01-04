#pragma once
#include "EventArgs.h"
#include "Entity.h"
class ShopEventArgs :
	public EventArgs
{
public:
	Entity::Type shopType;
	ShopEventArgs(Entity::Type type):EventArgs() {
		shopType = type;
	}
};

