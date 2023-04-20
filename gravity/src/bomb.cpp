#include "bomb.h"

Bomb::Bomb(sf::Vector2f pos)
{
	m_shape.setRadius(m_bombSize);
	m_shape.setFillColor(sf::Color::Black);
	m_shape.setOrigin(m_bombSize, m_bombSize);
	m_shape.setPosition(pos);
}

void Bomb::update(float dt)
{
	if (m_explosionTimer > m_bombTimeToExplode) {
		m_explosion = Explosion(m_shape.getPosition());
		hasExploded = true;
	}
	m_explosionTimer += dt;
}

void Bomb::render(sf::RenderWindow& window)
{
	window.draw(m_shape);
}

bool Bomb::getHasExploded()
{
	return hasExploded;
}

Explosion Bomb::getExplosion()
{
	return m_explosion;
}

sf::Vector2f Bomb::getPos()
{
	return m_shape.getPosition();
}
