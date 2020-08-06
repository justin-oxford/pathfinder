#include "stdafx.h"
#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
{
    this->title = "UN INIT";
    this->resolution = sf::VideoMode::getDesktopMode();
    this->fullscreen = false;
    this->verticalSync = false;
    this->framerateLimit = 120;
    this->contextSettings.antialiasingLevel = 0;
    this->videoModes = sf::VideoMode::getFullscreenModes();
}

// fncs
void GraphicsSettings::saveToFile(const std::string path)
{
    std::ofstream ofs(path);

    if (ofs.is_open())
    {
        ofs << this->title;
        ofs << resolution.width << " " << resolution.height;
        ofs << fullscreen;
        ofs << framerateLimit;
        ofs << verticalSync;
        ofs << contextSettings.antialiasingLevel;
    }
    ofs.close();
};
void GraphicsSettings::loadFromFile(const std::string path)
{
    std::ifstream ifs(path);

    if (ifs.is_open())
    {
        std::getline(ifs, this->title);
        ifs >> resolution.width >> resolution.height;
        ifs >> fullscreen;
        ifs >> framerateLimit;
        ifs >> verticalSync;
        ifs >> contextSettings.antialiasingLevel;
    }
    ifs.close();
};