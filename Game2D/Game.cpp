#include <time.h>
#include <iostream>
#include <string.h>
#include <irrKlang.h>
#include "Game.h"
#include "Resource.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "ObstacleObject.h"
#include "BackgroundObject.h"

GLint numLifes = 3;
GLint bullets = 3;
int points = 0;

int indexBullets = 0;
bool bulletCooldown = false;
const int MAX_INDEX = 1000;
int spriteSpeed = 0;

using namespace irrklang;
using namespace std;

ISoundEngine* SoundEngine2 = createIrrKlangDevice();
bool once = true;

SpriteRenderer* Renderer;
SpriteRenderer* RendererPlayer;
SpriteRenderer* RendererBoost;

BackgroundObject* Coating_Three_One;
BackgroundObject* Coating_Three_Two;
BackgroundObject* Coating_Two_One;
BackgroundObject* Coating_Two_Two;
BackgroundObject* Coating_One_One;
BackgroundObject* Coating_One_Two;

PlayerObject* Spaceship;
PlayerObject* Fire;
PlayerObject* Bullets[MAX_INDEX];

ObstacleObject* Satellite_One;
ObstacleObject* Satellite_Two;
ObstacleObject* Satellite_Three;
ObstacleObject* Bullet;
ObstacleObject* Cooldown;

GameObject* LifeBar;
GameObject* BulletCount;
GameObject* GameOver;
GameObject* StartGame;
GameObject* Tutorial;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height), SpacePressed(false)
{
}

Game::~Game()
{
	delete Renderer;
	delete Spaceship;
}

void Game::Init()
{
	// Load shaders
	this->LoadShader();
	// Load textures
	this->LoadTextureHealthBar();
	this->LoadTextureBackground();
	this->LoadTextureSatellite();
	this->LoadTextureBullets();
	this->LoadTextureSpaceship();
	this->LoadTextureFire();
	this->LoadTextureScreen();
	this->LoadTextureCooldown();

	this->SetSpriteRenderer();
	this->SetPlayer();
	this->SetBullets();
	this->SetGameOver();
	this->SetStartGame();
	this->SetTutorial();
	this->SetLifes();
	this->SetCoating_Three();
	this->SetCoating_Two();
	this->SetCoating_One();
	this->SetSatellite();
	this->SetBullet();
}

void Game::SetSpriteRenderer()
{
	Shader shader1 = Resource::GetShader("sprite");
	Renderer = new SpriteRenderer(shader1);
}

void Game::SetBullet()
{
	Bullet = new ObstacleObject(glm::vec2(((this->Width / 5) * 11), 300), glm::vec2(50, 150), Resource::GetTexture("Bullet_Up"));
}

void Game::SetSatellite()
{
	Satellite_One = new ObstacleObject(glm::vec2(((this->Width / 10) * 9), 100), glm::vec2(80, 80), Resource::GetTexture("Satellite"));
	Satellite_Two = new ObstacleObject(glm::vec2(((this->Width / 10) * 17), 120), glm::vec2(65, 60), Resource::GetTexture("Satellite"));
	Satellite_Three = new ObstacleObject(glm::vec2(((this->Width / 10) * 7), 170), glm::vec2(70, 50), Resource::GetTexture("Satellite"));
}

void Game::SetCoating_One()
{
	Coating_One_One = new BackgroundObject(glm::vec2(0, 636), glm::vec2(1280, 84), Resource::GetTexture("Coating_One"), glm::vec2(4.0f, 0.0f));
	Coating_One_Two = new BackgroundObject(glm::vec2(1280, 636), glm::vec2(1280, 84), Resource::GetTexture("Coating_One"), glm::vec2(4.0f, 0.0f));
}

void Game::SetCoating_Two()
{
	Coating_Two_One = new BackgroundObject(glm::vec2(0, 0), glm::vec2(1280, this->Height), Resource::GetTexture("Coating_Two"), glm::vec2(0.8f, 0.0f));
	Coating_Two_Two = new BackgroundObject(glm::vec2(1280, 0), glm::vec2((this->Width), (this->Height)), Resource::GetTexture("Coating_Two"), glm::vec2(0.8f, 0.0f));
}

