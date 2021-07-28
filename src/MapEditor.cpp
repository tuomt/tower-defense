#pragma once
#include "MapEditor.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>
#include "Helper.h"
#include "Collision.h"
#include "nlohmann/json.hpp"

using namespace Helper;
using json = nlohmann::json;

MapEditor::MapEditor(sf::RenderWindow& window)
	: Scene(window) {
	// Load default font from a file
	if (!font.loadFromFile(DEFAULT_FONT_PATH))
	{
		return;
	}

	if (!mapTexture.loadFromFile("../img/level_1.png")) {
		std::cout << "Failed to load map texture!\n";
		return;
	}

	mapSprite.setTexture(mapTexture);

	if (!waypointTexture.loadFromFile("../img/waypoint.png")) {
		std::cout << "Failed to load waypoint texture!\n";
		return;
	}
	waypointTexture.setSmooth(true);

	if (!restrictedAreaTexture.loadFromFile("../img/restricted_area.png")) {
		std::cout << "Failed to load restricted area texture!\n";
		return;
	}
	restrictedAreaTexture.setSmooth(true);
	restrictedAreaTexture.setRepeated(true);

	if (!knobTexture.loadFromFile("../img/resize_knob.png")) {
		return;
	}
	knobTexture.setSmooth(true);

	//state = MapEditor::State::GENERAL;

	// Init controls text
	controlsText.setPosition(10.f, 10.f);
	controlsText.setFont(font);
	controlsText.setCharacterSize(20);
	controlsText.setString("Hide controls - h\nPlace a marker - Left mouse button\nRemove last marker - Right mouse button\nToggle waypoint placing - e\nSave map - s");
	controlsText.setFillColor(sf::Color::Black);

	controlsBackground.setSize(sf::Vector2f(controlsText.getGlobalBounds().width + 10.f, controlsText.getGlobalBounds().height + 10.f));
	controlsBackground.setFillColor(sf::Color::White);
	controlsBackground.setOutlineColor(sf::Color::Black);
	controlsBackground.setOutlineThickness(2);
	controlsBackground.setPosition(controlsText.getPosition().x - 5.f, controlsText.getPosition().y - 5.f);
}

