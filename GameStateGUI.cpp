#include "stdafx.h"
#include "GameStateGUI.h"

GameStateGUI::GameStateGUI(sf::Font& font)
{
	this->UITopBar.setPrimitiveType(sf::TrianglesStrip);
	this->UITopBar.resize(6);
	// define its 4 corners
	this->UITopBar[0].position = sf::Vector2f(0.f, 0.f);
	this->UITopBar[1].position = sf::Vector2f(0.f, 100.f);
	this->UITopBar[2].position = sf::Vector2f(300.f, 0.f);
	this->UITopBar[3].position = sf::Vector2f(300.f, 100.f);
	this->UITopBar[4].position = sf::Vector2f(320.f, 0.f);
	this->UITopBar[5].position = sf::Vector2f(320.f, 80.f);

	sf::Color topBarColor = sf::Color(50, 50, 50, 200);
	this->UITopBar[0].color = topBarColor;
	this->UITopBar[1].color = topBarColor;
	this->UITopBar[2].color = topBarColor;
	this->UITopBar[3].color = topBarColor;
	this->UITopBar[4].color = topBarColor;
	this->UITopBar[5].color = topBarColor;

	this->UITopBarStroke.setPrimitiveType(sf::TrianglesStrip);
	this->UITopBarStroke.resize(6);
	// define its 4 corners
	this->UITopBar[0].position = sf::Vector2f(0.f, 0.f);
	this->UITopBar[1].position = sf::Vector2f(0.f, 105.f);
	this->UITopBar[2].position = sf::Vector2f(305.f, 0.f);
	this->UITopBar[3].position = sf::Vector2f(305.f, 105.f);
	this->UITopBar[4].position = sf::Vector2f(325.f, 0.f);
	this->UITopBar[5].position = sf::Vector2f(325.f, 85.f);

	sf::Color topBarStrokeColor = sf::Color(200, 0, 0, 200);
	this->UITopBarStroke[0].color = topBarStrokeColor;
	this->UITopBarStroke[1].color = topBarStrokeColor;
	this->UITopBarStroke[2].color = topBarStrokeColor;
	this->UITopBarStroke[3].color = topBarStrokeColor;
	this->UITopBarStroke[4].color = topBarStrokeColor;
	this->UITopBarStroke[5].color = topBarStrokeColor;

	// Year Information
	this->pointsHeader.setFont(font);
	this->pointsHeader.setCharacterSize(25);
	this->pointsHeader.setFillColor(sf::Color(200,200,200,200));
	this->pointsHeader.setString("Score: ");
	this->pointsHeader.setPosition(sf::Vector2f(25.f,10.f));
	this->pointsDisplay.setFont(font);
	this->pointsDisplay.setCharacterSize(40);
	this->pointsDisplay.setFillColor(sf::Color(255, 255, 255, 200));
	this->pointsDisplay.setString("0");
	this->pointsHeader.setPosition(sf::Vector2f(75.f, 10.f));
}

GameStateGUI::~GameStateGUI()
{

}

void GameStateGUI::update(
	const float& dt, 
	const int& score
)
{
	// update population display
	this->pointsDisplay.setString(std::to_string(score));
}

void GameStateGUI::render(sf::RenderTarget& target)
{
	//target.draw(this->topBar);
	target.draw(this->UITopBarShadow);
	target.draw(this->UITopBar);
	target.draw(this->pointsHeader);
	target.draw(this->pointsDisplay);
}
