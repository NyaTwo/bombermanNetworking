#pragma once
#include "sfml/Graphics.hpp"
#include "entity.h"

class wall : public entity {
public:
	wall();
	void render(sf::RenderWindow& window);
};