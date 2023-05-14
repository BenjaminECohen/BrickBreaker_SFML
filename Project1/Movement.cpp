#include <SFML/Graphics.hpp>
#include "Movement.h"

float calculateForce(sf::VertexArray& vertices)
{
	sf::Vector2f origin = vertices[0].position;
	sf::Vector2f to = vertices[1].position;

	float force = std::sqrt
	(
		std::pow(std::abs(to.y - origin.y), 2.f) + 
		std::pow(std::abs(to.x - origin.x), 2.f)
	);

	return force;

}


std::vector<float> getMovementVector(sf::VertexArray& vertices)
{
	sf::Vector2f origin = vertices[0].position;
	sf::Vector2f to = vertices[1].position;

	return std::vector<float> {to.x - origin.x, to.y - origin.y};

}

std::vector<float> getMovementVector(sf::VertexArray& vertices, bool normalize)
{
	if (normalize)
	{
		sf::Vector2f origin = vertices[0].position;
		sf::Vector2f to = vertices[1].position;
		float length = calculateForce(vertices);

		return std::vector<float> {(to.x - origin.x) / length, (to.y - origin.y) / length};
	}
	else
	{
		return getMovementVector(vertices);
	}

}
