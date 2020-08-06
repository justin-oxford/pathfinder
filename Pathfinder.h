#pragma once
#include "State.h" 
#include "PauseMenu.h"
#include "DebugWindow.h"
#include "Grid.h"

class Pathfinder :
	public State
{
private:
	// State Variables 
	std::future<bool> fut_map;

	// State Items
	Grid* map;
	
	// the views in the state
	sf::View defaultView;
	sf::View gameView;
	sf::View UIView;

	// the fonts used
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;
	sf::Font Montserrat;
	PauseMenu* pmenu;

	// The State's Debug Window -- should be moved into State.h as a general item
	DebugWindow* debugWindow;
	bool debugWindowOpen;

	// FPS variables
	unsigned frameCount;
	unsigned fps;

	// Game Variables
	float gameSpeed;

	// GUI (buttons, list, pretty bits, etc)
	gui::Button modeChangeButton;

	//Functions 
	void initVariables();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initGUI();
	void initDebugWindow();
	void initViews();
	void initMap();

public:
	Pathfinder(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~Pathfinder();

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

