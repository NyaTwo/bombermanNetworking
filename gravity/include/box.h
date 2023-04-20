#pragma once
#include "SFML/Graphics.hpp"
#include "entity.h"

class box : public entity {
public:
	box();
	void render(sf::RenderWindow& window);
	bool isDestroyed();
	void destroy();
private:
	bool destroyed = false;
};