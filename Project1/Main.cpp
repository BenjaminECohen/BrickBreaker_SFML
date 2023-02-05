#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "Movement.h"
#include "PlayerDrawables.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



//PlayerShape
sf::CircleShape playerShape(10.f);

const float WINDOW_STANDARD_SIZE = 200.f;
float windowCurrSize = WINDOW_STANDARD_SIZE;
float windowScale = 1.f;

//PlayerLine && Box
sf::Vector2i mouseStart = sf::Vector2i(0, 0);
sf::VertexArray points(sf::Lines, 2);
sf::VertexArray box(sf::LineStrip, 5);
bool leftDown = false;
bool rightDown = false;


//Movement
std::vector<float> forceVector{ 0.f, 0.f };
bool updateForce = false;
float speedModifier = 1.f; //Player may be able to change this later
float frictionModifier = 0.4f;



/*
void RectColliderDetection(sf::CircleShape& player, sf::VertexArray rect)
{
	float rectTop = rect[0].position.y;
	float rectBot = rect[3].position.y;
	float rectRight = rect[1].position.x;
	float rectLeft = rect[0].position.x;

	while (true)
	{

		//Check Bottom
		if (lineDistance(player.getPosition().y, rectBot, false) <= 0)
		{

		}
		//Check Top
		
	}
}*/

float lineDistance(float player, float obstacle, bool absoluteValue)
{
	if (absoluteValue)
	{
		return std::abs(player - obstacle);
	}
	return player - obstacle;
}


bool updateGame(sf::RenderWindow& window, sf::Clock& clock)
{
	float radius = playerShape.getRadius();
	

	//Update force
	if (updateForce)
	{
		forceVector = getMovementVector(points);
		std::cout << "X Force:  " << forceVector[0] << std::endl;
		std::cout << "Y Force:  " << forceVector[1] << std::endl;
	}

	//Check if ball is out of bounds
	if (playerShape.getPosition().x + radius >= WINDOW_STANDARD_SIZE) //Ball has gone too far right
	{
		forceVector[0] = std::abs(forceVector[0]) * -1.f;
	}
	else if (playerShape.getPosition().x - radius <= 0) //Ball has gone too far left
	{
		forceVector[0] = std::abs(forceVector[0]);
	}
	else if (playerShape.getPosition().y + radius >= WINDOW_STANDARD_SIZE) //Ball has gone too far down
	{
		forceVector[1] = std::abs(forceVector[1]) * -1.f;
	}
	else if(playerShape.getPosition().y - radius <= 0) //Ball has gone too far up
	{
		//Inverse x force
		forceVector[1] = std::abs(forceVector[1]);
	}

	//Check if ball has collided with a spawned entity
	if (checkObstacleOverlap(playerShape.getPosition(), playerShape.getRadius()) == Right) //Rightmost bound
	{
		forceVector[0] = std::abs(forceVector[0]);
	}
	else if (checkObstacleOverlap(playerShape.getPosition(), playerShape.getRadius()) == Left) //Leftmost bound
	{
		forceVector[0] = std::abs(forceVector[0]) * -1.f;
	}
	else if (checkObstacleOverlap(playerShape.getPosition(), playerShape.getRadius()) == Top) //Upper bound
	{
		forceVector[1] = std::abs(forceVector[1]) * -1.f;
	}
	else if (checkObstacleOverlap(playerShape.getPosition(), playerShape.getRadius()) == Bottom) //Downmost bound
	{
		forceVector[1] = std::abs(forceVector[1]);
	}

	//Force Vector + Friction
	forceVector[0] += forceVector[0] * -1.f * frictionModifier * clock.getElapsedTime().asSeconds();
	forceVector[1] += forceVector[1] * -1.f * frictionModifier* clock.getElapsedTime().asSeconds();

	if (std::abs(forceVector[0]) <= 0.1f && std::abs(forceVector[1]) <= 0.1f) //Once low enough, round to zero
	{
		//std::cout << "Round to zero" << std::endl;
		forceVector[0] = 0.f;
		forceVector[1] = 0.f;
	}

	//Determine new position of player
	sf::Vector2f newPos{
			playerShape.getPosition().x + (speedModifier * forceVector[0] * clock.getElapsedTime().asSeconds()),
			playerShape.getPosition().y + (speedModifier * forceVector[1] * clock.getElapsedTime().asSeconds())
	};
	playerShape.setPosition(newPos);
	updateForce = false;

	return true;

}

