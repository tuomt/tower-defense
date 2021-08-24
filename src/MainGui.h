#pragma once
#include <memory>
#include <list>
#include "Button.h"
#include "Gui.h"

class MainGui : public Gui
{
private:
	std::shared_ptr<Button> m_playButton;
	std::shared_ptr<Button> m_editorButton;
	std::shared_ptr<Button> m_quitButton;
public:
	MainGui(sf::RenderWindow& window, sf::Font& font);
};
