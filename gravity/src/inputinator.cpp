#include "inputinator.h"
inputinator::inputinator() {
	for (int i = 0; i < 5; i++) {
		predictions.push_back(prediction(0,i, sf::Vector2f(0, 0), sf::Vector2f(0, 0)));
	}
}


void inputinator::predictInput(sf::Vector2f currentPosition, uint32 clientTick)
{
	if (predictions.front().position == sf::Vector2f(0,0)) {
		predictions.front().position = currentPosition;
		predictions.front().clientTick = clientTick;
	}
	else {
		direction = currentPosition - predictions.front().position;
		for (auto&& prediction : predictions) {
			if (prediction.position == predictions.front().position) {
				prediction.position = currentPosition;
				prediction.clientTick = clientTick;
			}
			prediction.position += direction;
			//p.clientTick = clientTick + 1;
		}
	}
}

bool inputinator::validateInput(sf::Vector2f pos, uint32 clientTick)
{
	float misspredictionCheckX = predictions.at(1).position.x - pos.x;
	float misspredictionCheckY = predictions.at(1).position.y - pos.y;
	if ((misspredictionCheckX > -5 || misspredictionCheckX < 5 ) && (misspredictionCheckY > -5 || misspredictionCheckY < 5)) {
		return true;
	}
	return false;
}

void inputinator::handleMisprediction(sf::Vector2f pos)
{
	predictions.at(1).position = pos;
}

sf::Vector2f inputinator::getCurrentPos()
{
	return predictions.at(1).position;
}
