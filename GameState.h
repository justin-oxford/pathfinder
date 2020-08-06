#pragma once

#include "State.h" 
#include "PauseMenu.h"
#include "GameStateGUI.h"
#include "DebugWindow.h"
#include "Map.h"
#include "TileMap.h"

class GameState :
public State
{
private:
	sf::View defaultView;
	sf::View gameView;
	sf::View miniMap;
	sf::View UIView;
	
	sf::Font Crimson_Reg;
	sf::Font Crimson_Semibold;
	sf::Font Crimson_Bold;
	PauseMenu* pmenu;

	GameStateGUI* GUI; 

	DebugWindow* debugWindow;
	bool debugWindowOpen;



	TileMap map;

	// Game Variables
	float gameSpeed;

	unsigned frameCount;
	unsigned fps;

	//Functions 
	void initVariables();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initPlayers();
	void initGUI();
	void initDebugWindow();
	void initViews();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	// Functions
	void updateView(const float& dt);
	void updateFPS(const float& dt);
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updatePauseMenuButtons();
	void updateGUI(
		const float& dt,
		const unsigned& score
	 );
	void updateDebugWindow();
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};

