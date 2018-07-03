#ifndef DISPLAY_H
#define DISPLAY_H

#include "GL\glew.h"
#include <GLFW/glfw3.h>

#include <string>

class Display
{
public:
	struct MouseInput
	{
		float wheelDelta;
		float wheelX;
		float wheelY;
		bool scrolls;
	};
public:
	Display();
	Display(const std::string& title, unsigned int width, unsigned int height);
	virtual ~Display();

	void updateView(unsigned int width, unsigned int height);
	void processEvents();

	void setTitle(const std::string& title);
	void setTitleSufix(const std::string& sufix);

	GLFWwindow* getWindowPtr();
	float getWidth() const;
	float getHeight() const;
	float getRatio() const;

	MouseInput getMouseInput() const;

	bool isOpen() const;
	void endFrame();

public:
	static Display* displayPtr;
	bool sizeUpdated;

private:
	void init();
	static void resizeCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	std::string title;

	MouseInput mouseInput;
};

#endif
