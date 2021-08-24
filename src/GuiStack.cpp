#include "GuiStack.h"

void GuiStack::push(std::shared_ptr<Gui> gui)
{
	GuiTask task =
	{
		GuiTask::Type::Push,
		gui
	};

	m_taskQueue.push(task);
}

void GuiStack::pop()
{
	GuiTask task =
	{
		GuiTask::Type::Pop,
		nullptr
	};

	m_taskQueue.push(task);
}

void GuiStack::update()
{
	while (!m_taskQueue.empty()) {
		// Get the next task
		auto& task = m_taskQueue.front();

		switch (task.type)
		{
		case GuiTask::Type::Pop:
			// Remove Gui
			m_guiStack.pop();
			break;
		case GuiTask::Type::Push:
			// Add Gui
			m_guiStack.push(task.gui);
			break;
		}

		// Remove the task
		m_taskQueue.pop();
	}
}

const std::stack<std::shared_ptr<Gui>>& GuiStack::get() const
{
	return m_guiStack;
}
