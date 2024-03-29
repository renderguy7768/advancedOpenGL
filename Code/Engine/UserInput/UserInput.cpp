#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"
#include "../../Game/MyGame/MyGame.h"
#include "../Graphics/Effect.h"
#include "../../External/cyCodeBase/cyPoint.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Scene.h"
#include "../Camera/Camera.h"
#include "ConfigurableParameters.h"

namespace
{
	void keyPress(unsigned char c, int x, int y);
	void keyRelease(unsigned char c, int x, int y);
	void keyPressSpecial(int key, int x, int y);
	void keyReleaseSpecial(int key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	//void mouseMotionPassive(int x, int y);
	void close();

	int xPosOnPressLMB = 0, yPosOnPressLMB = 0;
	bool lmbFirstPress = true;
	bool lmbStillPressed = false;

	int xPosOnPressRMB = 0, yPosOnPressRMB = 0;
	bool rmbFirstPress = true;
	bool rmbStillPressed = false;

	int xPosOnPressCtrlLMB = 0, yPosOnPressCtrlLMB = 0;
	bool ctrlLMBFirstPress = true;
	bool ctrlLMBStillPressed = false;

	int xPosOnPressALTLMB = 0, yPosOnPressALTLMB = 0;
	bool altLMBFirstPress = true;
	bool altLMBStillPressed = false;

	int yPosOnPressALTRMB = 0;
	bool altRMBFirstPress = true;
	bool altRMBStillPressed = false;

	int xLast = 0, yLast = 0;
	bool isFirstMouseMove = true;

	cs6610::Gameplay::GameObject* teapot = nullptr;
	cs6610::Gameplay::GameObject* light = nullptr;
	//cs6610::Gameplay::GameObject* plane = nullptr;
	void GetRequiredGameOject();
}

namespace cs6610
{
	namespace UserInput
	{
		std::bitset<256> keys;
	}
}


bool cs6610::UserInput::Initialize()
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutSpecialFunc(keyPressSpecial);
	glutSpecialUpFunc(keyReleaseSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//glutPassiveMotionFunc(mouseMotionPassive);
	glutCloseFunc(close);
	return true;
}

namespace
{
	void keyPress(unsigned char c, int x, int y)
	{
		/*if (c == 'p' && !cs6610::UserInput::keys['p'])
		{
			cs6610::UserInput::isCameraPerspective = !cs6610::UserInput::isCameraPerspective;
		}*/
		if (c == 'r' && !cs6610::UserInput::keys['r'])
		{
			cs6610::ConfigurableParameters::autoLight = !cs6610::ConfigurableParameters::autoLight;
		}
		if (c == VK_ESCAPE)
		{
			cs6610::MyGame::CleanUp();
			cs6610::Graphics::CleanUp();
			glutLeaveMainLoop();
		}
		else
		{
			cs6610::UserInput::keys.set(c);
		}
	}

	void keyRelease(unsigned char c, int x, int y)
	{
		cs6610::UserInput::keys.reset(c);
	}

