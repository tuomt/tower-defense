#pragma once
#include "MapEditor.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>
#include "Helper.h"
#include "CollisionHandler.h"
#include "WaypointState.h"
#include "nlohmann/json.hpp"
#include "SceneManager.h"

using namespace Helper;
using json = nlohmann::json;

MapEditor::MapEditor(sf::RenderWindow& window)
	: Scene(window), m_state(new WaypointState(*this)) {
	m_window.setKeyRepeatEnabled(false);
	loadTextures();
}

MapEditor::~MapEditor()
{
	unloadTextures();
}

void MapEditor::handleInput(sf::Event& event, float dt) {

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			SceneManager::getInstance().quit();
			break;
		case sf::Keyboard::S:
			saveMap("level_2");
			break;
		}
	}

	m_state->handleInput(event, dt);
}

void MapEditor::update(float dt)
{
}

void MapEditor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_window.draw(m_mapSprite);

	for (auto& area : m_restrictedAreas) {
		m_window.draw(*area);
	}

	for (auto& waypoint : m_waypoints) {
		m_window.draw(*waypoint);
		if (waypoint->m_tailLine) {
			m_window.draw(*waypoint->m_tailLine);
		}
	}

	m_state->draw(m_window, states);
}

void MapEditor::changeState(std::unique_ptr<EditorState> state)
{
	m_state = std::move(state);
}

void MapEditor::saveMap(std::string mapName) 
{
	json map;
	map["waypoints"];
	for (auto& waypoint : m_waypoints) {
		json position = json::object();
		position["x"] = waypoint->getPosition().x;
		position["y"] = waypoint->getPosition().y;
		map["waypoints"].push_back(position);
	}

	map["restrictedAreas"];
	for (auto& area : m_restrictedAreas) {
		json size = json::object();
		size["x"] = area->getSize().x;
		size["y"] = area->getSize().y;

		json position = json::object();
		position["x"] = area->getPosition().x;
		position["y"] = area->getPosition().y;

		json origin = json::object();
		origin["x"] = area->getOrigin().x;
		origin["y"] = area->getOrigin().y;

		json areaObject = json::object() = {
			{"size", size},
			{"position", position},
			{"origin", origin},
			{"rotation", area->getRotation()}
		};
		map["restrictedAreas"].push_back(areaObject);
	}

	std::ofstream file("../cfg/maps/" + mapName + ".json");

	if (file.is_open()) {
		file << std::setw(4) << map << std::endl;
		file.close();
		std::cout << "Map saved successfully.\n";
	}
	else {
		std::cout << "Could not save the map. Failed to open file.\n";
	}
}

void MapEditor::loadTextures()
{
	auto& tm = TextureManager::getInstance();

	tm.loadTexture("level_1", "level_1.png", this);
	m_mapSprite.setTexture(tm.getTexture("level_1"));

	tm.loadTexture("waypoint", "waypoint.png", this);
	tm.getTexture("waypoint").setSmooth(true);

	tm.loadTexture("restricted_area", "restricted_area.png", this);
	tm.getTexture("restricted_area").setSmooth(true);
	tm.getTexture("restricted_area").setRepeated(true);

	tm.loadTexture("resize_knob", "resize_knob.png", this);
	tm.getTexture("resize_knob").setSmooth(true);
}

void MapEditor::unloadTextures()
{
	TextureManager::getInstance().unloadSceneSpecificTextures(this);
}
