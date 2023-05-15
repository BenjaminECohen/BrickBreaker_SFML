#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "Movement.h"
#include "PlayerDrawables.h"
#include "Block.h"
#include "PlayerBlock.h"
#include "Score.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



//PlayerShape = Ball
sf::CircleShape ball(10.f);
PlayerBlock player;




const float WINDOW_STANDARD_HEIGHT = 600.f;

float WINDOW_STANDARD_WIDTH = 400.f;
float BREAK_ZONE_MAX_HEIGHT = 253;

const float WIDTH_LINE_COUNT = 11;
const float HEIGHT_LINE_COUNT = 13;

const float BREAK_BLOCK_WIDTH = 40;
const float BREAK_BLOCK_HEIGHT = 20;

const float PLAYER_HEIGHT_POS = WINDOW_STANDARD_HEIGHT - 75.f;

const float PLAYER_BALL_BOUNCE_ANGLE_MAX = 45.f;

const double PI = 3.14159f;


//float windowCurrSize = WINDOW_STANDARD_SIZE;
float windowScale = 1.f;


//UI
sf::Text scoreText;
sf::Text countdownText;
sf::Text livesText;
sf::Text gameStateText;

//Lives
const int LIVES_MAX = 3;
int lives = LIVES_MAX;
std::string livesRep = "|";




//PlayerLine && Box
sf::Vector2i mouseStart = sf::Vector2i(0, 0);

sf::Vector2f playerNewPos = sf::Vector2f(0, 0);

sf::VertexArray vsLine(sf::Lines, 2);
sf::VertexArray box(sf::LineStrip, 5);
bool leftDown = false;
bool rightDown = false;


//Movement
const std::vector<float> BALL_START_FORCE{ 0.f, 300.f };
std::vector<float> forceVector{ 0.f, 0.f };
bool updateForce = false;
float speedModifier = 1.f; //Player may be able to change this later
float frictionModifier = 0.0f;

float speedIncreasePerHit = 0.01f;


bool runStart = true;

int _bListSize = 0;

#pragma region Game Loop

//Start of game for block break
void startGame()
{
	
	vsLine[0].color = sf::Color::Cyan;
	vsLine[1].color = sf::Color::Cyan;


	//Create break zone
	Block block;
	for (int y = (BREAK_BLOCK_HEIGHT / 2.f) + 1; 
		y < BREAK_ZONE_MAX_HEIGHT - (BREAK_BLOCK_HEIGHT / 2.f); 
		y += BREAK_BLOCK_HEIGHT + 1)
	{
		for (int x = (BREAK_BLOCK_WIDTH / 2.f) + 1; 
			x < WINDOW_STANDARD_WIDTH - (BREAK_BLOCK_WIDTH / 2.f); 
			x += BREAK_BLOCK_WIDTH + 1)
		{
			block = Block::Block(x, y, BREAK_BLOCK_WIDTH, BREAK_BLOCK_HEIGHT, 5);
			AddBlockToArray(block);
		}
	}

	//Create Player
	player = PlayerBlock(WINDOW_STANDARD_WIDTH / 2.f, PLAYER_HEIGHT_POS, 1.5f * BREAK_BLOCK_WIDTH, BREAK_BLOCK_HEIGHT);


	//Set Lives


	runStart = false;
}


void ThreadWaitBallReset()
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	forceVector = BALL_START_FORCE;
	
	
}

