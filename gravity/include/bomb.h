#pragma once
#include <SFML/Graphics.hpp>
#include "explosion.h"
#include "entity.h"

class Bomb{
public:
	Bomb(sf::Vector2f pos);
	void update(float dt);
	void render(sf::RenderWindow& window);
	bool getHasExploded();
	Explosion getExplosion();
	sf::Vector2f getPos();
private:
	sf::CircleShape		m_shape;
	bool				hasExploded = false;
	float				m_bombSize = 10.f;
	float				m_bombTimeToExplode = 3.f;
	float				m_explosionTimer = 0.f;
	Explosion			m_explosion;
};