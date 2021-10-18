#ifndef GAME_H
#define GAME_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameObject.h"

enum GameState {
	GAME_ACTIVE,
	GAME_OVER,
	GAME_MENU,
	GAME_TUTORIAL,
	GAME_WIN
};

const GLfloat PLAYER_VELOCITY(200.0f);

class Game
{
public:
	GameState State;
	GameState LastState;
	GLboolean Keys[1024];
	GLuint Width, Height;
	GLboolean SpacePressed;
	Game(GLuint width, GLuint height);
	~Game();

	// Initialize game state (load all shaders/textures/levels)
	void Init();
	void SetSpriteRenderer();
	void SetBullet();
	void SetSatellite();
	void SetCoating_One();
	void SetCoating_Two();
	void SetCoating_Three();
	void SetLifes();
	void SetTutorial();
	void SetStartGame();
	void SetGameOver();
	void SetBullets();
	void SetPlayer();
	void LoadTextureCooldown();
	void LoadTextureScreen();
	void LoadTextureFire();
	void LoadTextureSpaceship();
	void LoadTextureBullets();
	void LoadTextureSatellite();
	void LoadTextureBackground();
	void LoadTextureHealthBar();
	void LoadShader();
	// GameLoop
	void ProcessInput(GLfloat dt, bool cooldown);
	void CoolDown(bool cooldown);
	void InitializeObjects();
	void Update(GLfloat dt);
	void MoveBullet();
	void MoveSpaceship();
	void MoveGround();
	void MoveBackgroud();
	void Render();
	void NumberLifes();
	void BulletsCount();
	void Collisions();
	void CollisionsBullet();
	void CollisionsSatellite();
	void CollisionsBulletsInBirds();
	GLboolean CheckCollision(GameObject& one, GameObject& two);
};

#endif