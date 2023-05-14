#include "Score.h"
#include <string>


int _Score = 0;

void AddScore(int pointsToAdd)
{
	_Score += pointsToAdd;
	
}

void UpdateScoreText(sf::Text& scoreText)
{
	scoreText.setString("Score: " + std::to_string(_Score));
}

int GetScore()
{
	return _Score;
}


