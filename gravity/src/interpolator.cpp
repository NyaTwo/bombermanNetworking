#include "interpolator.h"

sf::Vector2f Interpolator::interpolate(sf::Vector2f& currPos, sf::Vector2f& nextPos, float t)
{
	currentPosition = currPos;
	nextPosition = nextPos;
	time = t;
	currentPosition.x = std::lerp(currentPosition.x, nextPosition.x, time);
	currentPosition.y = std::lerp(currentPosition.y, nextPosition.y, time);

	swapValuesInArray();
	return currentPosition;
}

void Interpolator::swapValuesInArray()
{
	std::swap(pastPositions[0], pastPositions[1]);
	std::swap(pastPositions[1], pastPositions[2]);
	std::swap(pastPositions[2], pastPositions[3]);
	std::swap(pastPositions[3], pastPositions[4]);
	pastPositions[4] = currentPosition;
}