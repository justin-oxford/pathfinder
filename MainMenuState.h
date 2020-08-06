#pragma once
#include "Pathfinder.h"
#include "SampleState.h"
#include "EditorState.h"
#include "SettingsState.h"
#include "gui.h"

class MainMenuState :
	public State
{
private:
	//Variables
	GraphicsSettings& gfxSettings;
	sf::Texture backgroundTexture;
	sf::Texture loadingSplash;
	sf::RectangleShape background;
	sf::Font Crimson_Reg;
	sf::Font Montserrat;

	Pathfinder* pathFinderState;

	std::map<std::string, gui::Button*> buttons;

	//Functions 
	void initVariable();
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initButtons();

public:
	MainMenuState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~MainMenuState();

	// Functions
	void updateInput(const float& dt, sf::Event sfEvent);
	void updateButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

