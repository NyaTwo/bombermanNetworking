#pragma once
#include "SFML/Graphics.hpp"
#include "box.h"
#include "wall.h"

struct {
	float	top = 120.f;
	float	bottom = 600.f;
	float	left = 400.f;
	float	right = 880.f;
} gameArea;
class gameMap {
public:
	void drawMap(std::vector <box>& boxes, std::vector<wall>& walls);
	void placesBoxesOnTheMap(std::vector<box>& boxes);
	void setBoxPosition(unsigned int i, sf::RectangleShape& box);
	void drawInnerWalls(std::vector<wall>& walls);
	void setInnerWallsPositions(unsigned int i, sf::RectangleShape& wall);
	void drawOuterWalls();
	void setOuterWallPositions(unsigned int i);
	void render(sf::RenderWindow& window);
private:
	sf::RectangleShape outerWalls[56]; //walls around the game area
	sf::RectangleShape innerWalls[37]; //indestructuble walls inside the game area
	unsigned int amountOfBoxes = 121;
	float spaceFromOuterWalls = 40.f;
	float spaceBetweenInnerWalls = 80.f;
	float startingPosX = 80.f;
	float startingPosY = 120.f;
	float distanceBetweenBoxes = 40.f;
	float row = 0;
};