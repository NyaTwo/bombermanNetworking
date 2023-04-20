#pragma once
#include "SFML/Graphics.hpp"
#include "entity.h"
class Explosion : public entity {
public:
	Explosion() = default;
	Explosion(sf::Vector2f pos);
	void update(float dt);
	void render(sf::RenderWindow& window);
	bool getIsOver();
	bool collisionWithExplosion(entity& entity);
private:
	void createExplosion();
	bool				explosionOver = false;
	bool				exploded = false;
	int					m_explosionSize = 1;
	float				m_sizeX = 38.f;
	float				m_sizeY = 38.f;
	float				m_explosionTimer = 0.f;
	float				m_explosionIsOver = 1.f;
	std::vector<sf::RectangleShape> m_explosion = {};
};
