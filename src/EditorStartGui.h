#pragma once
#include "Gui.h"
#include "Button.h"
#include "TextBox.h"

class EditorStartGui : public Gui
{
private:
	std::shared_ptr<Button> m_backButton;
	std::shared_ptr<TextBox> m_textBox;
public:
	EditorStartGui(sf::RenderWindow& window, sf::Font& font);
};