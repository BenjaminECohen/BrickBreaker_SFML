#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "Movement.h"

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

//PlayerLine
sf::Vector2i mouseStart = sf::Vector2i(0, 0);
sf::VertexArray points(sf::Lines, 2);


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


void timeSurprise(float duration)
{
	sf::Clock timer;

	while (true)
	{
		if (timer.getElapsedTime().asSeconds() >= duration)
		{
			std::cout << "Surprise!!!!" << std::endl;
			break;
		}
	}
}

sf::VertexArray getPlayerLineVertices(sf::RenderWindow& window)
{
	points[0].position = window.mapPixelToCoords(mouseStart);
	points[1].position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	points[0].color = sf::Color::Red;
	points[1].color = sf::Color::Red;

	return points;
	
}


int main()
{
	

	sf::RenderWindow window(sf::VideoMode((unsigned int)WINDOW_STANDARD_SIZE, (unsigned int)WINDOW_STANDARD_SIZE), "Dungeon!");
	

	sf::Time t1 = sf::microseconds(10000);
	sf::Time t2 = sf::milliseconds(10);
	sf::Time t3 = sf::seconds(0.01f);

	float sec = t3.asSeconds();

	sf::Clock clock;
	std::thread timedFunc(timeSurprise, 5.f);


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
	playerShape.setOrigin(playerShape.getRadius() / 2.f, playerShape.getRadius() / 2.f);
	playerShape.setPosition(window.getSize().y / 2.f, window.getSize().y / 2.f);


	bool mousePressed = false;

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
					mouseStart = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					mousePressed = true;
					break;
				}
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "Released at: ";
					std::cout << "x: " << event.mouseButton.x << " y: " << event.mouseButton.y << std::endl;
					std::cout << "Resulting Force: " << calculateForce(points) << std::endl;
					mousePressed = false;
					break;
				}
			}

			}
		}



		window.clear();

		//Draw all necessary stuff
		window.draw(playerShape);


		if (mousePressed)
		{
			window.draw(getPlayerLineVertices(window));
		}
		
		
		window.draw(title);
		



		window.display();
	}

	return 0;

}

