#include "stdafx.h"
#include "SettingsState.h"

// Init Funcitons
void SettingsState::initVariable()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
{
	this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));

	if (!this->backgroundTexture.loadFromFile("Resources/Images/Background/menu_bg.jpg"))
	{
		throw "ERR: Failed to load menu background image texture";
	}
	if (!this->loadingSplash.loadFromFile("Resources/Images/Background/splash_1.jpg"))
	{
		throw "ERR: Failed to load menu splash image texture";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void SettingsState::initFonts()
{
	if (!this->Crimson_Reg.loadFromFile("Fonts/CrimsonText-Regular.ttf"))
	{
		throw("ERROR: SettingsState >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Semibold.loadFromFile("Fonts/CrimsonText-Semibold.ttf"))
	{
		throw("ERROR: SettingsState >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Bold.loadFromFile("Fonts/CrimsonText-Bold.ttf"))
	{
		throw("ERROR: SettingsState >> COULD NOT LOAD FONT");
	}
}

void SettingsState::initKeybinds()
{
	std::ifstream ifs("Config/SettingsState_keybinds.txt");

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

void SettingsState::initGUI()
{
	this->buttons["BACK"] = new gui::Button(
		150.f, (float)this->window->getSize().y - 250.f,
		250.f, 100.f,
		&this->Crimson_Reg, "Back", 32,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200),
		sf::Color(50, 50, 50, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255),
		sf::Color(50, 50, 50, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255), 0.f,
		0);

	this->buttons["APPLY"] = new gui::Button(
		450.f, (float)this->window->getSize().y - 250.f,
		250.f, 100.f,
		&this->Crimson_Reg, "Apply", 32,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200),
		sf::Color(50, 50, 50, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255),
		sf::Color(50, 50, 50, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255), 0.f,
		0);
	std::vector<std::string> modes_str;
	for (auto& i : this->modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(250.f, 250.f, 200.f, 48.f, Crimson_Reg, modes_str.data(), modes_str.size());
}

void SettingsState::initText()
{
	this->optionsText.setFont(Crimson_Reg);

	this->optionsText.setPosition(sf::Vector2f(100.f, 100.f));
	this->optionsText.setCharacterSize(30);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 255));
	this->optionsText.setString(
		"Resolution \n\nFullscreen \n\nVSync \n\nAntialiasing \n\n"
	);
}

SettingsState::SettingsState(sf::RenderWindow* window, GraphicsSettings& gfxSettings, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states), gfxSettings(gfxSettings)
{
	this->initVariable();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initGUI();
	this->initText();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

void SettingsState::updateInput(const float& dt)
{

}

void SettingsState::updateGUI(const float& dt)
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

	// Quit Game
	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}

	// Apply Settings
	if (this->buttons["APPLY"]->isPressed())
	{
		// test test test
		this->gfxSettings.resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementID()];
		this->window->create(this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Default);
		this->initGUI();
	}

	// Dropdown List
	for (auto& it : this->dropDownLists)
	{
		it.second->update(this->mousePosView, dt);
	}

	// Dropdown Funcitonalities
}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateGUI(dt);

}

void SettingsState::renderGUI(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

	for (auto& it : this->dropDownLists)
	{
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);

	this->renderGUI(*target);
	target->draw(this->optionsText);
}

