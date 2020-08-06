#pragma once

class GraphicsSettings
{
public:
    GraphicsSettings();

    // variables
    std::string title;
    sf::VideoMode resolution;
    bool fullscreen;
    bool verticalSync;
    unsigned framerateLimit;
    sf::ContextSettings contextSettings;
    std::vector< sf::VideoMode > videoModes;

    // fncs
    void saveToFile(const std::string path);
    void loadFromFile(const std::string path);
};