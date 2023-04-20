#include "wall.h"

wall::wall()
{
	baseShape.setFillColor(sf::Color(160, 82, 45));
	baseShape.setSize(sf::Vector2f(39.f, 39.f));
	baseShape.setOrigin(baseShape.getSize().x / 2, baseShape.getSize().y / 2);
}

void wall::render(sf::RenderWindow& window)
{
	window.draw(baseShape);
}
