#pragma once
#include "SFML/Graphics.hpp"
#include "Block.h"

#ifndef __PLAYERBLOCK_H_INCLUDED__
#define __PLAYERBLOCK_H_INCLUDED__

class PlayerBlock : public Block
{
public:


	PlayerBlock();

	PlayerBlock(float posX, float posY, float width, float height);

	enum class Mode
	{
		Normal,
		Vector,
		Straight
	};

	sf::Vector2f GetPosition();

	float GetXDisplacement();

	//Assigns new coordinates and moves the old coodinates to prev
	void SetPosition(sf::Vector2f newPos);

	void Interaction();

	

	Mode GetMode();

	void SetMode(Mode newMode);


	int specialReturn();


private:
	float prevX;
	Mode mode;



};


#endif




