#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Resource.h"
#include <irrKlang.h>
#include <time.h>
#include <iostream>
#include <string.h>

using namespace irrklang;
using namespace std;

bool notPressed = true;
bool onceGameOver = true;
bool ready = true;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
const GLuint SCREEN_WIDTH = 1280;
const GLuint SCREEN_HEIGHT = 720;

Game Game2D(SCREEN_WIDTH, SCREEN_HEIGHT);
ISoundEngine* SoundEngine = createIrrKlangDevice();

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game 2D", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	cout << endl;
	cout << "Game 2D" << endl;

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game2D.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	Game2D.State = GAME_MENU;

	//SoundEngine->play2D("audio/breakout.mp3", true);

	int cooldown = 0;
	int tick = 0;

	while (!glfwWindowShouldClose(window))
	{
		if (Game2D.State == GAME_OVER and onceGameOver) {
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/gameOver.mp3", true);
			onceGameOver = false;
		}

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Pequeno delay para atirar e nao conseguir segurar espaço - muitas balas atirando ao mesmo tempo
		if (cooldown % 80 == 0) {
			ready = true;
		}
		if (ready and tick == 0) {
			if (!notPressed) {
				tick == 1;
				ready = false;
				notPressed = true;
			}
			Game2D.ProcessInput(deltaTime, false);
		}
		else if (tick == 1) {
			tick == 0;
			Game2D.ProcessInput(deltaTime, true);
		}
		else {
			Game2D.ProcessInput(deltaTime, true);
		}
		cooldown++;

		// Update Game state
		Game2D.Update(deltaTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Game2D.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	Resource::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (key == GLFW_KEY_SPACE and action == GLFW_PRESS and Game2D.State == GAME_ACTIVE and ready) {
			Game2D.SpacePressed = true;
			notPressed = false;
		}
		else if (key == GLFW_KEY_R and action == GLFW_PRESS and Game2D.State == GAME_OVER) {
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/breakout.mp3", true);
			Game2D.Keys[key] = GL_TRUE;
			onceGameOver = true;
		}
		else if (key == GLFW_KEY_ENTER and action == GLFW_PRESS and Game2D.State == GAME_OVER) {
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/breakout.mp3", true);
			Game2D.Keys[key] = GL_TRUE;
			onceGameOver = true;
		}
		else if (action == GLFW_PRESS)
			Game2D.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Game2D.Keys[key] = GL_FALSE;
		else if (action == GLFW_REPEAT)
			Game2D.Keys[key] = GL_TRUE;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#pragma once