void MapEditor::handleInput(sf::Event& event, float dt) {
	if (event.type == sf::Event::Closed)
		m_window.close();
	// Mouse movement
	else if (event.type == sf::Event::MouseMoved) {
		if (state == MapEditor::State::GENERAL) {
			if (selectedRestrictedArea != NULL) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);
				switch (selectionState) {
				case MapEditor::SelectionState::MOVING:
				{
					sf::Vector2f offset = mousePos - selectedRestrictedArea->getPosition();
					auto& pos = selectedRestrictedArea->getPosition();
					selectedRestrictedArea->setPosition(mousePos - grabOffset);
					break;
				}
				case MapEditor::SelectionState::ROTATING:
					selectedRestrictedArea->rotateToPoint(mousePos);
					break;
				case MapEditor::SelectionState::RESIZING:
					selectedRestrictedArea->resize(mousePos);
					auto& size = selectedRestrictedArea->getSize();
					// Set origin to center
					//selectedRestrictedArea->setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
					break;
				}
			}
		}
		else {
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);
			selectedComponent->setPosition(mousePos);
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		// Left mouse button
		if (event.mouseButton.button == sf::Mouse::Left) {
			switch (state) {
			case MapEditor::State::GENERAL:
			{
				bool selected = false;
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);
				// Iterate from end to beginning so that the component that was placed first is not selected 
				// in case two components overlap
				for (auto area = restrictedAreas.rbegin(); area != restrictedAreas.rend(); ++area) {
					// Detect if a resize knob is selected
					if (area->getKnob().getGlobalBounds().contains(mousePos)) {
						// Select the component
						selectedRestrictedArea = &*area;
						if (selectionState != MapEditor::SelectionState::ROTATING) {
							selectionState = MapEditor::SelectionState::RESIZING;
						}
						selected = true;
						break;
					}
					else if (area->getGlobalBounds().contains(mousePos)) {
						selectedRestrictedArea = &*area;
						grabOffset = mousePos - selectedRestrictedArea->getPosition();
						selectionState = MapEditor::SelectionState::MOVING;
						selected = true;
						break;
					}
				}

				if (!selected) {
					selectedRestrictedArea = NULL;
				}
				break;
			}
			case MapEditor::State::WAYPOINT:
			{
				// Create a new waypoint
				Waypoint waypoint(waypointTexture);
				sf::Vector2f center(waypoint.getSize().x / 2.f, waypoint.getSize().y / 2.f);
				waypoint.setOrigin(center);
				waypoint.setPosition(selectedComponent->getPosition());
				waypoints.push_back(waypoint);
				selectedComponent = &waypoints.at(waypoints.size() - 1);
				// Create a new line
				pathLines.push_back(sf::RectangleShape());
				selectedLine = &pathLines.at(pathLines.size() - 1);
				break;
			}
			case MapEditor::State::RES_AREA:
				// Create a new restricted area
				ResizableComponent restrictedArea(restrictedAreaTexture, knobTexture);
				restrictedArea.setPosition(selectedComponent->getPosition());
				restrictedAreas.push_back(restrictedArea);
				selectedComponent = &restrictedAreas.at(restrictedAreas.size() - 1);
				break;
			}
		}
		// Right mouse button
		else if (event.mouseButton.button == sf::Mouse::Right) {
			if (state == MapEditor::State::WAYPOINT && waypoints.size() > 1) {
				sf::Vector2f currentPosition = selectedComponent->getPosition();
				waypoints.pop_back();
				selectedComponent = &waypoints.at(waypoints.size() - 1);
				selectedComponent->setPosition(currentPosition);

				pathLines.pop_back();

				if (pathLines.size() == 0) {
					selectedLine = NULL;
				}
				else {
					selectedLine = &pathLines.at(pathLines.size() - 1);
				}
			}
			else if (state == MapEditor::State::RES_AREA && restrictedAreas.size() > 1) {
				sf::Vector2f currentPosition = selectedComponent->getPosition();
				restrictedAreas.pop_back();
				selectedComponent = &restrictedAreas.at(restrictedAreas.size() - 1);
				selectedComponent->setPosition(currentPosition);
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (selectedRestrictedArea != NULL) {
			selectedRestrictedArea = NULL;
			//selectedKnob = NULL;
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		// E
		if (event.key.code == sf::Keyboard::E) {
			if (state == MapEditor::State::GENERAL) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);
				Waypoint waypoint(waypointTexture);
				sf::Vector2f center(waypoint.getSize().x / 2.f, waypoint.getSize().y / 2.f);
				waypoint.setOrigin(center);

				//waypoint.setPosition(waypoints.at(waypoints.size() - 1).getPosition());
				waypoint.setPosition(mousePos);
				waypoints.push_back(waypoint);
				selectedComponent = &waypoints.at(waypoints.size() - 1);

				// Create a new line
				pathLines.push_back(sf::RectangleShape());
				selectedLine = &pathLines.at(pathLines.size() - 1);

				m_window.setMouseCursorVisible(false);
				state = MapEditor::State::WAYPOINT;
			}
			else if (state == MapEditor::State::WAYPOINT) {
				selectedComponent = NULL;
				selectedLine = NULL;
				waypoints.pop_back();
				if (pathLines.size() > 0) pathLines.pop_back();
				m_window.setMouseCursorVisible(true);
				state = MapEditor::State::GENERAL;
			}
		}
		// F
		else if (event.key.code == sf::Keyboard::F) {
			if (selectionState == MapEditor::SelectionState::ROTATING) {
				selectionState = MapEditor::SelectionState::MOVING;
			}
			else {
				selectionState = MapEditor::SelectionState::ROTATING;
			}
		}
		// R
		else if (event.key.code == sf::Keyboard::R) {
			if (state == MapEditor::State::GENERAL) {
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(m_window);
				ResizableComponent restrictedArea(restrictedAreaTexture, knobTexture);
				sf::Vector2f center(restrictedArea.getSize().x / 2.f, restrictedArea.getSize().y / 2.f);
				//restrictedArea.setOrigin(center);
				restrictedAreas.push_back(restrictedArea);
				selectedComponent = &restrictedAreas.at(restrictedAreas.size() - 1);

				m_window.setMouseCursorVisible(false);
				state = MapEditor::State::RES_AREA;
			}
			else if (state == MapEditor::State::RES_AREA) {
				selectedComponent = NULL;
				restrictedAreas.pop_back();
				m_window.setMouseCursorVisible(true);
				state = MapEditor::State::GENERAL;
			}
		}
		// S
		else if (event.key.code == sf::Keyboard::S) {
			if (state == MapEditor::State::GENERAL) {
				// Initialize map data
				MapData mapData;
				mapData.name = "level_2";
				mapData.waypoints = &waypoints;

				std::thread saveMapFileThread(&MapEditor::saveMap, this, mapData);
				saveMapFileThread.detach();
			}
		}
		// H
		else if (event.key.code == sf::Keyboard::H) {
			if (showControls) showControls = false;
			else showControls = true;
		}
	}
}

