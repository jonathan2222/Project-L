#include "Display.h"

#include "DisplayConfig.h"
#include "../Utils/Error.h"

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
	delete this->window;
}

void Display::processEvents()
{
	sf::Event evnt;
	while (this->window->pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			{
			if (evnt.key.code == sf::Keyboard::Escape)
				this->window->close();
			}
			break;
		default:
			break;
		}
	}
}

void Display::setTitle(const std::string & title)
{
	this->window->setTitle(title);
}

void Display::setTitleSufix(const std::string & sufix)
{
	this->window->setTitle(this->title + sufix);
}

sf::Window * Display::getWindowPtr()
{
	return this->window;
}

bool Display::isOpen()
{
	return this->window->isOpen();
}

void Display::init()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;
	this->window = new sf::Window(sf::VideoMode(this->width, this->height), this->title, sf::Style::Titlebar | sf::Style::Close, settings);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		Error::printError("DISPLAY::init()", "Failed to initialize GLEW!");
		exit(EXIT_FAILURE);
	}
}

