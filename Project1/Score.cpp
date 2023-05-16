#include "Score.h"
#include <string>
#include <iostream>


int _Score = 0;

void AddScore(int pointsToAdd)
{
	_Score += pointsToAdd;
	
}

void UpdateScoreText(sf::Text& scoreText)
{
	scoreText.setString(" Score: " + std::to_string(_Score));
}

int GetScore()
{
	return _Score;
}

void ResetScore()
{
	_Score = 0;
}


