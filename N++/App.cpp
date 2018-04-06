#include "App.h"

App::App(Window* window) :
	window(window)
{
}

App::App(int width, int height, std::string title) :
	window(new Window(width, height, title))
{
	this->window->show(SW_SHOW);
	this->window->setEventHandler(this);
}


void App::run()
{
	while (window->isOpen()) {
		window->pollMessages();

		static std::chrono::time_point<std::chrono::high_resolution_clock> previousInstant = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds dur =
			std::chrono::duration_cast<std::chrono::nanoseconds>(now - previousInstant);

		double seconds = double(dur.count()) / 1e9;

		//if (seconds > 1.0 / framerate) {
			this->update(seconds);

			this->renderApplication();

			previousInstant = now;
		//}
	}
}

void App::close()
{
	this->window->close();
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

void App::renderApplication()
{
	Renderer renderer = window->getNewRenderer();
	this->draw(renderer);
	this->window->submitRenderer(renderer);
}

