#ifndef __MOVEMENT_H_INCLUDED__
#define __MOVEMENT_H_INCLUDED__

float calculateForce(sf::VertexArray& vertices);

std::vector<float> getMovementVector(sf::VertexArray& vertices);

std::vector<float> getMovementVector(sf::VertexArray& vertices, bool normalize);

#endif