#include "stdafx.h"
#include "SampleState.h"

void SampleState::initVariables()
{
	this->gameSpeed = 1.0;
	this->frameCount = 0;
	this->debugWindowOpen = true;
	srand(std::time(0));
}

void SampleState::initKeybinds()
{
	std::ifstream ifs("Config/SampleState_keybinds.txt");

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

void SampleState::initFonts()
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

void SampleState::initTextures()
{
	/*if (!this->textures["TEXTURE_NAME"].loadFromFile("address"))
	{
		throw "ERR: Could not load ??";
	}*/
}

void SampleState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->Crimson_Semibold);
	this->pmenu->addButton("RESUME_STATE", 400.f, "Resume");
	this->pmenu->addButton("EXIT_STATE", 550.f, "Quit");
}

void SampleState::initGUI()
{
	
}

void SampleState::initDebugWindow()
{
	this->debugWindow = new DebugWindow(this->Crimson_Reg);
}

void SampleState::initViews()
{
	// Default View
	this->defaultView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	this->defaultView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->defaultView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);

	// Game View
	this->defaultView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	this->defaultView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->defaultView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);

	// UI View
	this->UIView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	this->UIView.setCenter((float)this->window->getSize().x / 2.f, (float)this->window->getSize().y / 2.f);
	this->UIView.setSize((float)this->window->getSize().x, (float)this->window->getSize().y);
}

SampleState::SampleState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	std::cout << "Sample State Title" << std::endl;
	std::cout << ">this is a description<" << std::endl;
	std::cout << "//" << std::endl;
	this->initVariables();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initGUI();
	this->initDebugWindow();
	this->initViews();
}

SampleState::~SampleState()
{
	delete this->pmenu;
}

void SampleState::updateView(const float& dt)
{
	
}

void SampleState::updateFPS(const float& dt)
{
	this->fps = (unsigned)1.f / (dt);
	this->frameCount = 0;
}

void SampleState::updateInput(const float& dt, sf::Event sfEvent)
{
	if (sfEvent.type == sf::Event::MouseButtonPressed)
	{
		if (sfEvent.mouseButton.button == sf::Mouse::Left)
		{
			//
		}
	}
	if (sfEvent.type == sf::Event::MouseButtonReleased)
	{
		if (sfEvent.mouseButton.button == sf::Mouse::Left)
		{
			//
		}
	}
}

void SampleState::updateUserInput(const float& dt)
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("SPACE"))))
	{
		std::cout << "SPACE PRESSED" << "\n";
	}
}

void SampleState::updatePauseMenuButtons()
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

void SampleState::updateGUI(const float& dt)
{
	
}

void SampleState::updateDebugWindow()
{
	this->debugWindow->update(this->fps);
}

void SampleState::update(const float& dt)
{
	this->frameCount += 1;
	if (this->frameCount > 59)
	{
		this->updateFPS(dt);
	}
	this->updateMousePositions();
	this->updateKeyTime(dt);
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

void SampleState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	if (this->paused) // paused menu
	{
		target->setView(this->defaultView);
		this->pmenu->render(*target);
	}

	// UI View
	target->setView(this->UIView);
	if (debugWindowOpen)
		//this->debugWindow->render(*target);
	
	target->setView(this->gameView);
}