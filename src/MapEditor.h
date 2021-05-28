#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "ResizableComponent.h"
#include "Component.h"
#include "Waypoint.h"

class MapEditor : public Scene
{
public:
	MapEditor(sf::RenderWindow& window);
	struct MapData {
		std::string name;
		std::vector<Waypoint>* waypoints;
	};
	void draw();
	void handleInput(sf::Event& event, float dt);
	void update(float dt);
	void saveMap(MapData mapData);
private:
	const float waypointScaleX = 0.5f;
	const float waypointScaleY = 0.5f;

	sf::Font font;
	sf::Text controlsText; // A text which shows editor controls
	sf::RectangleShape controlsBackground; // A background rectangle for the controls
	bool showControls = true; // A bool that determines whether the controls are visible
	enum class State { GENERAL, WAYPOINT, RES_AREA };
	State state = MapEditor::State::GENERAL;

	enum class SelectionState { MOVING, ROTATING, RESIZING };
	SelectionState selectionState = MapEditor::SelectionState::MOVING;

	sf::Texture mapTexture;
	sf::Texture waypointTexture;
	sf::Texture restrictedAreaTexture;
	sf::Texture knobTexture;
	std::vector<Waypoint> waypoints; // A vector of waypoints
	std::vector<ResizableComponent> restrictedAreas; // A vector of restricted areas where towers cannot be placed
	std::vector<sf::RectangleShape> pathLines; // A vector for lines that are drawn between waypoints
	sf::Sprite mapSprite; // A sprite for drawing the map (background image)
	// CHANGE TO SMART POINTER
	Component* selectedComponent = NULL; // A pointer to the currently selected component
	// CHANGE TO SMART POINTER
	ResizableComponent* selectedRestrictedArea = NULL;
	//sf::RectangleShape* selectedKnob = NULL; // A pointer to the currently selected resizing knob
	// null until at least two waypoints exist
	sf::RectangleShape* selectedLine = NULL; // A pointer to a line that is drawn from the last waypoint to the currently selected waypoint

	sf::Vector2f grabOffset;
};