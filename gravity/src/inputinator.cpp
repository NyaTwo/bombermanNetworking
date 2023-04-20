#include "inputinator.h"
inputinator::inputinator() {
	for (int i = 0; i < 5; i++) {
		predictions.push_back(sf::Vector2f(0, 0));
	}
}


void inputinator::predictInput(sf::Vector2f currentPosition)
{
	if (predictions.front() == sf::Vector2f(0,0)) {
		predictions.front() = currentPosition;
	}
	else {
		direction = currentPosition - predictions.front();
		for (auto p : predictions) {
			if (p == predictions.front()) {
				p = currentPosition;
			}
			p += direction;
		}
	}
}

bool inputinator::validateInput(sf::Vector2f pos)
{
	float misspredictionCheckX = predictions.at(1).x - pos.x;
	float misspredictionCheckY = predictions.at(1).y - pos.y;
	if ((misspredictionCheckX > -5 || misspredictionCheckX < 5 ) && (misspredictionCheckY > -5 || misspredictionCheckY < 5)) {
		return true;
	}
	return false;
}

void inputinator::handleMisprediction(sf::Vector2f pos)
{
	predictions.at(1) = pos;
}

sf::Vector2f inputinator::getCurrentPos()
{
	return predictions.at(1);
}
