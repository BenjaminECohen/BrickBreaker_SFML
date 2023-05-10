#include <SFML/Graphics.hpp>
#include "PlayerDrawables.h"
#include <iostream>

std::vector<Block> gameBlocks = std::vector<Block>();

std::vector<sf::VertexArray> obstacles = std::vector<sf::VertexArray>();


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



CollisionSide checkBlockOverlap(sf::Vector2f position, float radius)
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

				gameBlocks[i].Interaction();

				return closestSide;
			}
		}
		

	}

	return None;
}

CollisionSide checkObstacleOverlap(sf::Vector2f position, float radius)
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		int collideCount = 0;
		if (position.x + radius >= obstacles[i][0].position.x) // Left
		{
			collideCount++;
		}
		if (position.x - radius <= obstacles[i][1].position.x) //Right 
		{
			collideCount++;
		}
		if (position.y + radius >= obstacles[i][0].position.y) //Top
		{
			collideCount++;
		}
		if (position.y - radius <= obstacles[i][3].position.y) //Bottom
		{
			collideCount++;
		}

		if (collideCount >= 4)
		{
			//Always starts with left
			int LastLowest = std::abs(position.x + radius - obstacles[i][0].position.x);
			CollisionSide closestSide = Left;
			
			if (std::abs(position.x - radius - obstacles[i][1].position.x) < LastLowest) // Check Right
			{
				LastLowest = std::abs(position.x - radius - obstacles[i][1].position.x);
				closestSide = Right;
			}
			if (std::abs(position.y + radius - obstacles[i][0].position.y) < LastLowest) // Check Top
			{
				LastLowest = std::abs(position.y + radius - obstacles[i][0].position.y);
				closestSide = Top;
			}
			if (std::abs(position.y - radius - obstacles[i][3].position.y) < LastLowest) // Check Bottom
			{
				LastLowest = std::abs(position.y - radius - obstacles[i][3].position.y);
				closestSide = Bottom;
			}
			
			return closestSide;
		}

	}

	return None;
}


void AddBlockToArray(Block newBlock)
{
	gameBlocks.push_back(newBlock);
}

sf::VertexArray createObstacleVertexArray(sf::VertexArray box)
{
	sf::VertexArray obstacle(sf::Quads, 4);
	
	for (int i = 0; i < obstacle.getVertexCount(); i++)
	{
		obstacle[i] = box[i];
		obstacle[i].color = sf::Color::Cyan;
	}

	obstacles.push_back(obstacle);
	return obstacle;

};


int getBlockListSize()
{
	return gameBlocks.size();
}

sf::VertexArray& getBlockVertexArray(int index)
{
	return gameBlocks[index].GetVertexArray();
}

int getObstacleListSize()
{
	return obstacles.size();
}


bool checkIndexExist(int index)
{
	if (index >= obstacles.size())
	{
		return false;
	}

	return true;
}

sf::VertexArray getObstacle(int index)
{
	return obstacles[index];
}
