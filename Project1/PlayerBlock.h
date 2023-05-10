#pragma once
#include "SFML/Graphics.hpp"
#include "Block.h"

#ifndef __PLAYERBLOCK_H_INCLUDED__
#define __PLAYERBLOCK_H_INCLUDED__

class PlayerBlock : public Block
{

private:


public:

	PlayerBlock();

	PlayerBlock(float posX, float posY, float width, float height);

	sf::Vector2f GetPosition();

	void SetPosition(sf::Vector2f newPos);

	void Interaction();

	int specialReturn();

};


#endif




