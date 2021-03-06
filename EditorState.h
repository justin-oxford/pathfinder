#pragma once

#include "State.h"
#include "gui.h"

class EditorState :
	public State
{
private:
	//Variables
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;

	std::map<std::string, gui::Button*> buttons;

	//Functions 
	void initVariable();
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initButtons();

public:
	EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~EditorState();

	// Functions
	void updateInput(const float& dt);
	void updateButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