void ResetBall()
{
	forceVector[0] = 0.f;
	forceVector[1] = 0.f;
	speedModifier = 1.f;
	ball.setPosition(WINDOW_STANDARD_WIDTH / 2.f, WINDOW_STANDARD_HEIGHT / 2.f);

	//ADDME: Decrease Player Lives

	//Thread a wait time for the ball to resume movement
	std::thread ballReset(ThreadWaitBallReset);
	ballReset.detach();
}



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
	float radius = ball.getRadius();


	//FIXME
	//std::cout << "New Position" << ball.getPosition().x << ", " << ball.getPosition().y << std::endl;

	//



	//Update Ball force
	if (updateForce)
	{
		forceVector = getMovementVector(vsLine);
		std::cout << "X Force:  " << forceVector[0] << std::endl;
		std::cout << "Y Force:  " << forceVector[1] << std::endl;
	}

	//Check if ball is out of bounds
	if (ball.getPosition().x + radius >= WINDOW_STANDARD_WIDTH) //Ball has gone too far right
	{
		forceVector[0] = std::abs(forceVector[0]) * -1.f;
	}
	else if (ball.getPosition().x - radius <= 0) //Ball has gone too far left
	{
		forceVector[0] = std::abs(forceVector[0]);
	}
	else if (ball.getPosition().y + radius >= WINDOW_STANDARD_HEIGHT) //Ball has gone too far down
	{
		//Lose Life
		--lives;

		//Ball out of bounds and needs reset
		if (lives != 0)
		{
			ResetBall();
		}
		
		//forceVector[1] = std::abs(forceVector[1]) * -1.f;
	}
	else if(ball.getPosition().y - radius <= 0) //Ball has gone too far up
	{
		//Inverse x force
		forceVector[1] = std::abs(forceVector[1]);
	}

	//Check if ball has collided with a spawned entity

	switch (checkBlockOverlap(ball.getPosition(), ball.getRadius()))
	{
		case Right:
			forceVector[0] = std::abs(forceVector[0]);
			break;

		case Left:
			forceVector[0] = std::abs(forceVector[0]) * -1.f;
			break;

		case Top:
			forceVector[1] = std::abs(forceVector[1]) * -1.f;
			break;

		case Bottom:
			forceVector[1] = std::abs(forceVector[1]);
			break;

		default:
			//std::cout << "Something has gone wrong or No Collision" << std::endl;
			break;

	}

	//Check if ball has colldied with player
	if (checkPlayerOverlap(player, ball.getPosition(), ball.getRadius()) == Top && forceVector[1] > 0)
	{
		if (player.GetMode() == PlayerBlock::Mode::Normal)
		{
			forceVector[1] = std::abs(forceVector[1]) * -1.f;

			float playerXDisp = player.GetXDisplacement();

			float randomAngle = static_cast<float>(rand() / static_cast<float>(RAND_MAX))* PLAYER_BALL_BOUNCE_ANGLE_MAX;

			if (playerXDisp == 0)
			{
				forceVector[0] = forceVector[1] * std::tan(randomAngle * PI / 180.f);
			}
			else
			{
				forceVector[0] = (playerXDisp / std::abs(playerXDisp)) * forceVector[1] * std::tan(randomAngle * PI / 180.f);
			}
		}
		else //Vector Mode
		{
			std::vector<float> _vsLine = getMovementVector(vsLine);

			forceVector[1] = std::abs(forceVector[1]) * -1.f;
			forceVector[0] = (forceVector[1] * _vsLine[0]) / _vsLine[1];
			

			
			player.SetMode(PlayerBlock::Mode::Normal);
		}
		
		
		

		speedModifier += speedIncreasePerHit; //Increase ball speed

		
	}


	//Old code for collision detection
	/*
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
	}*/

	//Force Vector + Friction
	forceVector[0] += forceVector[0] * -1.f * frictionModifier * clock.getElapsedTime().asSeconds();
	forceVector[1] += forceVector[1] * -1.f * frictionModifier* clock.getElapsedTime().asSeconds();

	if (std::abs(forceVector[0]) <= 0.1f && std::abs(forceVector[1]) <= 0.1f) //Once low enough, round to zero
	{
		//std::cout << "Round to zero" << std::endl;
		forceVector[0] = 0.f;
		forceVector[1] = 0.f;
	}

	//Determine new position of the ball
	sf::Vector2f newPos{
			ball.getPosition().x + (speedModifier * forceVector[0] * clock.getElapsedTime().asSeconds()),
			ball.getPosition().y + (speedModifier * forceVector[1] * clock.getElapsedTime().asSeconds())
	};
	ball.setPosition(newPos);
	updateForce = false;

	return true;

}

/*
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



}*/

#pragma endregion


#pragma region Player Functions

sf::VertexArray generateVectorShotLine()
{
	return vsLine;

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

//Sets player x pos to where the mouse is.
void MovePlayer(sf::RenderWindow& window)
{
	playerNewPos = window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition(window).x, PLAYER_HEIGHT_POS));

	player.SetPosition(playerNewPos);


}

#pragma endregion


#pragma region UI Functions
//Lives Methods
void UpdateLivesText()
{
	std::string temp = "";

	for (int i = 0; i < lives; i++)
	{
		temp += livesRep + " ";
	}

	livesText.setString(temp);

	
	sf::FloatRect livesRect = livesText.getLocalBounds();
	livesText.setOrigin(livesRect.left + livesRect.width,
		livesRect.top + livesRect.height / 2.f);

}

void SetGameText(std::string val)
{
	gameStateText.setString(val);
}

sf::Text GetGameText()
{
	sf::FloatRect textRect = gameStateText.getLocalBounds();
	gameStateText.setOrigin(textRect.left + textRect.width / 2.f,
		textRect.top + textRect.height / 2.f);
	gameStateText.setPosition(WINDOW_STANDARD_WIDTH / 2.f, WINDOW_STANDARD_HEIGHT / 2.f);
	return gameStateText;
}


#pragma endregion

