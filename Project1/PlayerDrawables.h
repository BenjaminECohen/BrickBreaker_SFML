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


CollisionSide checkBlockOverlap(sf::Vector2f position, float radius, float scoreMultiplier);

void AddBlockToArray(Block newBlock);

int getBlockListSize();

sf::VertexArray& getBlockVertexArray(int index);

void ClearBlockArray();

int BlockArraySize();


