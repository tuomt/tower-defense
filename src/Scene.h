#pragma once
#include <SFML/Graphics.hpp>

class Scene : public sf::Drawable
{
protected:
	sf::RenderWindow& m_window;
public:
	Scene(sf::RenderWindow& window);
	virtual void handleInput(sf::Event& event, float dt) = 0;
	virtual void update(float dt) = 0;
	//virtual void load() = 0;
	//virtual void unload() = 0;
	const sf::Vector2f getMousePos();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};
