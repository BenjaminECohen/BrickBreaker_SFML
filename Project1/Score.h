
#include <SFML/Graphics.hpp>


#ifndef __SCORE_H_INCLUDED__
#define __SCORE_H_INCLUDED__


extern int _Score;

extern void AddScore(int pointsToAdd);

extern void UpdateScoreText(sf::Text& scoreText);

extern int GetScore();

extern void ResetScore();


#endif
