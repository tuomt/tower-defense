#pragma once
#include <stack>
#include <queue>
#include "Gui.h"

/* 
	A singleton class for managing GUIs. 
	The purpose of this class is execute the GUI changes (push & pop) 
	after the current scene has updated completely. 
	This is accomplished by pushing the changes (GuiTask) into a queue.
	The queue can be cleared with the update() method which executes 
	the changes in the same order they were made.
*/

class GuiStack
{
public:
	static GuiStack& getInstance() {
		static GuiStack instance;
		return instance;
	}

private:
	GuiStack() {}

	struct GuiTask
	{
		enum class Type { Push, Pop };
		Type type;
		std::shared_ptr<Gui> gui;
	};

	std::queue<GuiTask> m_taskQueue;
	std::stack<std::shared_ptr<Gui>> m_guiStack;
public:
	// Prevent making a copy of the singleton
	GuiStack(const GuiStack&) = delete;
	GuiStack(GuiStack&&) = delete;
	GuiStack& operator=(const GuiStack&) = delete;
	GuiStack& operator=(GuiStack&&) = delete;

	const std::stack<std::shared_ptr<Gui>>& get() const;

	void push(std::shared_ptr<Gui> gui);
	void pop();
	void update();
};

