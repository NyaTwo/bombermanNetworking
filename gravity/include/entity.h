#pragma once 
#include "SFML/Graphics.hpp"
class entity{
public:
	bool collisionCheck(entity& entity) const;
	sf::RectangleShape baseShape {};
};