void MapEditor::update(float dt)
{
}

void MapEditor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_window.draw(mapSprite);

	for (auto& area : restrictedAreas) {
		m_window.draw(area);
	}

	for (auto& waypoint : waypoints) {
		m_window.draw(waypoint);
	}

	if (state == MapEditor::State::WAYPOINT && waypoints.size() > 1) {
		// Create a line from the selected marker to the last marker that has been placed
		auto point1 = waypoints.at(waypoints.size() - 2).getPosition();
		auto point2 = selectedComponent->getPosition();
		float radius = 25.f;

		if (!Collision::collides(point1, point2, radius, radius)) {
			float angle = getAngle(point2, point1);
			point1 = getPerimeterPoint(point1, 25.f, angle);
			point2 = getPerimeterPoint(point2, -25.f, angle);
			float dist = getDistance(point2, point1);
			selectedLine->setSize(sf::Vector2f(dist, 2.f));
			selectedLine->setRotation(radToDeg(angle));
			selectedLine->setPosition(point1);
			selectedLine->setFillColor(sf::Color::Black);
		}
	}

	for (auto& line : pathLines) {
		m_window.draw(line);
	}

	if (showControls) {
		m_window.draw(controlsBackground);
		m_window.draw(controlsText);
	}
}


void MapEditor::saveMap(MapData mapData) {
	json map;
	map["waypoints"];
	for (auto& waypoint : waypoints) {
		json position = json::object();
		position["x"] = waypoint.getPosition().x;
		position["y"] = waypoint.getPosition().y;
		map["waypoints"].push_back(position);
		//map["waypoints"]["position"] = { {"x", pos.x}, {"y", pos.y} };
	}

	map["restrictedAreas"];
	for (auto& area : restrictedAreas) {
		json size = json::object();
		size["x"] = area.getSize().x;
		size["y"] = area.getSize().y;

		json position = json::object();
		position["x"] = area.getPosition().x;
		position["y"] = area.getPosition().y;

		json origin = json::object();
		origin["x"] = area.getOrigin().x;
		origin["y"] = area.getOrigin().y;

		json areaObject = json::object() = {
			{"size", size},
			{"position", position},
			{"origin", origin},
			{"rotation", area.getRotation()}
		};
		map["restrictedAreas"].push_back(areaObject);
	}

	std::ofstream file("../cfg/maps/" + mapData.name + ".json");

	if (file.is_open()) {
		file << std::setw(4) << map << std::endl;
		file.close();
		std::cout << "Map saved successfully.\n";
	}
	else {
		std::cout << "Could not save the map. Failed to open file.\n";
	}
}
