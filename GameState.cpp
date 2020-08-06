#include "stdafx.h"
#include "GameState.h"

void GameState::initVariables()
{
	this->gameSpeed = 1.0;
	this->frameCount = 0;
	this->debugWindowOpen = true;
	srand(std::time(0));
}

void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.txt");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";
		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();
}

void GameState::initFonts()
{
	if (!this->Crimson_Reg.loadFromFile("Fonts/CrimsonText-Regular.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Semibold.loadFromFile("Fonts/CrimsonText-Semibold.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Bold.loadFromFile("Fonts/CrimsonText-Bold.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
}

void GameState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Resources/Images/Sprites/sneek64_tst.png"))
	{
		throw "ERR: Could not load Gamestate Sprite Texture [PLAYER_SHEET]";
	}
}

void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->Crimson_Semibold);
	this->pmenu->addButton("GAME_STATE", 400.f, "Resume");
	this->pmenu->addButton("EXIT_STATE", 550.f, "Quit");
}

void GameState::initPlayers()
{
	this->player = new Player(0, 0, this->textures["PLAYER_SHEET"]);
}

void GameState::initGUI()
{
	this->GUI = new GameStateGUI(this->Crimson_Semibold);
}

void GameState::initDebugWindow()
{
	this->debugWindow = new DebugWindow(this->Crimson_Reg);
}

void GameState::initViews()
{
	// Default View
	this->defaultView.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	this->defaultView.setCenter((float) this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->defaultView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);

	// Game View
	this->gameView.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	this->gameView.setCenter(this->player->getPosition());
	this->gameView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);
	this->gameView.zoom(0.5f);

	// Minimap View
	this->miniMap.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	this->miniMap.setSize((float)this->window->getSize().x / 2, (float)this->window->getSize().y / 2);

	// UI View
	this->UIView.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	this->UIView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->UIView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);
}

GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	std::cout << "Welcome to SNEEK\nPre-Alpha 0.0.1" << std::endl;
	std::cout << "WASD to move" << std::endl;
	std::cout << "//" << std::endl;
	this->initVariables();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initPlayers();
	this->initGUI();
	this->initDebugWindow();
	this->initViews();
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->player;
}

void GameState::updateView(const float& dt)
{
	 //this->gameView.setCenter(this->player->getPosition());
	float sX = this->player->getPosition().x;
	float sY = this->player->getPosition().y;
	float mX = mousePosView.x;
	float mY = mousePosView.y;
	this->gameView.setCenter(
		sf::Vector2f(
			sX + ((mX - sX) / 2.f), 
			sY + ((mY - sY) / 2.f)
		)
	); 
}

void GameState::updateFPS(const float& dt)
{
	this->fps = (unsigned) 1.f / (dt);
	this->frameCount = 0;
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
		{
			this->pauseState();
		}
		else
		{
			this->unpauseState();
		}
	}
}

void GameState::updatePlayerInput(const float& dt)
{
	// Update Player Input
	float strafe = 0.f;
	float move = 0.f;
	float typeSpeed = 1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
	{
		strafe += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
	{
		strafe += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_FORWARD"))))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("SPRINT"))))
		{
			typeSpeed += 2.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("SNEAK"))))
		{
			typeSpeed -= 0.6f;
		}
		
		move += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_BACKWARD"))))
	{
		move += 0.5f;
	}
	this->player->move(dt, move, typeSpeed, strafe);
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("EXIT_STATE") && this->getKeyTime())
	{
		this->window->setView(this->defaultView);
		this->endState();
	}
	if (this->pmenu->isButtonPressed("GAME_STATE"))
	{
		this->unpauseState();
	}
}

void GameState::updateGUI(
	const float& dt,
	const unsigned& score
)
{
	this->GUI->update(dt, score);
}

void GameState::updateDebugWindow()
{
	this->debugWindow->update(this->fps);
}

void GameState::update(const float& dt)
{
	this->frameCount += 1;
	if (this->frameCount > 59)
	{
		this->updateFPS(dt);
	}
	this->updateMousePositions();
	this->updateKeyTime(dt);
	this->updateInput(dt);
	if (!this->paused) // uppaused
	{
		this->updateView(dt);
		this->updatePlayerInput(dt);
		this->player->update(dt, this->mousePosView);
		this->game_map.update(this->mousePosView);
		this->score = this->player->getScore();
		this->updateGUI(dt, score);
	}
	else // paused
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	if (debugWindowOpen)
		this->updateDebugWindow();
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	//this->game_map.render(*target);
	this->map.render(*target);
	this->player->render(*target);
	if (this->paused) // paused menu
	{
		target->setView(this->defaultView);
		this->pmenu->render(*target);
	}

	// UI View
	target->setView(this->UIView);
	this->GUI->render(*target);
	if (debugWindowOpen)
		//this->debugWindow->render(*target);

	target->setView(this->gameView);
}
