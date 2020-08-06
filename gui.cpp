#include "stdafx.h"
#include "gui.h"

// #########################################################################################################################
// TEXT
// #########################################################################################################################

gui::Text::Text()
{
	
}

gui::Text::~Text()
{

}

// #########################################################################################################################
// TEXT BOX
// #########################################################################################################################

gui::TextBox::TextBox()
{

}

gui::TextBox::~TextBox()
{

}

// #########################################################################################################################
// BUTTON
// #########################################################################################################################

gui::Button::Button()
{

}

gui::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned char_size,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
	sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
	sf::Color outline_idleColor,  sf::Color outline_hoverColor, sf::Color outline_activeColor, float outline_thickness,
	short unsigned id)
	: font(font),
	textIdleColor(textIdleColor), textHoverColor(textHoverColor), textActiveColor(textActiveColor),
	idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor),
	outlineIdleColor(outline_idleColor), outlineHoverColor(outline_hoverColor), outlineActiveColor(outline_activeColor), outlineThickness(outline_thickness),
	id(id)
{
	this->buttonState = BTN_IDLE;
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setOutlineThickness(this->outlineThickness);
	this->shape.setOutlineColor(this->outlineIdleColor);

	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(this->textIdleColor);
	this->text.setCharacterSize(char_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - 25
	);
	this->shape.setFillColor(this->idleColor);

	/*
	BUTTON SAMPLE
	
	new Button(
	x, y, width, height,
	&this->font, list[default_index], 16.f,
	sf::Color(25, 25, 25, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255),      // text color, text hover, text active
	sf::Color(25, 25, 25, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255),      // button color, button hover, button active
	sf::Color(25, 25, 25, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255), 1.f, // outline color, outline hover, outline active
	0);
	*/
}

gui::Button::~Button()
{

}

// Acessors
const bool gui::Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
	{
		return true;
	}
	return false;
}

const std::string gui::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& gui::Button::getID() const
{
	return this->id;
}


// Modifiers 

void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setID(const short unsigned id)
{
	this->id = id;
}

// Functions

void gui::Button::update(const sf::Vector2f& mousePos)
{
	//idle
	this->buttonState = BTN_IDLE;
	//hover
	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER;
		// pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE;
		}
	}
	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->text.setFillColor(this->textIdleColor);
		this->shape.setFillColor(this->idleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		this->shape.setOutlineThickness(this->outlineThickness);
		break;

	case BTN_HOVER:
		this->text.setFillColor(this->textHoverColor);
		this->shape.setFillColor(this->hoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		this->shape.setOutlineThickness(this->outlineThickness);
		break;

	case BTN_ACTIVE:
		this->text.setFillColor(this->textActiveColor);
		this->shape.setFillColor(this->activeColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		this->shape.setOutlineThickness(this->outlineThickness);
		break;

	default:
		this->text.setFillColor(sf::Color::Black);
		this->shape.setFillColor(sf::Color::Red);
		this->shape.setOutlineColor(sf::Color::Blue);
		this->shape.setOutlineThickness(2.f);
		break;
	}
}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}

// #########################################################################################################################
// DROP DOWN LIST
// #########################################################################################################################

gui::DropDownList::DropDownList(float x, float y, float width, float height, sf::Font& font, std::string list[], unsigned nrOfElements, unsigned default_index)
	: font(font), showList(false), keyTimeMax(5.f), keyTime(keyTimeMax)
{
	this->activeElement = new Button(x, y, width, height,
		&this->font, list[default_index], 24,
		sf::Color(211, 211, 211, 211), sf::Color(233, 233, 233, 255), sf::Color(255, 255, 255, 255), // text colors
		sf::Color(25, 25, 25, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255), // button colors
		sf::Color(25, 25, 25, 0), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 155, 255), 1.f, // outline colors
		0);
	for (size_t i = 0; i < nrOfElements; i++)
	{
		this->list.push_back(
			new gui::Button(
				x, y + ((i+1) * height), width, height,
				&this->font, list[i], 24,
				sf::Color(211, 211, 211, 211), sf::Color(233, 233, 233, 255), sf::Color(255, 255, 255, 255), // text colors
				sf::Color(25, 25, 25, 200), sf::Color(200, 190, 50, 200), sf::Color(255, 255, 155, 255), // button colors
				sf::Color(25, 25, 25, 0), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 155, 255), 1.f, // outline colors
				i
			)
		);
	}
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (size_t i = 0; i < this->list.size(); i++)
	{
		delete this->list[i];
	}
}

const unsigned short& gui::DropDownList::getActiveElementID() const
{
	return this->activeElement->getID();
}

const bool gui::DropDownList::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	else
	{
		return false;
	}
}

void gui::DropDownList::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 10.f * dt;
}

void gui::DropDownList::update(const sf::Vector2f& mousePos, const float& dt)
{
	this->updateKeyTime(dt);
	this->activeElement->update(mousePos);

	if (this->activeElement->isPressed() && this->getKeyTime())
	{
		if (this->showList)
		{
			this->showList = false;
		}
		else
		{
			this->showList = true;
		}
	}
	
	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->update(mousePos);

			if (i->isPressed() && this->getKeyTime())
			{
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setID(i->getID());
			}
		}
	}
}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->render(target);
		}
	}
}
