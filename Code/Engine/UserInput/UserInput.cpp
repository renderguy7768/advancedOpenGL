#include "UserInput.h"
#include "../../External/FreeGLUT/Includes/freeglut.h"

namespace
{
	void keyboard(unsigned char c, int x, int y);
	void mouse(int button, int state, int x, int y);
	void close(void);
}

bool cs6610::UserInput::Initialize(void)
{
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(close);
	return true;
}

namespace
{
	void keyboard(unsigned char c, int x, int y)
	{
		switch (c)
		{
		case VK_ESCAPE:
			glutLeaveMainLoop();
		default:
			break;
		}
	}
	void mouse(int button, int state, int x, int y)
	{
		//Will be implemented afterwards for camera
	}
	void close(void)
	{
		glutLeaveMainLoop();
	}
}