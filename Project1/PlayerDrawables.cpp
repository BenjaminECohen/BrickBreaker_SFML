#include <SFML/Graphics.hpp>
#include "PlayerDrawables.h"
#include <iostream>


std::vector<sf::VertexArray> obstacles = std::vector<sf::VertexArray>();


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
