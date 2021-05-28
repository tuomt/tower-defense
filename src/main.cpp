#include <SFML/Graphics.hpp>
#include <time.h>
#include "Helper.h"
#include "SceneManager.h"
#include "MapEditor.h"
#include "Game.h"

using namespace Helper;

int main()
{
	srand(time(nullptr));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1600, 900), "Tower Defense", sf::Style::Default, settings);
	window.setFramerateLimit(120);

	SceneManager& sceneManager = SceneManager::getInstance();
	sceneManager.currentScene = std::make_unique<Game>(window);

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;

		// Get delta time
		float dt = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else sceneManager.currentScene->handleInput(event, dt);
		}
		sceneManager.currentScene->update(dt);

		window.clear();
		sceneManager.currentScene->draw();
		window.display();
	}

	return 0;
}
