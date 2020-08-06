#include "stdafx.h"
#include "Engine.h"

// New FCNS

// Static Functions ##########################################

// Initializer Functions #####################################

void Engine::initVariables()
{
    this->window = NULL;
    this->dt = 0.f;
}

void Engine::initGraphicsSettings()
{
    this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void Engine::initWindow()
{
    if (this->gfxSettings.fullscreen)
    {
        this->window = new sf::RenderWindow(
            this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Fullscreen, 
            this->gfxSettings.contextSettings);
    }
    else
    {
        this->window = new sf::RenderWindow(
            this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Titlebar | sf::Style::Close, 
            this->gfxSettings.contextSettings);
    }
    this->window->setFramerateLimit(this->gfxSettings.framerateLimit);
    this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
}

void Engine::initKeys()
{
    std::ifstream ifs("Config/supported_keys.txt");

    if (ifs.is_open())
    {
        std::string key = "";
        int key_value = 0;
        while (ifs >> key >> key_value)
        {
            this->supportedKeys[key] = key_value;
        }
    }
    ifs.close();
}

void Engine::initStates()
{
    this->states.push(new MainMenuState(this->window, this->gfxSettings, &this->supportedKeys, &this-> states));
}

// Constructor/Destructor Functions ##########################

Engine::Engine()
{
    this->initVariables();;
    this->initGraphicsSettings();
    this->initVariables();
    this->initWindow();
    this->initKeys();
    this->initStates();
}

Engine::~Engine()
{
	delete this->window;

    while (!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
}

// Functions #################################################

void Engine::endApplication()
{
    std::cout << "Ending Application" << "\n";
}

void Engine::updateDt()
{
    // updates dt variable with time it takes to render one frame
    this->dt = this->dtClock.restart().asSeconds();
}

void Engine::updateSFMLEvents()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
        else
        {
            this->states.top()->updateInput(this->dt, this->sfEvent);
        }
    }
}

// update / inputs / events
void Engine::update()
{
    if (!this->states.empty())
    {
        this->updateSFMLEvents();
        this->states.top()->update(this->dt);
        if (this->states.top()->getQuit())
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    // Application End
    else
    {
        this->endApplication();
        this->window->close();
    }
}

// render it all out
void Engine::render()
{
    this->window->clear(sf::Color(25,25,25,255));

    if (!this->states.empty())
    {
        this->states.top()->render(this->window);
    }
    this->window->display();
}

// run the game
void Engine::run()
{
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}