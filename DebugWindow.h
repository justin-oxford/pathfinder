#pragma once

class DebugWindow
{
private:
	sf::VertexArray debugWindow;
	sf::Text fpsTitle;
	sf::Text fpsDisplay;

public:
	DebugWindow(sf::Font& font);
	virtual ~DebugWindow();

	virtual void update(unsigned fps);
	virtual void render(sf::RenderTarget& target);
};

