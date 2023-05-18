#include "Block.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>

Block::Block()
{
	posX = 0;
	posY = 0;

	width = 0;
	height = 0;

	maxHealth = 1;
	health = maxHealth;

	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;

	ReColor();

}

Block::Block(float posX, float posY, float width, float height)
{
	this->posX = posX;
	this->posY = posY;

	this->width = width;
	this->height = height;

	maxHealth = 1;
	health = maxHealth;

	//Create the sides array
	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;

	ReColor();

};

Block::Block(float posX, float posY, float width, float height, int health)
{
	this->posX = posX;
	this->posY = posY;

	this->width = width;
	this->height = height;

	maxHealth = health;
	this->health = maxHealth;

	//Create the sides array
	sides[0].position = sf::Vector2f(posX - (width / 2.f), posY - (height / 2.f));
	sides[1].position = sf::Vector2f(posX + (width / 2.f), posY - (height / 2.f));
	sides[2].position = sf::Vector2f(posX + (width / 2.f), posY + (height / 2.f));
	sides[3].position = sf::Vector2f(posX - (width / 2.f), posY + (height / 2.f));
	sides[4].position = sides[0].position;

	ReColor();

};

sf::VertexArray& Block::GetVertexArray()
{
	return sides;
}

float Block::GetWidth()
{
	return width;
}


float Block::GetHeight()
{
	return height;
}

int Block::GetHealth()
{
	return health;
}



void Block::ReColor()
{
	if (health > 0)
	{
		sf::Uint8 val = static_cast<sf::Uint8>(round(((float)health / (float)maxHealth) * 255));
		//std::cout << "Val: " << (int)val << std::endl;

		for (int i = 0; i < 5; i++)
		{
			sides[i].color = sf::Color::Color(255, val, val);
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			//sides[i].color = sf::Color::Cyan; //TEMPORARY FOR DEBUGING
			sides[i].color = sf::Color::Transparent;
		}
	}
	
};

void Block::ReColor(sf::Color newColor)
{
	for (int i = 0; i < 5; i++)
	{
		sides[i].color = newColor;
	}
}

void Block::Interaction() 
{
	health -= 1;
	if (health > 0)
	{
		AddScore(5);
	}
	else
	{
		AddScore(20);
	}
	ReColor();
};


void Block::Interaction(float scoreMultiplier)
{
	health -= 1;
	if (health > 0)
	{
		AddScore(static_cast<int>(5.0f * scoreMultiplier));
	}
	else
	{
		AddScore(static_cast<int>(20.0f * scoreMultiplier));
	}
	ReColor();
};



