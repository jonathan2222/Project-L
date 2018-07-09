#ifndef DISPLAY_H
#define DISPLAY_H

#include "GL\glew.h"
#include <GLFW/glfw3.h>

#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>

#include "DisplayConfig.h"

class Display
{
public:
	Display();
	Display(const std::string& title, unsigned int width = DEFAULT_DISPLAY_WIDTH, unsigned int height = DEFAULT_DISPLAY_HEIGHT);
	virtual ~Display();

	void updateView(unsigned int width, unsigned int height);

	void setTitle(const std::string& title);
	void setTitleSufix(const std::string& sufix);

	GLFWwindow* getWindowPtr();
	float getWidth() const;
	float getHeight() const;
	float getRatio() const;
	float getPixelXScale() const;
	float getPixelYScale() const;

	FT_Library& getFTLibrary();

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

	FT_Library ft;
};

#endif
