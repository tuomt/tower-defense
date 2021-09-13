#include <assert.h>
#include <memory>
#include "WaypointState.h"
#include "ResAreaState.h"
#include "PathLine.h"
#include "CollisionHandler.h"
#include "Helper.h"
#include "TextureManager.h"

using namespace Helper;

WaypointState::WaypointState(MapEditor& editor)
	: EditorState(editor)
{}

void WaypointState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_selection.isActive()) {
		target.draw(*m_selection.object, states);
	}
}

void WaypointState::handleInput(sf::Event& event, float dt)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
			onMouseButtonPressed(event, dt);
			break;
		case sf::Event::MouseButtonReleased:
			if (m_selectionState == SelectionState::Moving) {
				m_selection.deactivate();
			}
			break;
		case sf::Event::MouseMoved:
			onMouseMoved(event, dt);
			break;
		case sf::Event::KeyPressed:
			onKeyPressed(event, dt);
			break;
	}
}

void WaypointState::onMouseButtonPressed(sf::Event& event, float dt)
{
	auto& waypoints = m_editor.m_waypoints;

	if (event.mouseButton.button == sf::Mouse::Left) {
		switch (m_selectionState)
		{
			case SelectionState::Placing: 
			{
				// Add currently selected waypoint into the waypoint vector
				auto waypoint = std::static_pointer_cast<Waypoint>(m_selection.object);
				waypoints.push_back(waypoint);

				// Create a new waypoint
				waypoint = createWaypoint();
				m_selection.activate(waypoint);

				// Create a path line
				assert(!waypoints.empty());
				waypoints.back()->m_tailLine = std::make_shared<PathLine>(waypoints.back(), waypoint);
				waypoint->m_headLine = waypoints.back()->m_tailLine;

				break;
			}

			case SelectionState::Moving: 
			{
				// Iterate waypoints from end to beginning
				// so that the last placed waypoint is selected if there is overlap
				for (auto it = waypoints.rbegin(); it != waypoints.rend(); ++it) {
					auto& waypoint = *it;
					if (waypoint->getGlobalBounds().contains(m_editor.getMousePos())) {
						m_selection.activate(waypoint);
						m_selection.grabOffset = m_editor.getMousePos() - m_selection.object->getPosition();
						break;
					}
				}
				break;
			}
		}
	}
	else if (event.mouseButton.button == sf::Mouse::Right &&
			m_selectionState == SelectionState::Placing &&
			!waypoints.empty()) {

		waypoints.pop_back();
		if (!waypoints.empty()) {
			auto& wp = waypoints.back();
			m_selection.object->m_headLine = wp->m_tailLine;
			wp->m_tailLine->setEnd(m_selection.object);
		}
	}
}

void WaypointState::onMouseMoved(sf::Event& event, float dt)
{
	switch (m_selectionState)
	{
		case SelectionState::Placing:
		{
			auto& wp = m_selection.object;
			assert(wp != nullptr);
			wp->setPosition(m_editor.getMousePos() - m_selection.grabOffset);
			if (wp->m_headLine) {
				wp->m_headLine->updatePosition();
			}
			break;
		}
		case SelectionState::Moving:
		{
			if (m_selection.isActive()) {
				auto& wp = m_selection.object;
				wp->setPosition(m_editor.getMousePos() - m_selection.grabOffset);

				if (wp->m_headLine) {
					wp->m_headLine->updatePosition();
				}
				if (wp->m_tailLine) {
					wp->m_tailLine->updatePosition();
				}
			}
			break;
		}
	}
}

void WaypointState::onKeyPressed(sf::Event& event, float dt) 
{
	auto& waypoints = m_editor.m_waypoints;

	switch (event.key.code)
	{
		case sf::Keyboard::Num2:
			if (!waypoints.empty()) {
				waypoints.back()->m_tailLine = nullptr;
			}
			m_editor.changeState(std::make_unique<ResAreaState>(m_editor));
			//m_editor.m_window.setMouseCursorVisible(true);
			break;

		case sf::Keyboard::Space: 
			if (m_selectionState == SelectionState::Placing) {
				//m_editor.m_window.setMouseCursorVisible(true);
				m_selection.deactivate();

				if (!waypoints.empty()) {
					waypoints.back()->m_tailLine = nullptr;
				}

				m_selectionState = SelectionState::Moving;
			} else {
				//m_editor.m_window.setMouseCursorVisible(false);
				auto waypoint = createWaypoint();
				m_selection.activate(waypoint);

				if (!waypoints.empty()) {
					waypoints.back()->m_tailLine = std::make_shared<PathLine>(waypoints.back(), waypoint);
					waypoint->m_headLine = waypoints.back()->m_tailLine;
					waypoint->m_headLine->updatePosition();
				}

				m_selectionState = SelectionState::Placing;
			}
			break;
	}
}

std::shared_ptr<Waypoint> WaypointState::createWaypoint()
{
	sf::Texture& texture = TextureManager::getInstance().getTexture("waypoint");
	auto waypoint = std::make_shared<Waypoint>(texture);
	auto center = sf::Vector2f(waypoint->getSize().x / 2.f, waypoint->getSize().y / 2.f);
	waypoint->setOrigin(center);
	waypoint->setPosition(m_editor.getMousePos());
	return waypoint;
}
