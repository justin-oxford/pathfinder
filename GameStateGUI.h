#pragma once

#include "Player.h"

class GameStateGUI
{
private:
	sf::VertexArray UITopBar;
	sf::VertexArray UITopBarStroke;
	sf::VertexArray UITopBarShadow;
	sf::Text pointsHeader;
	sf::Text pointsDisplay;

	
public:
	GameStateGUI(sf::Font& font);
	virtual ~GameStateGUI();

	virtual void update(
		const float& dt,
		const int& score
		);
	virtual void render(sf::RenderTarget& target);
};

