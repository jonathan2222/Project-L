#include "Input.h"

GLFWwindow * Input::win = nullptr;

bool Input::pressedKeys[GLFW_KEY_LAST] = { false };
bool Input::pressedPreKeys[GLFW_KEY_LAST] = { false };
bool Input::pressedButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Input::pressedPreButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
int Input::mods = 0;

double Input::posX = 0.0;
double Input::posY = 0.0;
int Input::scrollX = 0;
int Input::scrollY = 0;
bool Input::scrolling = false;
bool Input::cursorInWindow = false;
int Input::cursorMode = 0;

void Input::init(GLFWwindow* window)
{
	win = window;
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
}

bool Input::isKeyPressed(int key)
{
	return pressedKeys[key];
}

bool Input::isKeyClicked(int key)
{
	bool ret = pressedKeys[key] && !pressedPreKeys[key];
	pressedPreKeys[key] = true;
	return ret;
}

bool Input::isKeyReleased(int key)
{
	return !pressedKeys[key];
}

bool Input::isButtonPressed(int button)
{
	return pressedButtons[button];
}

bool Input::isButtonReleased(int button)
{
	return !pressedButtons[button];
}

bool Input::isButtonClicked(int button)
{
	bool ret = pressedButtons[button] && !pressedPreButtons[button];
	pressedPreButtons[button] = true;
	return ret;
}

double Input::getMousePosX()
{
	return posX;
}

double Input::getMousePosY()
{
	return posY;
}

Vec2 Input::getMousePosition()
{
	return Vec2(posX, posY);
}

int Input::getScrollXOffest()
{
	return scrollX;
}

int Input::getScrollYOffest()
{
	return scrollY;
}

Vec2 Input::getScrollOffset()
{
	return Vec2(scrollX, scrollY);
}

bool Input::isScrolling()
{
	bool ret = scrolling;
	scrolling = false;
	return ret;
}

bool Input::isCursorInWindow()
{
	return cursorInWindow;
}

bool Input::isCursorHiden()
{
	return cursorMode == 1;
}

bool Input::isCursorNormal()
{
	return cursorMode == 0;
}

bool Input::isCursorDisable()
{
	return cursorMode == 2;
}

void Input::hideCursor()
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	cursorMode = 1;
}

void Input::restoreCursor()
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	cursorMode = 0;
}

void Input::disableCursor()
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursorMode = 2;
}

bool Input::isShiftDown()
{
	return mods & GLFW_MOD_SHIFT;
}

bool Input::isControlDown()
{
	return mods & GLFW_MOD_CONTROL;
}

bool Input::isAltDown()
{
	return mods & GLFW_MOD_ALT;
}

void Input::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	mods = mods;
	switch (action)
	{
	case GLFW_PRESS:	key_pressed(key);	break;
	case GLFW_REPEAT:	key_repeat(key);	break;
	case GLFW_RELEASE:	key_release(key);	break;
	}
}

void Input::mouse_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	posX = xpos;
	posY = ypos;
}

void Input::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	mods = mods;
	switch (action)
	{
	case GLFW_PRESS:	mouse_pressed(button);	break;
	case GLFW_RELEASE:	mouse_release(button);	break;
	}
}

void Input::mouse_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	scrolling = true;
	scrollX = xoffset;
	scrollY = yoffset;
}

void Input::cursor_enter_callback(GLFWwindow * window, int entered)
{
	cursorInWindow = entered;
}

void Input::key_pressed(int key)
{
	pressedPreKeys[key] = pressedKeys[key];
	pressedKeys[key] = true;
}

void Input::key_repeat(int key)
{
}

void Input::key_release(int key)
{
	pressedKeys[key] = false;
}

void Input::mouse_pressed(int button)
{
	pressedPreButtons[button] = pressedButtons[button];
	pressedButtons[button] = true;
}

void Input::mouse_release(int button)
{
	pressedButtons[button] = false;
}
