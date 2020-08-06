#include "stdafx.h"
#include "EditorState.h"

// Init Funcitons
void EditorState::initVariable()
{

}

void EditorState::initBackground()
{
}

void EditorState::initFonts()
{
	if (!this->Crimson_Reg.loadFromFile("Fonts/CrimsonText-Regular.ttf"))
	{
		throw("ERROR: EditorState >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Semibold.loadFromFile("Fonts/CrimsonText-Semibold.ttf"))
	{
		throw("ERROR: EditorState >> COULD NOT LOAD FONT");
	}
	if (!this->Crimson_Bold.loadFromFile("Fonts/CrimsonText-Bold.ttf"))
	{
		throw("ERROR: EditorState >> COULD NOT LOAD FONT");
	}
}

void EditorState::initKeybinds()
{
	std::ifstream ifs("Config/editorstate_keybinds.txt");

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

void EditorState::initButtons()
{
}

EditorState::EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	this->initVariable();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();

}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
		std::cout << "END EDITOR" << "\n";
	}
}

void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}
}

void EditorState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateButtons();
	std::cout << "RUN EDITOR" << "\n";
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	this->renderButtons(*target);
}