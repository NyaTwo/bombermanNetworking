#pragma once
#include "SFML/Graphics.hpp"
//add client tick to the data
//add actual input on the keyboard
class inputinator {
public:
	inputinator();
	bool validateInput(sf::Vector2f pos);
	void handleMisprediction(sf::Vector2f pos);
	sf::Vector2f getCurrentPos();
	void predictInput(sf::Vector2f currentPosition);
private:
	std::vector<sf::Vector2f> predictions;
	sf::Vector2f direction;
};