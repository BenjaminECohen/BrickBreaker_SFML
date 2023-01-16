#pragma once


int checkObstacleOverlap(sf::Vector2f position, float radius);

void createObstacleVertexArray(sf::VertexArray box);

int getObstacleListSize();

bool checkIndexExist(int index);

sf::VertexArray getObstacle(int index);