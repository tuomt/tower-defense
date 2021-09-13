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
	sf::Sprite m_background;
public:
	MainMenu(sf::RenderWindow& window);
	~MainMenu();

	// Inherited via Scene
	virtual void handleInput(sf::Event& event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void loadTextures() override;
	virtual void unloadTextures() override;
};