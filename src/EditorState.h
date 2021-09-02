#pragma once
#include "MapEditor.h"

class MapEditor;

class EditorState
{
protected:
	MapEditor& m_editor;
public:
	EditorState(MapEditor& editor);
	virtual void handleInput(sf::Event& event, float dt) = 0;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};