void Game::SetCoating_Three()
{
	Coating_Three_One = new BackgroundObject(glm::vec2(0, 0), glm::vec2(1280, this->Height), Resource::GetTexture("Coating_Three"), glm::vec2(0.5f, 0.0f));
	Coating_Three_Two = new BackgroundObject(glm::vec2(1280, 0), glm::vec2((this->Width), (this->Height)), Resource::GetTexture("Coating_Three"), glm::vec2(0.5f, 0.0f));
}

void Game::SetLifes()
{
	LifeBar = new GameObject(glm::vec2(0, 0), glm::vec2(800, 400), Resource::GetTexture("Life_Three"));
}

void Game::SetTutorial()
{
	Tutorial = new GameObject(glm::vec2(0, 1280), glm::vec2(this->Width, this->Height), Resource::GetTexture("Tutorial"));
}

void Game::SetStartGame()
{
	StartGame = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("StartGame"));
}

void Game::SetGameOver()
{
	GameOver = new GameObject(glm::vec2(1280, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("GameOver"));
}

void Game::SetBullets()
{
	BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Six"));
}

void Game::SetPlayer()
{
	Spaceship = new PlayerObject(glm::vec2(200, 300), glm::vec2(140, 45), Resource::GetTexture("Spaceship"), glm::vec2(0.0f, 0.0f));
	Fire = new PlayerObject(glm::vec2(170, 310), glm::vec2(60, 24), Resource::GetTexture("Fire"), glm::vec2(0.0f, 0.0f));
}

void Game::LoadTextureCooldown()
{
	Resource::LoadTexture("textures/Verde.png", GL_TRUE, "Verde");
	Resource::LoadTexture("textures/Vermelho.png", GL_TRUE, "Vermelho");
}

void Game::LoadTextureScreen()
{
	Resource::LoadTexture("textures/GameOver.png", GL_TRUE, "GameOver");
	Resource::LoadTexture("textures/Menu.png", GL_TRUE, "StartGame");
	Resource::LoadTexture("textures/Tutorial.png", GL_TRUE, "Tutorial");
}

void Game::LoadTextureFire()
{
	Resource::LoadTexture("textures/BoostSheet.png", GL_TRUE, "Fire");
}

void Game::LoadTextureSpaceship()
{
	Resource::LoadTexture("textures/Spaceship.png", GL_TRUE, "Spaceship");
}

void Game::LoadTextureBullets()
{
	Resource::LoadTexture("textures/Bullet_Up.png", GL_TRUE, "Bullet_Up");
	Resource::LoadTexture("textures/Bullet.png", GL_TRUE, "Bullet");
	Resource::LoadTexture("textures/Bullet_Six.png", GL_TRUE, "Bullet_Six");
	Resource::LoadTexture("textures/Bullet_Five.png", GL_TRUE, "Bullet_Five");
	Resource::LoadTexture("textures/Bullet_Four.png", GL_TRUE, "Bullet_Four");
	Resource::LoadTexture("textures/Bullet_Three.png", GL_TRUE, "Bullet_Three");
	Resource::LoadTexture("textures/Bullet_Two.png", GL_TRUE, "Bullet_Two");
	Resource::LoadTexture("textures/Bullet_One.png", GL_TRUE, "Bullet_One");
	Resource::LoadTexture("textures/White.png", GL_TRUE, "White");
}

void Game::LoadTextureSatellite()
{
	Resource::LoadTexture("textures/Satellite.png", GL_TRUE, "Satellite");
}

void Game::LoadTextureBackground()
{
	Resource::LoadTexture("textures/Coating_One.png", GL_TRUE, "Coating_One");
	Resource::LoadTexture("textures/Coating_Two.png", GL_TRUE, "Coating_Two");
	Resource::LoadTexture("textures/Coating_Three.png", GL_TRUE, "Coating_Three");
}

void Game::LoadTextureHealthBar()
{
	Resource::LoadTexture("textures/Life_Three.png", GL_TRUE, "Life_Three");
	Resource::LoadTexture("textures/Life_Two.png", GL_TRUE, "Life_Two");
	Resource::LoadTexture("textures/Life_One.png", GL_TRUE, "Life_One");
}

void Game::LoadShader()
{
	Resource::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	Resource::GetShader("sprite").Use().SetInteger("sprite", 0);
	Resource::GetShader("sprite").SetMatrix4("projection", projection);
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE) {
		this->MoveBackgroud();
		this->MoveGround();
		this->MoveSpaceship();
		this->MoveBullet();

		for (int i = 0; i < indexBullets; i++) {
			Bullets[i]->Move(Bullets[i]);
		}

		this->Collisions();
	}
}

