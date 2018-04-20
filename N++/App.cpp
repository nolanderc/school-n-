#include "App.h"
#include "Interpolate.h"


App::App(App* parent) :
	window(parent->window), parent(parent), running(true)
{
	this->preferredWindowSize = this->getWindowSize();
}

App::App(int width, int height, std::string title) :
	window(new Window(width, height, title)), preferredWindowSize({width, height}), running(true)
{
	this->window->show(SW_SHOW);
	this->window->setEventHandler(this);
}


void App::run()
{
	while (window->isOpen() && this->running) {
		window->pollMessages();

		static std::chrono::time_point<std::chrono::high_resolution_clock> previousInstant = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds dur =
			std::chrono::duration_cast<std::chrono::nanoseconds>(now - previousInstant);

		double seconds = double(dur.count()) / 1e9;


		this->update(seconds);
		this->renderApplication();

		previousInstant = now;

		// Kör den ägda appen ifall det är möjligt
		if (this->child) {
			this->window->setEventHandler(this->child);
			this->child->run();
			delete this->child;
			this->child = nullptr;

			this->setWindowSize(preferredWindowSize.cx, preferredWindowSize.cy);
		}
	}

	this->closed();
}

void App::close()
{
	this->running = false;

	if(this->parent)
	{
		this->window->setEventHandler(parent);
	} else
	{
		this->window->close();
	}
}

void App::repaint()
{
	this->redraw();
	this->renderApplication();
}

SIZE App::getWindowSize()
{
	return this->window->getSize();
}

void App::setWindowSize(int width, int height)
{
	this->window->setSize(width, height);
	this->preferredWindowSize = { width, height };
}

void App::setWindowTitle(std::string title)
{
	this->window->setTitle(title);
}


bool App::isKeyDown(int key)
{
	return this->window->isKeyDown(key);
}


Bitmap App::createCompatibleBitmap(SIZE size)
{
	return this->window->createCompatibleBitmap(size);
}

void App::addChild(App* child)
{
	if(this->child) {
		delete this->child;
	}

	this->child = child;
}


void App::renderApplication()
{
	Renderer renderer = window->getNewRenderer();
	this->draw(renderer);
	this->window->submitRenderer(renderer);
}

