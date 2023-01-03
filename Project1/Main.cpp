#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


bool updateGame(sf::Clock clock)
{
	if (clock.getElapsedTime().asSeconds() >= 1.f)
	{
		
		std::cout << "Restarting: " << clock.getElapsedTime().asSeconds() << std::endl;
		return true;
	}
	return false;
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


int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "Dungeon!");
	sf::CircleShape shape(100.f);

	sf::Time t1 = sf::microseconds(10000);
	sf::Time t2 = sf::milliseconds(10);
	sf::Time t3 = sf::seconds(0.01f);

	float sec = t3.asSeconds();

	sf::Clock clock;
	std::thread timedFunc(timeSurprise, 5.f);

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
					//Resize the window to always be a perfect square, justified to the y size
					sf::Vector2u newSize(window.getSize().y, window.getSize().y);
					window.setSize(newSize);
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
		window.draw(shape);
		window.display();
	}

	

	return 0;
}

