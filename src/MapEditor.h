#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Scene.h"
#include "ResizableComponent.h"
#include "Component.h"
#include "Waypoint.h"
#include "TextureManager.h"
#include "EditorState.h"

class EditorState;

class MapEditor : public Scene
{
private:
	std::unique_ptr<EditorState> m_state;
	sf::Sprite m_mapSprite; // A sprite for drawing the map (background image)
public:
	template <class T>
	struct Selection 
	{
		std::shared_ptr<T> object = nullptr;
		sf::Vector2f grabOffset;

		void activate(const std::shared_ptr<T> target)
		{
			object = target;
		}

		void deactivate() 
		{
			object = nullptr;
			grabOffset = sf::Vector2f(0.f, 0.f);
		}

		bool isActive() const
		{
			if (object) return true;
			else return false;
		}
	};

	std::vector<std::shared_ptr<Waypoint>> m_waypoints; // A vector of waypoints
	std::vector<std::shared_ptr<ResizableComponent>> m_restrictedAreas; // A vector of restricted areas where towers cannot be placed

	MapEditor(sf::RenderWindow& window);
	~MapEditor();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleInput(sf::Event& event, float dt) override;
	void update(float dt) override;
	void loadTextures() override;
	void unloadTextures() override;

	void changeState(std::unique_ptr<EditorState> state);
	void saveMap(std::string mapName);
};
