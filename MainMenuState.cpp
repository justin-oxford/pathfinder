#include "stdafx.h"
#include "MainMenuState.h"

// Init Funcitons
void MainMenuState::initVariable()
{

}

void MainMenuState::initBackground()
{
	this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
	//this->background.setFillColor(sf::Color(25, 25, 25, 255));

	if (!this->backgroundTexture.loadFromFile("Resources/Images/Background/menu_bg.jpg"))
	{
		throw "ERR: Failed to load menu background image texture";
	}
	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initFonts()
{
	if (!this->Crimson_Reg.loadFromFile("Fonts/CrimsonText-Regular.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
	if (!this->Montserrat.loadFromFile("Fonts/Montserrat-Medium.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
	/*
	if (!this->Crimson_Semibold.loadFromFile("Fonts/CrimsonText-Semibold.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Bold.loadFromFile("Fonts/CrimsonText-Bold.ttf"))
	{
		throw("ERROR: MAINMENUSTATE >> COULD NOT LOAD FONT");
	}*/
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.txt");

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

void MainMenuState::initButtons()
{
	// SEE GUI.CPP for button contructor values

	this->buttons["SAMPLE_STATE"] = new gui::Button(
		((float)this->window->getSize().x / 2) - 125,
		(float)this->window->getSize().y / 2 - 200,
		250.f, 100.f,
		&this->Montserrat, "START", 40,
		sf::Color(36, 36, 36, 200), sf::Color(36, 36, 36, 255), sf::Color(36, 36, 36, 200),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 225), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255), 0.f,
		0
	);
	this->buttons["EXIT_STATE"] = new gui::Button(
		((float)this->window->getSize().x / 2) - 125.f,
		(float)this->window->getSize().y - 175.f,
		250.f, 100.f,
		&this->Montserrat, "EXIT", 40,
		sf::Color(36, 36, 36, 200), sf::Color(36, 36, 36, 255), sf::Color(36, 36, 36, 200),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 225), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255), 0.f,
		0
	);
}

MainMenuState::MainMenuState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states), gfxSettings(gfxSettings)
{
	this->initVariable();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();

}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void MainMenuState::updateInput(const float& dt, sf::Event sfEvent)
{

}

void MainMenuState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}
	// Play Game
	if (this->buttons["SAMPLE_STATE"]->isPressed())
	{
		this->states->push(new Pathfinder(this->window, this->supportedKeys, this->states));
	}

	// Quit Game
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	//this->updateInput(dt);

	this->updateButtons();
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);

	this->renderButtons(*target);
}
