#pragma once


enum CollisionSide
{
	None = 0,
	Top = 1,
	Bottom = 2,
	Right = 3,
	Left = 4
};

CollisionSide checkObstacleOverlap(sf::Vector2f position, float radius);


sf::VertexArray createObstacleVertexArray(sf::VertexArray box);

int getObstacleListSize();

bool checkIndexExist(int index);

sf::VertexArray getObstacle(int index);