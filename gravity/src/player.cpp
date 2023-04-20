#include "player.h"

Player::Player()
	:hp(MAX_HP)
{
    baseShape.setFillColor(sf::Color::Green);
    baseShape.setSize(sf::Vector2f(40.f, 40.f));

    baseShape.setPosition(400.f, 120.f);
    baseShape.setOrigin(baseShape.getSize().x /2 , baseShape.getSize().y /2);
}

void Player::update(float dt, client& c, keyboard& k, std::vector<Bomb>& b)
{
    getInput(k);
    moveThePlayer();
    if (k.pressed(sf::Keyboard::Space) && !hasPlacedBomb) {
        Bomb bomb(baseShape.getPosition());
        b.push_back(bomb);
        c.setStaticEntityPos(bomb.getPos());
        hasPlacedBomb = true;
    }
    if (invulnerabilityTimer > invulnerableOver) {
        gotHit = false;
        invulnerabilityTimer = 0;
    }
    invulnerabilityTimer += dt;
    c.setPlayerPos(baseShape.getPosition());

}

void Player::getInput(keyboard& k)
{
    if (k.down(sf::Keyboard::Key::W)) {
        dir = direction::up;
    }
    else if (k.down(sf::Keyboard::Key::S)) {
        dir = direction::down;
    }
    else if (k.down(sf::Keyboard::Key::D)) {
        dir = direction::right;
    }
    else if (k.down(sf::Keyboard::Key::A)) {
        dir = direction::left;
    }
    else {
        dir = direction::none;
    }
}

void Player::moveThePlayer()
{
    if (dir == direction::up ) {
        baseShape.move(0, -moveDistance);
    }
    else if (dir == direction::down)  {
        baseShape.move(0, moveDistance);
    }
    else if (dir == direction::right) {
        baseShape.move(moveDistance, 0);
    }
    else if (dir == direction::left) {
        baseShape.move(-moveDistance, 0);
    }
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(baseShape);
}

sf::Vector2f Player::getPlayerPosition()
{
    return baseShape.getPosition();
}

void Player::setPlayerPosition(sf::Vector2f pos)
{
    baseShape.setPosition(pos);
}

void Player::collidingWithBoxOrWall(keyboard& k)
{
    if (dir == direction::up) {
        baseShape.setPosition(baseShape.getPosition().x, baseShape.getPosition().y + moveDistance);
        
    }
    else if (dir == direction::down) {
        baseShape.setPosition(baseShape.getPosition().x, baseShape.getPosition().y - moveDistance);
    }
    else if (dir == direction::left) {
        baseShape.setPosition(baseShape.getPosition().x + moveDistance, baseShape.getPosition().y);
       
    }
    else if (dir == direction::right) {
        baseShape.setPosition(baseShape.getPosition().x - moveDistance, baseShape.getPosition().y);
    }
}

void Player::bombExploded()
{
    hasPlacedBomb = false;
}

void Player::hurt()
{
    if (gotHit == false) {
        hp--;
        gotHit = true;
    }
}

int Player::getHealth()
{
    return hp;
}
