#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "Gameplay.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

//PlayerShape
sf::CircleShape playerShape(10.f);

const float GAME_WINDOW_START_SIZE = 200.f;
float windowCurrSize = GAME_WINDOW_START_SIZE;
float windowScale = 1.f;


bool updateGame(sf::Clock& clock)
{
	if (clock.getElapsedTime().asSeconds() >= 1.f)
	{

		std::cout << "Restarting: " << clock.getElapsedTime().asSeconds() << std::endl;
		return true;
	}
	return false;
}

void resizeWindow(sf::RenderWindow& window)
{
	std::cout << "Gameplay: Window Resized" << std::endl;


	windowCurrSize = window.getSize().y;
	if (windowCurrSize < GAME_WINDOW_START_SIZE)
	{
		windowCurrSize = GAME_WINDOW_START_SIZE;
	}
	//Resize the window to always be a perfect square, justified to the y size
	sf::Vector2u newSize(windowCurrSize, windowCurrSize);
	window.setSize(newSize);

	//Get new scale of screen compared to original value
	windowScale = 1.f + (windowCurrSize - GAME_WINDOW_START_SIZE) / GAME_WINDOW_START_SIZE;
	std::cout << "Window Scale is now: " << windowScale * 100 << "%" << std::endl;
	


}

int gameloop(sf::RenderWindow& window, sf::Clock& clock)
{
	//set size to game start size
	window.setSize(sf::Vector2u((unsigned int)GAME_WINDOW_START_SIZE, (unsigned int)GAME_WINDOW_START_SIZE));

	//Set player shape params
	playerShape.setOrigin(playerShape.getRadius() / 2.f, playerShape.getRadius() / 2.f);
	playerShape.setPosition(window.getSize().y / 2.f, window.getSize().y / 2.f);

	while (window.isOpen())
	{
		sf::Event event;
		if (updateGame(clock))
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
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						std::cout << "Clicked at  ";
						std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					}
				}
			}
		}

		

		window.clear();
		window.draw(playerShape);
		//Draw all necessary stuff



		window.display();
	}

	return 0;
	
}