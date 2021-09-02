#pragma once
#include "EditorState.h"
#include "MapEditor.h"
#include "Waypoint.h"

class WaypointState : public EditorState
{
private:
	enum class SelectionState { Moving, Placing };
	SelectionState m_selectionState = SelectionState::Moving;
	MapEditor::Selection<Waypoint> m_selection;
public:
	WaypointState(MapEditor& editor);
	void handleInput(sf::Event& event, float dt) override;
	void onMouseButtonPressed(sf::Event& event, float dt);
	void onMouseMoved(sf::Event& event, float dt);
	void onKeyPressed(sf::Event& event, float dt);

	std::shared_ptr<Waypoint> createWaypoint();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
