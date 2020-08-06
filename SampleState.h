#pragma once
#include "State.h" 
#include "PauseMenu.h"
#include "DebugWindow.h"

class SampleState :
	public State
{
private:
	// the views in the state
	sf::View defaultView;
	sf::View gameView;
	sf::View UIView;

	// the fonts used
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;
	PauseMenu* pmenu;

	// The State's Debug Window -- should be moved into State.h as a general item
	DebugWindow* debugWindow;
	bool debugWindowOpen;
	// FPS variables
	unsigned frameCount;
	unsigned fps;

	// Game Variables
	float gameSpeed;

	//Functions 
	void initVariables();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initGUI();
	void initDebugWindow();
	void initViews();

public:
	SampleState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~SampleState();

	// Functions
	void updateView(const float& dt);
	void updateFPS(const float& dt);
	void updateInput(const float& dt, sf::Event sfEvent);
	void updateUserInput(const float& dt);
	void updatePauseMenuButtons();
	void updateGUI(const float& dt);
	void updateDebugWindow();
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};

