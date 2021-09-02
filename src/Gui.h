#pragma once
#include <list>
#include <memory>
#include "Widget.h"
#include "Scene.h"

class Gui : public Scene
{
protected:
	sf::Font& m_font;
public:
	Gui(sf::RenderWindow& window, sf::Font& font)
		: Scene(window), m_font(font) {};

	std::list<std::shared_ptr<Widget>> m_widgets;

	void addWidget(const std::shared_ptr<Widget>& widget);
	void removeWidget(const std::shared_ptr<Widget>& widget);

	// Inherited via Scene
	virtual void handleInput(sf::Event& event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
