#pragma once
#include <stack>
#include <memory>
#include "Button.h"
#include "Scene.h"
#include "Gui.h"

class MainMenu : public Scene
{
private:
	sf::Font m_font;
public:
	MainMenu(sf::RenderWindow& window);
	virtual void handleInput(sf::Event& event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};