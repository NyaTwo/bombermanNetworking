#include "box.h"

box::box()
{
	baseShape.setSize(sf::Vector2f(35.f, 35.f));
	baseShape.setOutlineThickness(2.f);
	baseShape.setOutlineColor(sf::Color::Black);
	baseShape.setFillColor(sf::Color(160, 32, 240));
	baseShape.setOrigin(baseShape.getSize().x / 2, baseShape.getSize().y / 2);
}

void box::render(sf::RenderWindow& window)
{
	window.draw(baseShape);
}

bool box::isDestroyed()
{
	return destroyed;
}

void box::destroy() {
	destroyed = true;
}


