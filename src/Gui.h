#pragma once
#include <list>
#include <memory>
#include "Widget.h"

class Gui : public sf::Drawable
{
protected:
	sf::RenderWindow& m_window;
	sf::Font& m_font;
public:
	Gui(sf::RenderWindow& window, sf::Font& font)
		: m_window(window), m_font(font) {};

	std::list<std::shared_ptr<Widget>> m_widgets;

	void addWidget(const std::shared_ptr<Widget>& widget);
	void removeWidget(const std::shared_ptr<Widget>& widget);

	virtual void handleInput(sf::Event& event, float dt);
	virtual void update(float dt);
	
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