	void mouse(int button, int state, int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (glutGetModifiers() == GLUT_ACTIVE_CTRL && ctrlLMBFirstPress)
				{
					xPosOnPressCtrlLMB = x;
					yPosOnPressCtrlLMB = y;
					ctrlLMBFirstPress = false;
					ctrlLMBStillPressed = true;
				}
				else if (glutGetModifiers() == GLUT_ACTIVE_ALT && altLMBFirstPress)
				{
					xPosOnPressALTLMB = x;
					yPosOnPressALTLMB = y;
					altLMBFirstPress = false;
					altLMBStillPressed = true;
				}
				else if (lmbFirstPress)
				{
					xPosOnPressLMB = x;
					yPosOnPressLMB = y;
					lmbFirstPress = false;
					lmbStillPressed = true;
				}
			}
			else if (state == GLUT_UP)
			{
				lmbFirstPress = true;
				lmbStillPressed = false;

				ctrlLMBFirstPress = true;
				ctrlLMBStillPressed = false;

				altLMBFirstPress = true;
				altLMBStillPressed = false;
			}
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (glutGetModifiers() == GLUT_ACTIVE_ALT && altRMBFirstPress)
				{
					yPosOnPressALTRMB = y;
					altRMBFirstPress = false;
					altRMBStillPressed = true;
				}
				else if (rmbFirstPress)
				{
					yPosOnPressRMB = y;
					rmbFirstPress = false;
					rmbStillPressed = true;
				}
			}
			else if (state == GLUT_UP)
			{
				rmbFirstPress = true;
				rmbStillPressed = false;

				altRMBFirstPress = true;
				altRMBStillPressed = false;

				isFirstMouseMove = true;
			}
		}
	}

	void mouseMotion(int x, int y)
	{
		if (lmbStillPressed)
		{
			float xOffsetLMB = static_cast<float>(xPosOnPressLMB - x);
			float yOffsetLMB = static_cast<float>(y - yPosOnPressLMB);
			xPosOnPressLMB = x;
			yPosOnPressLMB = y;
			teapot ? teapot->UpdateOrientation(xOffsetLMB, yOffsetLMB) : GetRequiredGameOject();
		}
		if (rmbStillPressed)
		{
			float xOffsetRMB = static_cast<float>(x - xPosOnPressRMB);
			float yOffsetRMB = static_cast<float>(y - yPosOnPressRMB);
			xPosOnPressRMB = x;
			yPosOnPressRMB = y;
			teapot ? teapot->UpdatePosition(xOffsetRMB, yOffsetRMB) : GetRequiredGameOject();
		}
		if (ctrlLMBStillPressed)
		{
			float xOffsetCtrlLMB = static_cast<float>(x - xPosOnPressCtrlLMB);
			float yOffsetCtrlLMB = static_cast<float>(y - yPosOnPressCtrlLMB);
			xPosOnPressCtrlLMB = x;
			yPosOnPressCtrlLMB = y;
			light && !cs6610::ConfigurableParameters::autoLight ? light->UpdatePosition(xOffsetCtrlLMB, yOffsetCtrlLMB) : GetRequiredGameOject();
		}
		if (altLMBStillPressed)
		{
			/*float xOffsetALTLMB = static_cast<float>(xPosOnPressALTLMB - x);
			float yOffsetALTLMB = static_cast<float>(yPosOnPressALTLMB - y);
			xPosOnPressALTLMB = x;
			yPosOnPressALTLMB = y;
			plane ? plane->UpdateOrientation(xOffsetALTLMB, yOffsetALTLMB) : GetRequiredGameOject();*/
		}
		if (altRMBStillPressed)
		{
			//float yOffsetALTRMB = static_cast<float>(yPosOnPressALTRMB - y);
			//yPosOnPressALTRMB = y;
			if (isFirstMouseMove)
			{
				xLast = x;
				yLast = y;
				isFirstMouseMove = false;
			}
			float xOffset = static_cast<float>(x - xLast);
			float yOffset = static_cast<float>(y - yLast);
			xLast = x;
			yLast = y;
			cs6610::MyGame::mainScene->GetCamera()->UpdateCurrentCameraOrientation(xOffset, yOffset);
			//plane ? plane->UpdatePosition(0.0f, yOffsetALTRMB) : GetRequiredGameOject();
		}
		else
		{
			isFirstMouseMove = true;
		}
	}

	/*void mouseMotionPassive(int x, int y)
	{
		if (!lmbStillPressed && !rmbStillPressed && !ctrlLMBStillPressed && !altLMBStillPressed && !altRMBStillPressed)
		{
			if (isFirstMouseMove)
			{
				xLast = x;
				yLast = y;
				isFirstMouseMove = false;
			}
			float xOffset = static_cast<float>(x - xLast);
			float yOffset = static_cast<float>(y - yLast);
			xLast = x;
			yLast = y;
			cs6610::MyGame::mainScene->GetCamera()->UpdateCurrentCameraOrientation(xOffset, yOffset);
		}
	}*/

	void close()
	{
		cs6610::MyGame::CleanUp();
		cs6610::Graphics::CleanUp();
		glutLeaveMainLoop();
	}
	void keyPressSpecial(int key, int x, int y)
	{
		if (key == GLUT_KEY_F6 && !cs6610::UserInput::keys[key])
		{
			cs6610::UserInput::keys.set(key);
			cs6610::Graphics::Effect::ReCompile();
		}
	}
	void keyReleaseSpecial(int key, int x, int y)
	{
		cs6610::UserInput::keys.reset(key);
	}

	void GetRequiredGameOject()
	{
		/*if (!teapot)
		{
			teapot = cs6610::MyGame::mainScene->GetGameobjectByName("Teapot");
		}*/
		if (!light)
		{
			light = cs6610::MyGame::mainScene->GetGameobjectByName("Light");
		}
		/*if (!plane)
		{
			plane = cs6610::MyGame::mainScene->GetGameobjectByName("Plane");
		}*/
	}
}