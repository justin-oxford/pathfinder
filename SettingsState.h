#pragma once
#include "State.h"
#include "gui.h"

class SettingsState :
	public State
{
private:
	//Variables
	GraphicsSettings& gfxSettings;
	sf::Texture backgroundTexture;
	sf::Texture loadingSplash;
	sf::RectangleShape background;
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, gui::DropDownList*> dropDownLists;

	sf::Text optionsText;

	std::vector<sf::VideoMode> modes;

	//Functions 
	void initVariable();
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initGUI();
	void initText();

public:
	SettingsState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~SettingsState();

	// Functions
	void updateInput(const float& dt);
	void updateGUI(const float& dt);
	void update(const float& dt);
	void renderGUI(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

