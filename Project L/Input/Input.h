#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include "../Maths/Vectors/Vec2.h"

#include <iostream>

class Input
{
public:
	static void init(GLFWwindow* window);

	static bool isKeyPressed(int key);
	// TODO: not the best way to do it. can only call it one time in one frame
	static bool isKeyClicked(int key);
	static bool isKeyReleased(int key);

	static bool isButtonPressed(int button);
	static bool isButtonReleased(int button);
	static bool isButtonClicked(int button);

	// Mouse position
	static double getMousePosX();
	static double getMousePosY();
	static Vec2 getMousePosition();

	// Scrolling
	static int getScrollXOffest();
	static int getScrollYOffest();
	static Vec2 getScrollOffset();
	// TODO: not the best way to do it. can only call it one time in one frame
	static bool isScrolling();

	// Cursor information
	static bool isCursorInWindow();
	static bool isCursorHiden();
	static bool isCursorNormal();
	static bool isCursorDisable();
	static void hideCursor();
	static void restoreCursor();
	static void disableCursor();

	// Modifier key flags
	static bool isShiftDown();
	static bool isControlDown();
	static bool isAltDown();

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void cursor_enter_callback(GLFWwindow* window, int entered);

	static void key_pressed(int key);
	static void key_repeat(int key);
	static void key_release(int key);

	static void mouse_pressed(int button);
	static void mouse_release(int button);

private:
	static GLFWwindow * win;

	static bool pressedKeys[GLFW_KEY_LAST];
	static bool pressedPreKeys[GLFW_KEY_LAST];
	static bool pressedButtons[GLFW_MOUSE_BUTTON_LAST];
	static bool pressedPreButtons[GLFW_MOUSE_BUTTON_LAST];
	static int mods;
	static double posX;
	static double posY;
	static int scrollX;
	static int scrollY;
	static bool scrolling;
	static bool cursorInWindow;
	static int cursorMode;
};

#endif