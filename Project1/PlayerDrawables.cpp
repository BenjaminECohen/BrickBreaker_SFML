#include <SFML/Graphics.hpp>
#include "PlayerDrawables.h"


std::vector<sf::VertexArray> obstacles = std::vector<sf::VertexArray>();




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
