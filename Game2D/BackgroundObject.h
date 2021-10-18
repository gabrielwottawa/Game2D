#ifndef BACKGROUNDOBJECT_H
#define BACKGROUNDOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's ball object that
// were too specific for within GameObject alone.
class BackgroundObject : public GameObject
{
public:
	// Ball state	
	GLboolean Stuck;
	// Constructor(s)
	BackgroundObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity);
	void Move(BackgroundObject* background);
};

#endif