#include "gameMap.h"

void gameMap::drawMap(std::vector<box>& boxes, std::vector<wall>& walls)
{
	drawOuterWalls();
	drawInnerWalls(walls);
	placesBoxesOnTheMap(boxes);
}

void gameMap::placesBoxesOnTheMap(std::vector<box>& boxes)
{
	for (unsigned int i = 0; i < amountOfBoxes; i++) {
		box b;
		setBoxPosition(i, b.baseShape);
		boxes.push_back(b);
	}
}

void gameMap::setBoxPosition(unsigned int i, sf::RectangleShape& box)
{
	if (i < 9) {
		box.setPosition(gameArea.left + startingPosX + i * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 14) {
		row = 1;
		box.setPosition(gameArea.left + startingPosX + (i - 9) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 27) {
		row = 2;
		box.setPosition(gameArea.left + (i - 14) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 34) {
		row = 3;
		box.setPosition(gameArea.left + (i - 27) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 47) {
		row = 4;
		box.setPosition(gameArea.left + (i - 34) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 54) {
		row = 5;
		box.setPosition(gameArea.left + (i - 47) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 67) {
		row = 6;
		box.setPosition(gameArea.left + (i - 54) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 74) {
		row = 7;
		box.setPosition(gameArea.left + (i - 67) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 87) {
		row = 8;
		box.setPosition(gameArea.left + (i - 74) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 94) {
		row = 9;
		box.setPosition(gameArea.left + (i - 87) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 107) {
		row = 10;
		box.setPosition(gameArea.left + (i - 94) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 112) {
		row = 11;
		box.setPosition(gameArea.left + startingPosX + (i - 107) * (distanceBetweenBoxes * 2), startingPosY + distanceBetweenBoxes * row);
	}
	else if (i < 122) {
		row = 12;
		box.setPosition(gameArea.left + startingPosX + (i - 112) * distanceBetweenBoxes, startingPosY + distanceBetweenBoxes * row);
	}
}

void gameMap::drawInnerWalls(std::vector<wall>& walls)
{
	for (unsigned int i = 0; i < 37; i++) {
		wall w;
		setInnerWallsPositions(i, w.baseShape);
		walls.push_back(w);
	}
}

void gameMap::setInnerWallsPositions(unsigned int i, sf::RectangleShape& wall)
{	
	if (i < 7) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + i * spaceBetweenInnerWalls, gameArea.top + spaceFromOuterWalls);
	}
	else if (i > 6 && i < 13) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + (i - 7) * spaceBetweenInnerWalls, gameArea.top * 2);
	}
	else if (i > 12 && i < 19) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + (i - 13) * spaceBetweenInnerWalls, gameArea.top * 3 - spaceFromOuterWalls);
	}
	else if (i > 18 && i < 25) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + (i - 19) * spaceBetweenInnerWalls, gameArea.top * 4 - spaceBetweenInnerWalls);
	}
	else if (i > 24 && i < 31) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + (i - 25) * spaceBetweenInnerWalls, gameArea.bottom - gameArea.top);
	}
	else if (i > 30) {
		wall.setPosition(gameArea.left + spaceFromOuterWalls + (i - 31) * spaceBetweenInnerWalls, gameArea.bottom - spaceFromOuterWalls);
	}
}

void gameMap::drawOuterWalls()
{
	for (unsigned int i = 0; i < 56; i++) {
		outerWalls[i].setFillColor(sf::Color(160, 82, 45));
		outerWalls[i].setSize(sf::Vector2f(40.f, 40.f));
		outerWalls[i].setOrigin(outerWalls[i].getSize().x / 2, outerWalls[i].getSize().y / 2);
		setOuterWallPositions(i);
	}
}

void gameMap::setOuterWallPositions(unsigned int i)
{
	float outerWallTop = gameArea.top - outerWalls[i].getSize().y;
	float outerWallBottom = gameArea.bottom + outerWalls[i].getSize().y;
	float outerWallLeft = gameArea.left - outerWalls[i].getSize().x;
	float outerWallRight = gameArea.right + outerWalls[i].getSize().x;
	if (i < 14) {
		outerWalls[i].setPosition(outerWallLeft + i * outerWalls[i].getSize().x, outerWallTop);
	}
	else if (i > 13 && i < 28) {
		outerWalls[i].setPosition(outerWallLeft, outerWallTop + (i - 13) * outerWalls[i].getSize().y);
	}
	else if (i > 27 && i < 41) {
		outerWalls[i].setPosition(outerWallLeft + (i - 27) * outerWalls[i].getSize().x, outerWallBottom);
	}
	else {
		outerWalls[i].setPosition(outerWallRight, outerWallTop/2 + (i - 40) * outerWalls[i].getSize().y);
	}
}

void gameMap::render(sf::RenderWindow& window)
{
	for (auto oW : outerWalls) {
		window.draw(oW);
	}
	for (auto iW : innerWalls) {
		window.draw(iW);
	}
}
