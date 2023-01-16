#include <SFML/Graphics.hpp>
#include "PlayerDrawables.h"
#include <iostream>


std::vector<sf::VertexArray> obstacles = std::vector<sf::VertexArray>();


int checkObstacleOverlap(sf::Vector2f position, float radius)
{

	

	for (int i = 0; i < obstacles.size(); i++)
	{
		if (position.x + radius >= obstacles[i][0].position.x && position.x - radius <= obstacles[i][1].position.x) // Left and Right
		{
			if (position.y >= obstacles[i][0].position.y && position.y <= obstacles[i][3].position.y) //up then down
			{
				if (std::abs(position.x - obstacles[i][0].position.x) < std::abs(position.x - obstacles[i][1].position.x))
				{
					std::cout << "OVERLAP Left" << std::endl;
					return 2;
				}
				else
				{
					
					std::cout << "OVERLAP Right" << std::endl;
					return 1;
				}

				
			}
		}
		if (position.y >= obstacles[i][0].position.y && position.y <= obstacles[i][3].position.y) //up and down
		{
			if (position.x + radius >= obstacles[i][0].position.x && position.x - radius <= obstacles[i][1].position.x) //Left then Right
			{
				if (std::abs(position.y - obstacles[i][0].position.y) < std::abs(position.y - obstacles[i][3].position.y))
				{
					std::cout << "OVERLAP Up" << std::endl;
					return 3;
				}
				else
				{

					std::cout << "OVERLAP Down" << std::endl;
					return 4;
				}
			}
		}

	}

	return 0;
}


void createObstacleVertexArray(sf::VertexArray box)
{
	sf::VertexArray obstacle(sf::Quads, 4);
	
	for (int i = 0; i < obstacle.getVertexCount(); i++)
	{
		obstacle[i] = box[i];
		obstacle[i].color = sf::Color::Cyan;
	}

	obstacles.push_back(obstacle);

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
