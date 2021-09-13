#include "ResAreaState.h"
#include "WaypointState.h"

ResAreaState::ResAreaState(MapEditor& editor)
	: EditorState(editor)
{
}

void ResAreaState::handleInput(sf::Event& event, float dt)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		onMouseButtonPressed(event, dt);
		break;

	case sf::Event::MouseButtonReleased:
		switch (m_selectionState) 
		{
		case SelectionState::Moving:
		case SelectionState::Rotating:
		case SelectionState::Resizing:
			m_selection.deactivate();
			break;
		}
		break;

	case sf::Event::MouseMoved:
		onMouseMoved(event, dt);
		break;

	case sf::Event::KeyPressed:
		onKeyPressed(event, dt);
		break;

	case sf::Event::KeyReleased:
		switch (event.key.code)
		{
			case sf::Keyboard::E:
			case sf::Keyboard::R:
				if (m_selectionState != SelectionState::Placing) {
					m_selectionState = SelectionState::Moving;
					m_selection.deactivate();
				}
				break;
		}
	}
}

void ResAreaState::onMouseButtonPressed(sf::Event& event, float dt)
{
	auto& resAreas = m_editor.m_restrictedAreas;

	switch (m_selectionState)
	{
		case SelectionState::Placing:
		{
			// Add currently selected restricted area into the res. area vector
			auto area = std::static_pointer_cast<ResizableComponent>(m_selection.object);
			resAreas.push_back(area);

			// Create a new res. area
			area = createResArea();
			area->setPosition(m_editor.getMousePos());
			m_selection.activate(area);
			break;
		}
		case SelectionState::Moving: 
		{
			for (auto it = resAreas.rbegin(); it != resAreas.rend(); ++it) {
				auto& area = *it;
				if (area->getGlobalBounds().contains(m_editor.getMousePos())) {
					m_selection.activate(area);
					m_selection.grabOffset = m_editor.getMousePos() - m_selection.object->getPosition();
					break;
				}
			}
			break;
		}

		case SelectionState::Rotating:
		case SelectionState::Resizing:
		{
			for (auto it = resAreas.rbegin(); it != resAreas.rend(); ++it) {
				auto& area = *it;
				if (area->getKnob().getGlobalBounds().contains(m_editor.getMousePos())) {
					m_selection.activate(area);
					break;
				}
			}
			break;
		}
	}
}

void ResAreaState::onMouseMoved(sf::Event& event, float dt)
{
	switch (m_selectionState)
	{
	case SelectionState::Placing:
	case SelectionState::Moving:
		if (m_selection.isActive()) {
			m_selection.object->setPosition(m_editor.getMousePos() - m_selection.grabOffset);
		}
		break;

	case SelectionState::Rotating:
		if (m_selection.isActive()) {
			m_selection.object->rotateToPoint(m_editor.getMousePos());
		}
		break;

	case SelectionState::Resizing:
		if (m_selection.isActive()) {
			m_selection.object->resize(m_editor.getMousePos());
		}
		break;
	}
}

void ResAreaState::onKeyPressed(sf::Event& event, float dt)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Num1:
	{
		m_editor.changeState(std::make_unique<WaypointState>(m_editor));
		if (m_selectionState == SelectionState::Placing) {
			//m_window.setMouseCursorVisible(true);
		}
		break;
	}
	case sf::Keyboard::Space:
	{
		if (m_selectionState == SelectionState::Moving) {
			m_selectionState = SelectionState::Placing;
			//m_window.setMouseCursorVisible(false);
			
			// Create a new restricted area and activate it
			auto area = createResArea();
			m_selection.activate(area);
		}
		else if (m_selectionState == SelectionState::Placing) {
			m_selectionState = SelectionState::Moving;
			//m_window.setMouseCursorVisible(true);
			m_selection.deactivate();
		}
		break;
	}
	case sf::Keyboard::E:
	{
		if (m_selectionState != SelectionState::Placing) {
			m_selectionState = SelectionState::Resizing;
		}
		break;
	}
	case sf::Keyboard::R:
	{
		if (m_selectionState != SelectionState::Placing) {
			m_selectionState = SelectionState::Rotating;
		}
		break;
	}
	}
}

std::shared_ptr<ResizableComponent> ResAreaState::createResArea()
{
	auto& tm = TextureManager::getInstance();
	auto& resAreaTexture = tm.getTexture("restricted_area");
	auto& knobTexture = tm.getTexture("resize_knob");

	auto area = std::make_shared<ResizableComponent>(resAreaTexture, knobTexture);
	area->setPosition(m_editor.getMousePos());
	return area;
}

void ResAreaState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_selection.isActive()) {
		target.draw(*m_selection.object, states);
	}
}
