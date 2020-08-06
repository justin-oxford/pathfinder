#pragma once

enum button_states { BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE };

namespace gui
{
	class Text
	{
	private:
		sf::Text text;
	public:
		Text();
		~Text();
	};

	class TextBox
	{
	private:
		sf::Text textBoxText;
		sf::RectangleShape textBox;
	public:
		TextBox();
		~TextBox();
	};
	
	class Button
	{
	private:
		short unsigned buttonState;
		short unsigned id;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;
		float outlineThickness;

	public:
		Button();
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned char_size,
			sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
			sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
			sf::Color outline_idleColor, sf::Color outline_hoverColor, sf::Color outline_activeColor, float outline_thickness,
			short unsigned id = 0);
		~Button();

		//Accessors
		const bool isPressed() const;
		const std::string getText() const;
		const short unsigned& getID() const;

		// Modifiers
		void setText(const std::string text);
		void setID(const short unsigned id);

		//Functions
		void update(const sf::Vector2f& mousePos);
		void render(sf::RenderTarget& target);
	};

	class DropDownList
	{
	private:
		float keyTime;
		float keyTimeMax;
		sf::Font& font;
		gui::Button* activeElement;
		std::vector<gui::Button*> list;
		bool showList;

	public:
		DropDownList(float x, float y, float width, float height, sf::Font& font, std::string list[], unsigned nrOfElements, unsigned default_index = 0);
		~DropDownList();

		// accessors
		const unsigned short& getActiveElementID() const;

		//Functions
		const bool getKeyTime();
		void updateKeyTime(const float& dt);
		void update(const sf::Vector2f& mousePos, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class PopUpBox
	{
		//
	};
}
