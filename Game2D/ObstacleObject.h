#ifndef OBSTACLEOBJECT_H
#define OBSTACLEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class ObstacleObject : public GameObject
{
public:
	// Ball state	
	GLboolean Stuck;
	glm::vec2 firsPos;
	// Constructor(s)
	ObstacleObject();
	ObstacleObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	// Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
	void      Move(ObstacleObject* obstacle, bool isBird);
	// Resets the ball to original state with given position and velocity
	void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif