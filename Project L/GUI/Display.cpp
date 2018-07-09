#include "Display.h"

#include "../Utils/Error.h"

Display* Display::displayPtr = nullptr;

Display::Display() : title(DEFAULT_DISPLAY_TITLE), width(DEFAULT_DISPLAY_WIDTH), height(DEFAULT_DISPLAY_HEIGHT)
{
	this->window = nullptr;
	init();
}

Display::Display(const std::string & title, unsigned int width, unsigned int height) : title(title), width(width), height(height)
{
	this->window = nullptr;
	init();
}

Display::~Display()
{
	FT_Done_FreeType(this->ft);
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Display::updateView(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	glViewport(0, 0, this->width, this->height);
}

void Display::setTitle(const std::string & title)
{
	this->title = title;
	glfwSetWindowTitle(this->window, title.c_str());
}

void Display::setTitleSufix(const std::string & sufix)
{
	glfwSetWindowTitle(this->window,(this->title + sufix).c_str());
}

GLFWwindow* Display::getWindowPtr()
{
	return this->window;
}

float Display::getWidth() const
{
	return (float)this->width;
}

float Display::getHeight() const
{
	return (float)this->height;
}

float Display::getRatio() const
{
	return (float)this->width / (float)this->height;
}

float Display::getPixelXScale() const
{
	return 1.0f / getWidth();
}

float Display::getPixelYScale() const
{
	return 1.0f / getHeight();
}

FT_Library & Display::getFTLibrary()
{
	return this->ft;
}

bool Display::isOpen() const
{
	return glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(this->window);
}

void Display::endFrame()
{
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void Display::init()
{
	Display::displayPtr = this;

	// --------------- INIT GLFW ---------------
	if (!glfwInit())
	{
		Error::printError("DISPLAY::init()", "Failed to initialize GLFW!");
		exit(EXIT_FAILURE);
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Modern opengl
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Make MacOS happy
#endif

	this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
	if (this->window == NULL)
	{
		Error::printError("DISPLAY::init()", "Failed to open GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(this->window);

	// --------------- INIT GLEW ---------------

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		Error::printError("DISPLAY::init()", "Failed to initialize GLEW!");
		glfwDestroyWindow(this->window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, this->width, this->height);

	glfwSetWindowSizeCallback(this->window, resizeCallback);
	glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1); // V-Sync

	// --------------- INIT FREE TYPE --------------- 
	if (FT_Init_FreeType(&this->ft))
	{
		Error::printError("DISPLAY::init()", "Failed to initialize Free Type library");
		exit(EXIT_FAILURE);
	}
}

void Display::resizeCallback(GLFWwindow * window, int width, int height)
{
	// Only resize if width and height != 0
	if (width != 0 || height != 0)
	{
		Display::displayPtr->updateView(width, height);
		Display::displayPtr->sizeUpdated = true;
	}
}

