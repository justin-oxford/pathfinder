#pragma once

#include "gui.h"

class PauseMenu
{
private:
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;
	sf::Text menuText;

	sf::RectangleShape background;
	sf::RectangleShape container;
	std::map<std::string, gui::Button*> buttons;

	// Functions


public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	// Accessor
	std::map<std::string, gui::Button*>& getButtons();

	// Funcitons
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key, float y, const std::string text);
	void update(const sf::Vector2f& mousePos);
	void render(sf::RenderTarget& target);
};
