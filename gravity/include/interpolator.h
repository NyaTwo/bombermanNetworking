#pragma once
#include "SFML/Graphics.hpp"

class Interpolator {
public:
	sf::Vector2f interpolate(sf::Vector2f& currPos, sf::Vector2f& nextPos, float t);
private:
	void swapValuesInArray();
	sf::Vector2f currentPosition = {};
	sf::Vector2f nextPosition = {};
	float		 time = 0;
	sf::Vector2f pastPositions[5];
};