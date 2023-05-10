#include <SFML/Graphics.hpp>

#ifndef __BLOCK_H_INCLUDED__
#define __BLOCK_H_INCLUDED__

class Block
{
protected:

	float posX, posY, width, height;
	sf::VertexArray sides = sf::VertexArray(sf::Quads, 5);

private:
	

	int maxHealth, health; //Health = total # of hits to break block
	void ReColor();
	

public:

	Block();

	Block(float posX, float posY, float width, float height);

	Block(float posX, float posY, float width, float height, int health);

	sf::VertexArray& GetVertexArray();

	float GetWidth();

	float GetHeight();

	int GetHealth();

	sf::Color blockColor;
	
	virtual void Interaction();

	virtual int specialReturn();


};

#endif