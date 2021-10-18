#include "BackgroundObject.h"

BackgroundObject::BackgroundObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
	: GameObject(pos,              //posicao
		size,                      //size
		sprite,                    //sprite
		glm::vec3(1.0f),           //color
		velocity),					//velocity
	Stuck(true) { }

void BackgroundObject::Move(BackgroundObject* background)
{
	if (!background->Stuck) {
		background->Position.x -= background->Velocity.x;

		if (background->Position.x <= -1279.0f) {
			background->Position.x = 1279.0f;
		}
	}
}