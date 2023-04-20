#include "entity.h"

bool entity::collisionCheck( entity& entity) const
{
	sf::Vector2f thisPos = baseShape.getPosition();
	sf::Vector2f thatPos = entity.baseShape.getPosition();
	sf::Vector2f thisSize = baseShape.getSize();
	sf::Vector2f thatSize = entity.baseShape.getSize();

	float dx = (thisPos.x + (thisSize.x / 2)) - (thatPos.x + (thatSize.x / 2));
	float dy = (thisPos.y + (thisSize.y / 2)) - (thatPos.y + (thatSize.y / 2));
	float distance = std::sqrt((dx * dx) + (dy * dy));

	return (distance <= ((thisSize.x / 2) + (thatSize.x / 2)));
}
