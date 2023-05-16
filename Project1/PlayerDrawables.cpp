#include <SFML/Graphics.hpp>
#include "PlayerDrawables.h"
#include <iostream>

std::vector<Block> gameBlocks = std::vector<Block>();

CollisionSide checkPlayerOverlap(PlayerBlock playerBlock, sf::Vector2f position, float radius)
{
	//Check if Player Overlap
	sf::Vertex upperLeftPoint;
	sf::Vertex lowerRightPoint;
	int collideCount = 0;


	upperLeftPoint = playerBlock.GetVertexArray()[0];
	lowerRightPoint = playerBlock.GetVertexArray()[2];


	if (position.x + radius >= upperLeftPoint.position.x) // Left
	{
		collideCount++;
	}
	if (position.x - radius <= lowerRightPoint.position.x) //Right 
	{
		collideCount++;
	}
	if (position.y + radius >= upperLeftPoint.position.y) //Top
	{
		collideCount++;
	}
	if (position.y - radius <= lowerRightPoint.position.y) //Bottom
	{
		collideCount++;
	}
	if (collideCount >= 4)
	{
		return Top;
	}
	return None;
	
}

CollisionSide checkBlockOverlap(sf::Vector2f position, float radius, float scoreMultiplier)
{
	sf::Vertex upperLeftPoint;
	sf::Vertex lowerRightPoint;
	int collideCount = 0;
	CollisionSide closestSide = Left;

	//Check if Break Block Overlap
	for (int i = 0; i < gameBlocks.size(); i++)
	{
		if (gameBlocks[i].GetHealth() != 0)
		{
			upperLeftPoint = gameBlocks[i].GetVertexArray()[0];
			lowerRightPoint = gameBlocks[i].GetVertexArray()[2];

			collideCount = 0;
			if (position.x + radius >= upperLeftPoint.position.x) // Left
			{
				collideCount++;
			}
			if (position.x - radius <= lowerRightPoint.position.x) //Right 
			{
				collideCount++;
			}
			if (position.y + radius >= upperLeftPoint.position.y) //Top
			{
				collideCount++;
			}
			if (position.y - radius <= lowerRightPoint.position.y) //Bottom
			{
				collideCount++;
			}

			if (collideCount >= 4)
			{
				//Always starts with left
				int LastLowest = std::abs(position.x + radius - upperLeftPoint.position.x);

				closestSide = Left;

				if (std::abs(position.x - radius - lowerRightPoint.position.x) < LastLowest) // Check Right
				{
					LastLowest = std::abs(position.x - radius - lowerRightPoint.position.x);
					closestSide = Right;
				}
				if (std::abs(position.y + radius - upperLeftPoint.position.y) < LastLowest) // Check Top
				{
					LastLowest = std::abs(position.y + radius - upperLeftPoint.position.y);
					closestSide = Top;
				}
				if (std::abs(position.y - radius - lowerRightPoint.position.y) < LastLowest) // Check Bottom
				{
					LastLowest = std::abs(position.y - radius - lowerRightPoint.position.y);
					closestSide = Bottom;
				}

				gameBlocks[i].Interaction(scoreMultiplier);

				if (gameBlocks[i].GetHealth() == 0)
				{
					gameBlocks.erase(gameBlocks.begin() + i);
				}

				return closestSide;
			}
		}
		

	}

	return None;
}

void AddBlockToArray(Block newBlock)
{
	gameBlocks.push_back(newBlock);
}


int getBlockListSize()
{
	return gameBlocks.size();
}

sf::VertexArray& getBlockVertexArray(int index)
{
	return gameBlocks[index].GetVertexArray();
}

void ClearBlockArray()
{
	gameBlocks.clear();
}

int BlockArraySize()
{
	return gameBlocks.size();
}



