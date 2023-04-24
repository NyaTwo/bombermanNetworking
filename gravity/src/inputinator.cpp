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
		
		for (auto&& prediction : predictions) {
			prediction.direction = currentPosition - predictions.front().position;
			if (prediction.position == predictions.front().position) {
				prediction.position = currentPosition;
				prediction.clientTick = clientTick;
			}
			prediction.position += prediction.direction;
			prediction.clientTick = clientTick + prediction.id;
		}
	}
}

bool inputinator::validateInput(sf::Vector2f pos, uint32 clientTick)
{
	for (auto&& prediction : predictions) {
		if (prediction.clientTick == clientTick) {
			float misspredictionCheckX = prediction.position.x - pos.x;
			float misspredictionCheckY = prediction.position.y - pos.y;
			if ((misspredictionCheckX > -5 || misspredictionCheckX < 5) && (misspredictionCheckY > -5 || misspredictionCheckY < 5)) {
				return true;
			}
		}
	}
	handleMisprediction(pos, clientTick);
	return false;
}

void inputinator::handleMisprediction(sf::Vector2f pos, uint32 clientTick)
{
	for (auto&& prediction : predictions) {
		if (prediction.clientTick == clientTick) {
			prediction.position = pos;
		}
	}
}
