#include "stdafx.h"
#include "DebugWindow.h"

DebugWindow::DebugWindow(sf::Font& font)
{
	this->debugWindow.setPrimitiveType(sf::Quads);
	this->debugWindow.resize(4);
	// define its 4 corners
	this->debugWindow[0].position = sf::Vector2f(5.f, 5.f);
	this->debugWindow[1].position = sf::Vector2f(155.f, 5.f);
	this->debugWindow[2].position = sf::Vector2f(155.f, 205.f);
	this->debugWindow[3].position = sf::Vector2f(5.f, 205.f);

	this->debugWindow[0].color = sf::Color(15, 15, 15, 150);
	this->debugWindow[1].color = sf::Color(15, 15, 15, 150);
	this->debugWindow[2].color = sf::Color(15, 15, 15, 150);
	this->debugWindow[3].color = sf::Color(15, 15, 15, 150);

	this->fpsTitle.setFont(font);
	this->fpsTitle.setCharacterSize(25);
	this->fpsTitle.setFillColor(sf::Color::White);
	this->fpsTitle.setString("FPS: ");
	this->fpsTitle.setPosition(sf::Vector2f(10.f, 10.f));
	this->fpsDisplay.setFont(font);
	this->fpsDisplay.setCharacterSize(25);
	this->fpsDisplay.setFillColor(sf::Color::White);
	this->fpsDisplay.setString("0");
	this->fpsDisplay.setPosition(sf::Vector2f(75.f, 10.f));
}

DebugWindow::~DebugWindow()
{

}

void DebugWindow::update(unsigned fps)
{
	this->fpsDisplay.setString(std::to_string(fps));
}

void DebugWindow::render(sf::RenderTarget& target)
{
	target.draw(this->debugWindow);
	target.draw(this->fpsTitle);
	target.draw(this->fpsDisplay);
}
