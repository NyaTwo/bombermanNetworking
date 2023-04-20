#include "enemy.h"

enemy::enemy()
{
    baseShape.setFillColor(sf::Color::Red);
    baseShape.setSize(sf::Vector2f(40.f, 40.f));
    baseShape.setPosition(400.f, 120.f);
    baseShape.setOrigin(baseShape.getSize().x / 2, baseShape.getSize().y / 2);
}

void enemy::update(float dt, client& c)
{
    if (invulnerabilityTimer > invulnerableOver) {
        gotHit = false;
        invulnerabilityTimer = 0;
    }
    invulnerabilityTimer += dt;

    sf::Vector2f currentPos = baseShape.getPosition();
    sf::Vector2f nextPos = c.getPosOfDynamicEntities(1);
    baseShape.setPosition(inter.interpolate(currentPos, nextPos, c.getTime()));
}

void enemy::render(sf::RenderWindow& window)
{
    window.draw(baseShape);
}

void enemy::setEnemyPosition(sf::Vector2f pos)
{
    baseShape.setPosition(pos);
}

int enemy::getHealth()
{
    return hp;
}

void enemy::hurt()
{
    if (gotHit == false) {
        hp--;
        gotHit = true;
    }
}
