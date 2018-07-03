#ifndef DISPLAY_H
#define DISPLAY_H

#include "GL\glew.h"
#include "SFML\Window.hpp"
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

	void processEvents();

	void setTitle(const std::string& title);
	void setTitleSufix(const std::string& sufix);

	sf::Window* getWindowPtr();
	float getWidth() const;
	float getHeight() const;

	MouseInput getMouseInput() const;

	bool isOpen() const;

private:
	void init();

	sf::Window* window;
	unsigned int width;
	unsigned int height;
	std::string title;

	MouseInput mouseInput;
};

#endif