int main()
{
	WINDOW_STANDARD_WIDTH = (BREAK_BLOCK_WIDTH * (WIDTH_LINE_COUNT - 1)) + WIDTH_LINE_COUNT;
	BREAK_ZONE_MAX_HEIGHT = (BREAK_BLOCK_HEIGHT * (HEIGHT_LINE_COUNT - 1)) + HEIGHT_LINE_COUNT;

	std::cout << "Window dimensions are: " << WINDOW_STANDARD_WIDTH << " by " << WINDOW_STANDARD_HEIGHT << std::endl;
	std::cout << "Break zone max height is: " << BREAK_ZONE_MAX_HEIGHT << std::endl;

	sf::RenderWindow window(sf::VideoMode((unsigned int)WINDOW_STANDARD_WIDTH, (unsigned int)WINDOW_STANDARD_HEIGHT), "Brick Breaker XV: Redux EXCITE 5!");
	

	std::cout << "Window dimensions by window default view: " << window.getDefaultView().getSize().x 
		<< " by " << window.getDefaultView().getSize().y << std::endl;
	std::cout << "Window dimensions by window size: " << window.getSize().x 
		<< " by " << window.getSize().y << std::endl;

	srand(std::time(0));

	sf::Time t1 = sf::microseconds(10000);
	sf::Time t2 = sf::milliseconds(10);
	sf::Time t3 = sf::seconds(0.01f);

	float sec = t3.asSeconds();

	sf::Clock clock;
	//std::thread timedFunc(timeSurprise, 5.f);
	sf::FloatRect textRect;

	sf::Font font1;
	if (!font1.loadFromFile("Game Of Squids.ttf"))
	{
		std::cout << "font not found" << std::endl;
	}

	//Set Score Variables
	scoreText.setFont(font1);
	scoreText.setPosition(0, 575);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::Green);
	scoreText.setStyle(sf::Text::Bold);
	UpdateScoreText(scoreText);
	textRect = scoreText.getLocalBounds();
	scoreText.setOrigin(0, textRect.top + textRect.height / 2.f);
	


	//Set Lives
	livesText.setFont(font1);
	textRect = livesText.getLocalBounds();
	livesText.setOrigin(textRect.left + textRect.width,
		textRect.top + textRect.height / 2.f);
	livesText.setPosition(WINDOW_STANDARD_WIDTH, 575);
	livesText.setCharacterSize(24);
	livesText.setFillColor(sf::Color::Green);
	livesText.setStyle(sf::Text::Bold);


	//Set GameText
	gameStateText.setFont(font1);
	gameStateText.setCharacterSize(24);
	gameStateText.setFillColor(sf::Color::Green);
	gameStateText.setStyle(sf::Text::Bold);


	//Set ball params
	ball.setOrigin(ball.getRadius(), ball.getRadius());
	ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	forceVector = BALL_START_FORCE;


	bool mousePressed = false;

	while (window.isOpen())
	{
		if (runStart)
		{
			startGame();
		}

		sf::Event event;
		if (lives != 0 && updateGame(window, clock))
		{
			clock.restart();
		}
		else
		{
			SetGameText("Game Over");
			//End game
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
				//resizeWindow(window);
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
				//Catch mode change
				//Enable Vector Shot
				if (event.mouseButton.button == sf::Mouse::Left && player.GetMode() == PlayerBlock::Mode::Normal)
				{
					player.SetMode(PlayerBlock::Mode::Vector);
				}
				//Disable Vector Shot
				else if (event.mouseButton.button == sf::Mouse::Left && player.GetMode() == PlayerBlock::Mode::Vector)
				{
					player.SetMode(PlayerBlock::Mode::Normal);
				}

				/*
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
				}*/
			}
			case sf::Event::MouseMoved:
			{
				if (player.GetMode() == PlayerBlock::Mode::Normal)
				{
					MovePlayer(window);
				}
				if (player.GetMode() == PlayerBlock::Mode::Vector)
				{
					//Get Origin of Player as start and mouse pos as end of line
					vsLine[0].position = player.GetPosition() - sf::Vector2f(0, player.GetHeight() / 2.f);
					vsLine[1].position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				}

			}

			case sf::Event::MouseButtonReleased:
			{
				/*
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
				}*/
			}

			}
		}

		

		window.clear();

		


		//Draw all necessary stuff

		window.draw(ball); //Draw Player

		//Draw Blocks
		_bListSize = getBlockListSize();
		if (_bListSize != 0)
		{
			for (int i = 0; i < _bListSize; i++)
			{
				window.draw(getBlockVertexArray(i));
			}
		}


		//Draw Player Block
		window.draw(player.GetVertexArray());

		if (player.GetMode() == PlayerBlock::Mode::Vector)
		{
			window.draw(generateVectorShotLine());
		}
		


		//Draw Obstacles (OLD)
		/*
		if (getObstacleListSize() != 0) 
		{
			int oSize = getObstacleListSize();
			for (int i = 0; i < oSize; i++)
			{
				window.draw(getObstacle(i));
			}
		}
		*/
		
		/*
		if (leftDown) //Left Mouse Functionality
		{
			window.draw(generateVectorShotLine);
		}
		if (rightDown) //Right Mouse Functionality
		{
			window.draw(getPlayerBoxVertices(window));
		}*/

		
		window.draw(GetGameText());
		
		
		UpdateScoreText(scoreText);
		window.draw(scoreText);
		
		UpdateLivesText();
		window.draw(livesText);



		window.display();
	}

	return 0;

}

