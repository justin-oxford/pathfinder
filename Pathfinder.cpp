#include "stdafx.h"
#include "Pathfinder.h"

void Pathfinder::initVariables()
{
	this->gameSpeed = 1.0;
	this->frameCount = 0;
	this->debugWindowOpen = true;
	srand((unsigned int)std::time(0));
}

void Pathfinder::initKeybinds()
{
	std::ifstream ifs("Config/AStar_keybinds.txt");

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

void Pathfinder::initFonts()
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
	if (!this->Montserrat.loadFromFile("Fonts/Montserrat-Medium.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
}

void Pathfinder::initTextures()
{
	
}

void Pathfinder::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->Montserrat);
	this->pmenu->addButton("RESUME_STATE", 400.f, "Resume");
	this->pmenu->addButton("EXIT_STATE", 550.f, "Quit");
}

void Pathfinder::initGUI()
{
	
}

void Pathfinder::initDebugWindow()
{
	this->debugWindow = new DebugWindow(this->Montserrat);
}

void Pathfinder::initViews()
{
	// Default View
	this->defaultView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	this->defaultView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->defaultView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);

	// Game View
	this->gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	this->gameView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->gameView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);
}

void Pathfinder::initMap()
{
	this->map = new Grid(*this->window, this->Montserrat);
}

Pathfinder::Pathfinder(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	std::cout << "Algorithm Show-off-er" << std::endl;
	std::cout << ">Currently has Dijkstra's & A*<" << std::endl;
	std::cout << "//" << std::endl;
	this->initVariables();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initGUI();
	this->initDebugWindow();
	this->initViews();
	this->initMap();
}

Pathfinder::~Pathfinder()
{
	delete this->pmenu;
	delete this->map;
}

void Pathfinder::updateView(const float& dt)
{

}

void Pathfinder::updateFPS(const float& dt)
{
	this->fps = (unsigned int) (1.f / (dt));
	this->frameCount = 0;
}

void Pathfinder::updateInput(const float& dt, sf::Event sfEvent)
{
	if (sfEvent.type == sf::Event::MouseButtonPressed)
	{
		// place the start, only if you're not drawing
		if (sfEvent.mouseButton.button == sf::Mouse::Left && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("LCTRL"))))
		{
			this->map->placeStartPoint(this->mousePosView);
		}

		// place the end, only if you're not drawing
		if (sfEvent.mouseButton.button == sf::Mouse::Right && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("LCTRL"))))
		{
			this->map->placeEndPoint(this->mousePosView);
		}
	}
	else if (sfEvent.type == sf::Event::MouseButtonReleased)
	{

	}
}

void Pathfinder::updateUserInput(const float& dt)
{
	// this will pause the state if the player hits ESC
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
	// A Runs A*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("A"))) && this->getKeyTime())
	{
		std::cout << "Using A-Star" << "\n";
		this->fut_map = std::async(std::launch::async, &Grid::astarAlg, this->map);
	}
	// D runs Dijkstra
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("D"))) && this->getKeyTime())
	{
		std::cout << "Using Dijkstra" << "\n";
		this->fut_map = std::async(std::launch::async, &Grid::dijkstrasAlg, this->map);
	}
	// draw if left click + Ctrl
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("LCTRL"))))
	{
		this->map->drawObstacles(this->mousePosView);
	}
	// erase if right click + Ctrl
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("LCTRL"))))
	{
		this->map->eraseObastcles(this->mousePosView);
	}
	// C clears everything 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("C"))))
	{
		this->map->clearMap();
	}
}

void Pathfinder::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("EXIT_STATE") && this->getKeyTime())
	{
		this->window->setView(this->defaultView);
		this->endState();
	}
	if (this->pmenu->isButtonPressed("RESUME_STATE"))
	{
		this->unpauseState();
	}
}

void Pathfinder::updateGUI(const float& dt)
{

}

void Pathfinder::updateDebugWindow()
{
	this->debugWindow->update(this->fps);
}

void Pathfinder::update(const float& dt)
{
	// Check if threads are rejoinable, if so join them
	if (this->fut_map.valid())
	{
		if (this->fut_map.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			this->fut_map.get();
		}
	}
	// Update FPS data -- other debug data will go here too
	this->frameCount += 1;
	if (this->frameCount > 59)
	{
		this->updateFPS(dt);
		this->window->setTitle("Pathfinder --- FPS: " + std::to_string(fps));
	}
	this->updateMousePositions();
	this->updateKeyTime(dt);
	this->map->update(dt, this->mousePosView);
	if (!this->paused) // unpaused
	{
		this->updateView(dt);
		this->updateUserInput(dt);
		this->updateGUI(dt);
	}
	else // paused
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	if (debugWindowOpen)
		this->updateDebugWindow();
}

void Pathfinder::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	// State Objects
	this->map->render(*target);

	// Pause Menu
	if (this->paused) // paused menu
	{
		target->setView(this->defaultView);
		this->pmenu->render(*target);
	}

	// UI View
	if (debugWindowOpen)
		//this->debugWindow->render(*target);

	target->setView(this->gameView);
}
