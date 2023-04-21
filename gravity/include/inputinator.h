#pragma once
#include "SFML/Graphics.hpp"
#include <shared.hpp>
//add client tick to the data
//add actual input on the keyboard
class inputinator {
public:
	inputinator();
	bool validateInput(sf::Vector2f pos, uint32 clientTick);
	void handleMisprediction(sf::Vector2f pos);
	sf::Vector2f getCurrentPos();
	void predictInput(sf::Vector2f currentPosition, uint32 clientTick);
private:
	struct prediction {
		uint32 clientTick;
		uint32 id; //?
		sf::Vector2f position;
		sf::Vector2f direction;
	};
	std::vector<prediction> predictions;
	

	
};