#pragma once
#include "Block.h"
#include "PlayerBlock.h"

enum CollisionSide
{
	None = 0,
	Top = 1,
	Bottom = 2,
	Right = 3,
	Left = 4
};


//Player Centric
CollisionSide checkPlayerOverlap(PlayerBlock playerBlock, sf::Vector2f position, float radius);


CollisionSide checkBlockOverlap(sf::Vector2f position, float radius);

CollisionSide checkObstacleOverlap(sf::Vector2f position, float radius);

void AddBlockToArray(Block newBlock);

sf::VertexArray createObstacleVertexArray(sf::VertexArray box);

int getBlockListSize();

sf::VertexArray& getBlockVertexArray(int index);


int getObstacleListSize();

bool checkIndexExist(int index);

sf::VertexArray getObstacle(int index);