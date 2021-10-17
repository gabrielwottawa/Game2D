#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class PlayerObject : public GameObject
{
public:
	// Ball state	
	GLboolean Stuck = true;
	// Constructor(s)
	PlayerObject();
	PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity);
	//PlayerObject(glm::vec2 pos, Texture2D sprite, glm::vec2 posTexture);
	void Move(PlayerObject* bullet);
	// Resets the ball to original state with given position and velocity
	void Reset(glm::vec2 position, glm::vec2 velocity);

	GLfloat TextureBoostPosX = 0.0f;
	GLfloat TextureBoostPosY = 1.0f / 4.0f;
	GLfloat TextureBulletPosX = 0.0f;
	GLfloat TextureBulletPosY = 1.0f;
	GLfloat TexturePlayerPosX = 0.0f;
};

#endif