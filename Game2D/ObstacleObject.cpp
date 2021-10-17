#include "ObstacleObject.h"

bool up = true;

ObstacleObject::ObstacleObject()
	: GameObject(), Stuck(true) { }

ObstacleObject::ObstacleObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	: GameObject(pos,                 //posicao
		size,                         //size
		sprite,                       //sprite
		glm::vec3(1.0f),              //color
		glm::vec2(0.8f, 1.0f)),       //velocity
	Stuck(true), firsPos(pos) { }

void ObstacleObject::Move(ObstacleObject* obstacle, bool isBird)
{
	if (!obstacle->Stuck) {
		obstacle->Position.x -= obstacle->Velocity.x;

		if (obstacle->Position.x <= -1280.0f) {
			obstacle->Position.x = 1280.0f;
			obstacle->Destroyed = false;
		}
		if (isBird) {
			int yUp = obstacle->firsPos.y - 200.0f;
			int yDown = obstacle->firsPos.y + 200.0f;
			if (up) {
				if (obstacle->Position.y > yUp)
					obstacle->Position.y -= 0.60f;
				else
					up = false;
			}
			else
			{

				if (obstacle->Position.y <= yDown)
					obstacle->Position.y += 0.60f;
				else
					up = true;
			}
		}
	}
}

void ObstacleObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}