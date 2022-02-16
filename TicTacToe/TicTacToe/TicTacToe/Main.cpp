// Careful with order of #include

#include "matrix.h"
#include "MyPhoton.h"
#include "TicTacToe.h"
#include "Application.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <mutex>
//#include <chrono>

#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

const int RESOLUTION_X = 1280;
const int RESOLUTION_Y = 720;

std::mutex MutexLock;

static int BoardLocation = -1;
static double MouseX = 0;
static double MouseY = 0;

static double BoundariesX[4][2] = 
{
	// Boxes boundaries
	{ 345, 515 },
	{ 555, 725 },
	{ 765, 935 },
	// ================
	// Reset
	{ 20, 90 }
};
static double BoundariesY[4][2] = 
{
	// Boxes boundaries
	{ 65, 234 },
	{ 275, 445 },
	{ 485, 655 },
	// ================
	// Reset
	{ 15, 85 }
};

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Ue orthod 2D view
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Change affector
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		//ParticleSystem::particleAffector = ParticleSystem::gravityAffector;
		if (ParticleSystem::toggleGravity) ParticleSystem::toggleGravity = false;
		else ParticleSystem::toggleGravity = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		//ParticleSystem::particleAffector = ParticleSystem::colorAffector;
		if (ParticleSystem::toggleColor) ParticleSystem::toggleColor = false;
		else ParticleSystem::toggleColor = true;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		//ParticleSystem::particleAffector = ParticleSystem::scaleAffector;
		if (ParticleSystem::toggleScale) ParticleSystem::toggleScale = false;
		else ParticleSystem::toggleScale = true;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		//ParticleSystem::particleAffector = ParticleSystem::rotatorAffector;
		if (ParticleSystem::toggleRotator) ParticleSystem::toggleRotator = false;
		else ParticleSystem::toggleRotator = true;
	}

	// Set velocity
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (ParticleSystem::velocityTrigger) ParticleSystem::velocityTrigger = false;
		else ParticleSystem::velocityTrigger = true;
	}

	// Change emitter
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		ParticleSystem::emitterShape = ParticleSystem::circleEmitter;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		ParticleSystem::emitterShape = ParticleSystem::ringEmitter;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		ParticleSystem::emitterShape = ParticleSystem::boxEmitter;
	}
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		glfwGetCursorPos(window, &MouseX, &MouseY);
		//std::cout << "MouseX: " << MouseX << ", MouseY: " << MouseY << std::endl;

		if (MouseX > BoundariesX[3][0] && MouseX < BoundariesX[3][1])
		{
			MyPhoton::getInstance().SendGameAnswer(10);
			return;
		}

		// If want to use single arrays use the formulas (x * 2) & (y * 2) + 1 to get the arrray location
		for (int x = 0; x < 3; x++)
		{
			if (MouseX > BoundariesX[x][0] && MouseX < BoundariesX[x][1])
			{
				for (int y = 0; y < 3; y++)
				{
					if (MouseY > BoundariesY[y][0] && MouseY < BoundariesY[y][1])
					{
						BoardLocation = x + (y * 3);
						//std::cout << "Clicked box [" << BoardLocation + 1 << "].\n";
						MyPhoton::getInstance().SendGameAnswer(BoardLocation);
						return;
					}
				}
			}
		}
		//std::cout << "Clicked outside of box.\n";
		MyPhoton::getInstance().SendGameAnswer(9);
	}
}

void MessengerFunc()
{
	std::string inputString;
	while (true)
	{
		//std::wcout << "Send: ";
		getline(std::cin, inputString);

		MutexLock.lock();
		MyPhoton::getInstance().SendStringMessage(inputString);
		MutexLock.unlock();

		Sleep(100);
	}
}
void MessengerByteFunc()
{
	while (1)
	{
		std::string inputString;
		getline(std::cin, inputString);

		//std::cout << "Me : " << inputString << std::endl;
		// Encrypt message
		for (int i = 0; i < inputString.length(); i++)
		{
			inputString[i] = inputString[i] ^ CharCipherKey;
		}
		//
		//std::cout << "Me [Encrypted]: " << inputString << "\n\n";

		MutexLock.lock();
		//MyPhoton::getInstance().sendString(mystring.c_str());
		MyPhoton::getInstance().SendByteMessage(inputString.c_str(), inputString.length());
		MutexLock.unlock();

		Sleep(100);
	}
}

int main(void)
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "TicTacToe", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);

	glfwSwapInterval(0); //enable/disable vsync

	// Create main tic-tac-toe game
	TicTacToe MainGame;

	double maxFPS = 60.0;
	double minDeltaTime = 1.0 / maxFPS;

	double deltaTime = 0.0;
	double fps = 0.0;

	double currentTime = 0.0;
	double prevTime = glfwGetTime();
	double printerTime = glfwGetTime();

	MyPhoton::getInstance().SetTicTacToe(&MainGame);
	MyPhoton::getInstance().connect();

	std::thread MessageThread(&MessengerByteFunc);

	// ==Game loop==
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		
		// Print delta time and FPS every 1 second in console
		/*if (currentTime - printerTime > 1.0)
		{
			app.update(deltaTime, fps);
			printerTime = currentTime;
		}*/

		// ==================================
		// Lecturer's sleeper
		int minSleepTimeTH = 10;
		// Conservatively asking the system to sleep
		int sleepTime = (minDeltaTime - deltaTime) * 1000 - minSleepTimeTH;
		if (sleepTime >= minSleepTimeTH)
		{
			Sleep(sleepTime);
		}
		// Sleep time is not precise, use loop to "waste" extra cpu cycles
		while (deltaTime < minDeltaTime)
		{
			deltaTime = glfwGetTime() - prevTime;
		}
		// ==================================

		// Lecturer's printer
		prevTime = glfwGetTime();
		fps = 1.0 / deltaTime;
		char str[32];
		sprintf_s(str, "TicTacToe: %g, %g", fps, deltaTime);
		glfwSetWindowTitle(window, str);

		//app.update(deltaTime, fps); // Update delta time and FPS in console

		MainGame.DrawBoard(deltaTime);

		MutexLock.lock();
		MyPhoton::getInstance().run();
		MutexLock.unlock();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	MessageThread.join();

	glfwTerminate();
	return 0;
}