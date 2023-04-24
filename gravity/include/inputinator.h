#pragma once
#include "SFML/Graphics.hpp"
#include <shared.hpp>
//add client tick to the data
//add actual input on the keyboard
class inputinator {
public:
	inputinator();
	bool validateInput(sf::Vector2f pos, uint32 clientTick);
	void predictInput(sf::Vector2f currentPosition, uint32 clientTick);
private:
	void handleMisprediction(sf::Vector2f pos, uint32 clientTick);
	struct prediction {
		uint32 clientTick;
		uint32 id; //?
		sf::Vector2f position;
		sf::Vector2f direction;
	};
	std::vector<prediction> predictions;
	

	
};