void Game::MoveBullet()
{
	Bullet->Move(Bullet, false);
}

void Game::MoveSpaceship()
{
	Satellite_One->Move(Satellite_One, true);
	Satellite_Two->Move(Satellite_Two, true);
	Satellite_Three->Move(Satellite_Three, true);
}

void Game::MoveGround()
{
	Coating_One_One->Move(Coating_One_One);
	Coating_One_Two->Move(Coating_One_Two);
}

void Game::MoveBackgroud()
{
	Coating_Three_One->Move(Coating_Three_One);
	Coating_Three_Two->Move(Coating_Three_Two);
	Coating_Two_One->Move(Coating_Two_One);
	Coating_Two_Two->Move(Coating_Two_Two);
}

void Game::ProcessInput(GLfloat dt, bool cooldown)
{
	// Fast reset
	if (this->Keys[GLFW_KEY_R] and this->State == GAME_OVER) {
		this->Init();
		this->State = GAME_ACTIVE;
		numLifes = 3;
		bullets = 3;
		indexBullets = 0;
		points = 0;
		once = true;
		SoundEngine2->play2D("audio/press.mp3", false);
	}
	// Menu
	if (this->Keys[GLFW_KEY_ENTER] and this->State == GAME_OVER) {
		this->Init();
		StartGame->Position.x = 0.0f;
		this->State = GAME_MENU;
		SoundEngine2->play2D("audio/press.mp3", false);
	}
	// Start
	else if (this->Keys[GLFW_KEY_ENTER] and this->State == GAME_MENU) {
		this->Init();
		this->State = GAME_ACTIVE;
		numLifes = 3;
		bullets = 3;
		indexBullets = 0;
		points = 0;
		once = true;
		SoundEngine2->play2D("audio/press.mp3", false);
	}
	// Tutorial
	if (this->Keys[GLFW_KEY_T] and this->State == GAME_MENU) {
		Tutorial->Position.y = 0.0f;
		this->State = GAME_TUTORIAL;
		SoundEngine2->play2D("audio/press.mp3", false);
	}
	// Return Menu
	if (this->Keys[GLFW_KEY_V] and this->State == GAME_TUTORIAL) {
		Tutorial->Position.y = 1280.0f;
		StartGame->Position.x = 0.0f;
		this->State = GAME_MENU;
		SoundEngine2->play2D("audio/press.mp3", false);
	}

	// Game start
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		this->InitializeObjects();

		if (!Spaceship->Stuck) {
			if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
			{
				if (Spaceship->Position.y >= 0) {
					Spaceship->Position.y -= velocity;
					Fire->Position.y -= velocity;
				}
			}
			if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
			{
				if (Spaceship->Position.y <= this->Height - 120.0f) {
					Spaceship->Position.y += velocity;
					Fire->Position.y += velocity;
				}
			}
			if (this->SpacePressed and !cooldown)
			{
				glm::vec2 bulletPos = glm::vec2(320, 320);
				PlayerObject* Bullet = new PlayerObject(bulletPos, glm::vec2(50, 20), Resource::GetTexture("Bullet"), glm::vec2(12.0f, 0.0f));

				Bullet->Stuck = false;
				Bullet->Position.x = Spaceship->Position.x + 120.0f;
				Bullet->Position.y = Spaceship->Position.y + 20.0f;

				if (bullets == 0)
					SoundEngine2->play2D("audio/solid.mp3", false);
				else
					SoundEngine2->play2D("audio/bullet.mp3", false);
				if (indexBullets < MAX_INDEX and bullets > 0) {
					Bullets[indexBullets++] = Bullet;
					bullets -= 1;
				}

				this->SpacePressed = false;
			}
		}
	}

	// Cooldown
	this->CoolDown(cooldown);
}

