#pragma once
#include "EditorState.h"
#include "MapEditor.h"
#include "ResizableComponent.h"

class ResAreaState : public EditorState
{
private:
	enum class SelectionState { Moving, Placing, Rotating, Resizing };
	SelectionState m_selectionState = SelectionState::Moving;
	MapEditor::Selection<ResizableComponent> m_selection;
public:
	ResAreaState(MapEditor& editor);
	void handleInput(sf::Event& event, float dt) override;
	void onMouseButtonPressed(sf::Event& event, float dt);
	void onMouseMoved(sf::Event& event, float dt);
	void onKeyPressed(sf::Event& event, float dt);

	std::shared_ptr<ResizableComponent> createResArea();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
