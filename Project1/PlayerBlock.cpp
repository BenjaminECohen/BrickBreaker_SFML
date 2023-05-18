#include "PlayerBlock.h"
#include <iostream>


PlayerBlock::PlayerBlock()
{
	posX = 0;
	posY = 0;

	prevX = 0;

	width = 0;
	height = 0;

	mode = Mode::Normal;

	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;

	for (int i = 0; i < sides.getVertexCount(); i++)
	{
		sides[i].color = sf::Color::Green;
	}
}

PlayerBlock::PlayerBlock(float posX, float posY, float width, float height)
{
	this->posX = posX;
	this->posY = posY;

	this->prevX = posX;

	this->width = width;
	this->height = height;

	mode = Mode::Normal;

	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;

	for (int i = 0; i < sides.getVertexCount(); i++)
	{
		sides[i].color = sf::Color::Green;
	}
}

sf::Vector2f PlayerBlock::GetPosition()
{
	return sf::Vector2f(posX, posY);
}

float PlayerBlock::GetXDisplacement()
{
	return posX - prevX;
}

void PlayerBlock::SetPosition(sf::Vector2f newPos)
{

	prevX = posX;

	posX = newPos.x;
	posY = newPos.y;

	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;
}

void PlayerBlock::Interaction()
{
	std::cout << "PlayerBlock Hit By Ball" << std::endl;
}


PlayerBlock::Mode PlayerBlock::GetMode()
{
	return mode;
}

void PlayerBlock::SetMode(PlayerBlock::Mode newMode)
{
	mode = newMode;
	if (newMode == Mode::Normal)
	{
		ReColor(sf::Color::Green);
	}
	if (newMode == Mode::Vector)
	{
		ReColor(sf::Color::Cyan);
	}
}

