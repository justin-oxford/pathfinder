#ifndef ENGINE_H
#define ENGINE_H

#include "MainMenuState.h"

class Engine
{
private:
    
    // Variables
    GraphicsSettings gfxSettings;
    sf::RenderWindow *window;
    sf::Event sfEvent;

    // delta time
    sf::Clock dtClock;
    float dt;

    std::stack<State*> states;

    std::map<std::string, int> supportedKeys;

    // Initialization
    void initVariables();
    void initGraphicsSettings();
    void initWindow();
    void initKeys();
    void initStates();

public:
    // Constructor/Destructor

    // The Engine constructor
    Engine();
    virtual ~Engine();

    // Regular
    void endApplication();

    // Update
    void updateDt();
    void updateSFMLEvents();
    void update();
    
    // Render
    void render();

    // Core
    void run();
};

#endif