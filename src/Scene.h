#pragma once
#include <SFML/Graphics.hpp>

class Scene
{
protected:
	sf::RenderWindow& m_window;
public:
	Scene(sf::RenderWindow& window);
	virtual void handleInput(sf::Event& event, float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	//virtual void load() = 0;
	//virtual void unload() = 0;
	const sf::Vector2f getMousePos();
};
