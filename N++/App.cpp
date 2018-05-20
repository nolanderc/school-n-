#include "App.h"
#include "Interpolate.h"


App::App(App* parent) :
	window(parent->window), parent(parent), running(true)
{
	this->preferredWindowSize = this->getWindowSize();
}

App::App(Window* window) :
	window(window), preferredWindowSize(window->getSize()), running(true)
{
	this->window->setEventHandler(this);
}

App::~App()
{
	delete this->child;
}


int App::run()
{
	while (window->isOpen() && this->running) {
		// Hantera alla meddelanden ifån fönstret
		window->pollMessages();


		// Beräkna tiden sedan den förra uppdateringen
		static std::chrono::time_point<std::chrono::high_resolution_clock> previousInstant = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds dur =
			std::chrono::duration_cast<std::chrono::nanoseconds>(now - previousInstant);
		previousInstant = now;

		double seconds = double(dur.count()) / 1e9;


		// Uppdatera appen
		this->update(seconds);

		// Rita appen
		this->renderApplication();


		// Om denna app har en underordnad app, kör den istället
		while (this->child) {
			this->window->setEventHandler(this->child);
			int exitCode = this->child->run();
			delete this->child;
			this->child = nullptr;

			this->setWindowSize(preferredWindowSize.x, preferredWindowSize.y);
			this->childClosed(exitCode);
		}
	}

	// Appen har stängts
	this->closed();
	return this->exitCode;
}

void App::close(int exitCode)
{
	this->running = false;
	this->exitCode = exitCode;

	// Om denna app är underordnad en annan, överlåt alla meddelanden till denna istället
	// Annars, stäng fönstret
	if(this->parent) {
		this->window->setEventHandler(parent);
	} else {
		this->window->close();
	}
}

void App::repaint()
{
	this->redraw();
	this->renderApplication();
}

Vector2i App::getWindowSize()
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


bool App::isKeyDown(KeyCode key)
{
	return this->window->isKeyDown(key);
}

Vector2i App::getMousePosition()
{
	return this->window->getMousePosition();
}


Bitmap* App::createCompatibleBitmap(Vector2i size)
{
	return (Bitmap*) this->window->createCompatibleBitmap(size);
}

void App::addChild(App* child)
{
	delete this->child;
	this->child = child;
}

void App::alert(std::string title, std::string message) {
	this->window->alert(title, message);
}


void App::renderApplication()
{
	Renderer* renderer = window->getNewRenderer();

	this->draw(*renderer);
	this->window->submitRenderer(*renderer);

	delete renderer;
}