void Game::CoolDown(bool cooldown)
{
	if (cooldown) {
		Cooldown = new ObstacleObject(glm::vec2(20, 650), glm::vec2(50, 50), Resource::GetTexture("Vermelho"));
	}
	else {
		Cooldown = new ObstacleObject(glm::vec2(20, 650), glm::vec2(50, 50), Resource::GetTexture("Verde"));
	}
}

void Game::InitializeObjects()
{
	StartGame->Position.x = 1280;
	Tutorial->Position.y = 1280;
	Spaceship->Stuck = false;
	Fire->Stuck = false;
	Coating_Three_One->Stuck = false;
	Coating_Three_Two->Stuck = false;
	Coating_Two_One->Stuck = false;
	Coating_Two_Two->Stuck = false;
	Coating_One_One->Stuck = false;
	Coating_One_Two->Stuck = false;
	Satellite_One->Stuck = false;
	Satellite_Two->Stuck = false;
	Satellite_Three->Stuck = false;
	Bullet->Stuck = false;
	Cooldown->Stuck = false;
}

void Game::Render()
{
	if (this->State == GAME_MENU)
		StartGame->Draw(*Renderer, 0.02f);
	if (this->State == GAME_TUTORIAL)
		Tutorial->Draw(*Renderer, 0.02f);
	if (this->State == GAME_OVER)
		GameOver->Draw(*Renderer, 0.02f);
	if (this->State == GAME_ACTIVE)
	{
		// Player sprite init
		Shader shader1 = Resource::GetShader("sprite");
		RendererPlayer = new SpriteRenderer(shader1, Spaceship->TexturePlayerPosX, Spaceship->TexturePlayerPosX + 1.0f, false);

		if (Fire->TextureBoostPosX >= 3.0f / 4.0f)
			Fire->TextureBoostPosX = 0.0f; Fire->TextureBoostPosY = 1.0f / 4.0f;
		if (spriteSpeed % 10 == 0)
			RendererBoost = new SpriteRenderer(shader1, Fire->TextureBoostPosX += 1.0f / 4.0f, Fire->TextureBoostPosY += 1.0f / 4.0f, true);
		else
			RendererBoost = new SpriteRenderer(shader1, Fire->TextureBoostPosX, Fire->TextureBoostPosY, true);

		// Background
		Coating_Three_One->Draw(*Renderer, 0.2f);
		Coating_Three_Two->Draw(*Renderer, 0.2f);
		Coating_Two_One->Draw(*Renderer, 0.1f);
		Coating_Two_Two->Draw(*Renderer, 0.1f);

		// Ammo
		this->BulletsCount();
		if (bullets != 0)
			BulletCount->Draw(*Renderer, 0.2f);

		// Health Bar
		this->NumberLifes();
		LifeBar->Draw(*Renderer, 0.2f);

		// Ground
		Coating_One_One->Draw(*Renderer, 0.02f);
		Coating_One_Two->Draw(*Renderer, 0.02f);

		// Cooldown
		Cooldown->Draw(*Renderer, 0.2f);

		// Number Birds
		if (!Satellite_One->Destroyed)
			Satellite_One->Draw(*Renderer, 0.03f);
		if (!Satellite_Two->Destroyed)
			Satellite_Two->Draw(*Renderer, 0.03f);
		if (!Satellite_Three->Destroyed)
			Satellite_Three->Draw(*Renderer, 0.03f);

		// Bala Up
		if (!Bullet->Destroyed)
			Bullet->Draw(*Renderer, 0.02f);

		// Player
		Spaceship->DrawPlayer(*RendererPlayer, 0.2f);
		Fire->DrawPlayer(*RendererBoost, 0.2f);

		// Bullets
		for (int i = 0; i < indexBullets; i++) {
			if (Bullets[i]->Position.x <= 1400.0f and !Bullets[i]->Destroyed)
				Bullets[i]->Draw(*Renderer, 0.2f);
		}

		// Control boost sprite speed
		spriteSpeed++;
	}
}