void resizeWindow(sf::RenderWindow& window)
{
	std::cout << "Gameplay: Window Resized" << std::endl;


	windowCurrSize = window.getSize().y;
	if (windowCurrSize < WINDOW_STANDARD_SIZE)
	{
		windowCurrSize = WINDOW_STANDARD_SIZE;
	}
	//Resize the window to always be a perfect square, justified to the y size
	sf::Vector2u newSize(windowCurrSize, windowCurrSize);
	window.setSize(newSize);

	//Get new scale of screen compared to original value
	windowScale = 1.f + (windowCurrSize - WINDOW_STANDARD_SIZE) / WINDOW_STANDARD_SIZE;
	std::cout << "Window Scale is now: " << windowScale * 100 << "%" << std::endl;



}



sf::VertexArray getPlayerLineVertices(sf::RenderWindow& window)
{
	points[0].position = window.mapPixelToCoords(mouseStart);
	points[1].position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	points[0].color = sf::Color::Red;
	points[1].color = sf::Color::Red;

	return points;

}

sf::VertexArray getPlayerBoxVertices(sf::RenderWindow& window)
{

	box[0].position = window.mapPixelToCoords(mouseStart);														//upper left
	box[1].position = window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition(window).x, mouseStart.y));	//upper right
	box[2].position = window.mapPixelToCoords(sf::Mouse::getPosition(window));									//lower right
	box[3].position = window.mapPixelToCoords(sf::Vector2i(mouseStart.x, sf::Mouse::getPosition(window).y));	//lower left														
	box[4].position = window.mapPixelToCoords(mouseStart);														//Last Connection
	
	for (int i = 0; i < box.getVertexCount(); i++)
	{
		box[i].color = sf::Color::Blue;
	}

	return box;
}




int main()
{
	

	sf::RenderWindow window(sf::VideoMode((unsigned int)WINDOW_STANDARD_SIZE, (unsigned int)WINDOW_STANDARD_SIZE), "Dungeon!");
	

	sf::Time t1 = sf::microseconds(10000);
	sf::Time t2 = sf::milliseconds(10);
	sf::Time t3 = sf::seconds(0.01f);

	float sec = t3.asSeconds();

	sf::Clock clock;
	//std::thread timedFunc(timeSurprise, 5.f);


	sf::Font font1;
	if (!font1.loadFromFile("Game Of Squids.ttf"))
	{
		std::cout << "font not found" << std::endl;
	}


	sf::Text title;
	title.setFont(font1);
	title.setString("Ballin'");
	title.setCharacterSize(24);
	title.setFillColor(sf::Color::Green);
	title.setStyle(sf::Text::Bold);

	//Set player shape params
	playerShape.setOrigin(playerShape.getRadius(), playerShape.getRadius());
	playerShape.setPosition(window.getSize().y / 2.f, window.getSize().y / 2.f);


	bool mousePressed = false;

	while (window.isOpen())
	{
		sf::Event event;
		if (updateGame(window, clock))
		{
			clock.restart();
		}

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				std::cout << "Closing Window" << std::endl;
				window.close();
				break;
			}
			case sf::Event::Resized:
			{
				resizeWindow(window);
				break;
			}
			case sf::Event::LostFocus:
			{
				std::cout << "Lost Focus" << std::endl;
				//Pasue game or do something
				break;
			}
			case sf::Event::GainedFocus:
			{
				std::cout << "Gained Focus" << std::endl;
				//Resume game or do something
				break;
			}
			case sf::Event::TextEntered:
			{
				std::cout << static_cast<char>(event.text.unicode) << " Was Pressed" << std::endl;
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (!rightDown && event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "Clicked at  ";
					std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					mouseStart = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

					//Set control booleans
					mousePressed = true;
					leftDown = true;
					break;
				}
				else if (!leftDown && event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "Right button down " << std::endl;
					std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					mouseStart = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

					//Set Control booleans
					rightDown = true;
					break;
				}
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left && leftDown)
				{
					std::cout << "Released at: ";
					std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					
					updateForce = true;

					//Set control booleans
					mousePressed = false;
					leftDown = false;
					break;

				}
				else if (rightDown && event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "Released at: ";
					std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					createObstacleVertexArray(box);

					//Set Control booleans
					rightDown = false;
					break;
				}
			}

			}
		}

		

		window.clear();

		


		//Draw all necessary stuff

		window.draw(playerShape); //Draw Player

		if (getObstacleListSize() != 0) //Draw Obstacles
		{
			int oSize = getObstacleListSize();
			for (int i = 0; i < oSize; i++)
			{
				window.draw(getObstacle(i));
			}
		}

		
		

		if (leftDown) //Left Mouse Functionality
		{
			window.draw(getPlayerLineVertices(window));
		}
		if (rightDown) //Right Mouse Functionality
		{
			window.draw(getPlayerBoxVertices(window));
		}

		
		
		
		window.draw(title);
		



		window.display();
	}

	return 0;

}

