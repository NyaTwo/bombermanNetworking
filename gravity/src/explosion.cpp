#include "explosion.h"

Explosion::Explosion(sf::Vector2f pos)
{
	baseShape.setSize(sf::Vector2f(m_sizeX, m_sizeY));
	baseShape.setFillColor(sf::Color::Yellow);
	baseShape.setOrigin(m_sizeX / 2, m_sizeY / 2);
	baseShape.setPosition(pos);
	createExplosion();
}

void Explosion::update(float dt)
{
	if (m_explosionTimer > m_explosionIsOver) {
		explosionOver = true;
	}
	m_explosionTimer += dt;
}


void Explosion::render(sf::RenderWindow& window)
{
	for (auto e : m_explosion) {
		window.draw(e);
	}
}

bool Explosion::getIsOver()
{
	return explosionOver;
}

bool Explosion::collisionWithExplosion(entity& entity) 
{
	for (auto& e : m_explosion) {
		sf::Vector2f thisPos = e.getPosition();
		sf::Vector2f thatPos = entity.baseShape.getPosition();
		sf::Vector2f thisSize = e.getSize();
		sf::Vector2f thatSize = entity.baseShape.getSize();

		float dx = (thisPos.x + (thisSize.x / 2)) - (thatPos.x + (thatSize.x / 2));
		float dy = (thisPos.y + (thisSize.y / 2)) - (thatPos.y + (thatSize.y / 2));
		float distance = std::sqrt((dx * dx) + (dy * dy));

		if ((distance <= ((thisSize.x / 2) + (thatSize.x / 2)))) {
			return true;
			break;
		}
	}
	return exploded;
}

void Explosion::createExplosion()
{
	sf::RectangleShape explosionCenter = baseShape;
	sf::RectangleShape explosionTop = baseShape;
	sf::RectangleShape explosionDown = baseShape;
	sf::RectangleShape explosionLeft = baseShape;
	sf::RectangleShape explosionRight = baseShape;

	explosionTop.setPosition(baseShape.getPosition().x, baseShape.getPosition().y + m_sizeY);
	explosionDown.setPosition(baseShape.getPosition().x, baseShape.getPosition().y - m_sizeY);
	explosionLeft.setPosition(baseShape.getPosition().x - m_sizeX, baseShape.getPosition().y);
	explosionRight.setPosition(baseShape.getPosition().x + m_sizeX, baseShape.getPosition().y);
	m_explosion.push_back(explosionCenter);
	m_explosion.push_back(explosionTop);
	m_explosion.push_back(explosionDown);
	m_explosion.push_back(explosionLeft);
	m_explosion.push_back(explosionRight);
}
