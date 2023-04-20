#pragma once
#include "input.hpp"
#include "client.hpp"
#include "bomb.h"
#include "entity.h"
static const int MAX_HP = 3;
enum class direction {
	none, up, down, left, right
};

class Player : public entity {
public:
	Player();
	void update(float dt, client& c, keyboard& k, std::vector<Bomb>& b);
	void getInput(keyboard& k);
	void moveThePlayer();
	void render(sf::RenderWindow& window);
	void setPlayerPosition(sf::Vector2f pos);
	void collidingWithBoxOrWall(keyboard& k);
	void bombExploded();
	void hurt();
	int  getHealth();
	sf::Vector2f getPlayerPosition();


private:
	bool				hasPlacedBomb = false;
	bool				gotHit = false;
	float				invulnerabilityTimer = 0;
	float				invulnerableOver = 5;
	int					hp = 0;
	float				moveDistance = 1.f;
	Interpolator		interpolator;
	direction			dir = direction::none;
};