#include "PlayerObject.h"

PlayerObject::PlayerObject()
	: GameObject(), Stuck(true) { }

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
	: GameObject(pos,      
		size,			
		sprite,            
		glm::vec3(1.0f),   
		velocity),		
	Stuck(true) { }

void PlayerObject::Move(PlayerObject* bullet) {
	if (!bullet->Stuck) {
		bullet->Position.x += bullet->Velocity.x;
	}
}