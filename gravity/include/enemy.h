#pragma once
#include "entity.h"
#include "bomb.h"
#include "interpolator.h"
#include "client.hpp"
class enemy : public entity {
public:
	enemy();
	void update(float dt, client& c);
	void render(sf::RenderWindow& window);
	void setEnemyPosition(sf::Vector2f pos);
	int  getHealth();
	void hurt();
private:
	int hp = 3;
	bool gotHit = false;
	float invulnerabilityTimer = 0;
	float invulnerableOver = 5;
	Interpolator inter;
};