void Game::NumberLifes()
{
	if (numLifes == 3)
		LifeBar->Draw(*Renderer, 0.2f);
	else if (numLifes == 2)
		LifeBar = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(800, 400), Resource::GetTexture("Life_Two"));
	else if (numLifes == 1)
		LifeBar = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(800, 400), Resource::GetTexture("Life_One"));
	else {
		if (once) {
			SoundEngine2->play2D("audio/explosion.mp3", false);
			once = false;
		}
		GameOver->Position.x = 0.0f;
		this->State = GAME_OVER;
		indexBullets = 0;
		cout << "Pontuacao Total: " << points << endl;
	}
}

void Game::BulletsCount()
{
	if (bullets == 6)
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Six"));
	else if (bullets == 5)
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Five"));
	else if (bullets == 4)
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Four"));
	else if (bullets == 3)
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Three"));
	else if (bullets == 2)
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_Two"));
	else
		BulletCount = new GameObject(glm::vec2(0, 0), glm::vec2(this->Width, this->Height), Resource::GetTexture("Bullet_One"));
}


void Game::Collisions() {
	this->CollisionsBirds();
	this->CollisionsBullet();
	this->CollisionsBulletsInBirds();
}

void Game::CollisionsBullet() {

	if (CheckCollision(*Spaceship, *Bullet))
	{
		if (!Bullet->Destroyed) {
			Bullet->Destroyed = true;
			if (bullets < 6)
				bullets += 1;
			SoundEngine2->play2D("audio/powerup.mp3", false);
			points += 5;
		}
	}
}

void Game::CollisionsBirds() {
	if (CheckCollision(*Spaceship, *Satellite_One))
	{
		if (!Satellite_One->Destroyed) {
			Satellite_One->Destroyed = true;
			numLifes -= 1;
			SoundEngine2->play2D("audio/health.mp3", false);
		}
	}
	else if (CheckCollision(*Spaceship, *Satellite_Two))
	{
		if (!Satellite_Two->Destroyed) {
			Satellite_Two->Destroyed = true;
			numLifes -= 1;
			SoundEngine2->play2D("audio/health.mp3", false);
		}
	}
	else if (CheckCollision(*Spaceship, *Satellite_Three))
	{
		if (!Satellite_Three->Destroyed) {
			Satellite_Three->Destroyed = true;
			numLifes -= 1;
			SoundEngine2->play2D("audio/health.mp3", false);
		}
	}
}

void Game::CollisionsBulletsInBirds() {
	for (int i = 0; i < indexBullets; i++) {
		if (CheckCollision(*Satellite_One, *Bullets[i])) {
			if (!Satellite_One->Destroyed and !Bullets[i]->Destroyed) {
				Satellite_One->Destroyed = true;
				Bullets[i]->Destroyed = true;
				SoundEngine2->play2D("audio/destroy.mp3", false);
				points += 15;
			}
		}
		else if (CheckCollision(*Satellite_Two, *Bullets[i])) {
			if (!Satellite_Two->Destroyed and !Bullets[i]->Destroyed) {
				Satellite_Two->Destroyed = true;
				Bullets[i]->Destroyed = true;
				SoundEngine2->play2D("audio/destroy.mp3", false);
				points += 15;
			}
		}
		else if (CheckCollision(*Satellite_Three, *Bullets[i])) {
			if (!Satellite_Three->Destroyed and !Bullets[i]->Destroyed) {
				Satellite_Three->Destroyed = true;
				Bullets[i]->Destroyed = true;
				SoundEngine2->play2D("audio/destroy.mp3", false);
				points += 15;
			}
		}
	}
}

GLboolean Game::CheckCollision(GameObject& one, GameObject& two)
{
	GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}