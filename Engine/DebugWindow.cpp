#include "DebugWindow.h"

DebugWindow::DebugWindow()
{
}

DebugWindow::~DebugWindow()
{
	clearLog();
	for (int i = 0; i < m_history.size(); i++)
	{
		free(m_history.at(i));
	}
}

void DebugWindow::clearLog()
{
	for (int i = 0; i < m_items.size(); i++)
	{
		free(m_items.at(i));
	}
	m_items.clear();
	m_scrollToBottom = true;
}

void DebugWindow::playDebugWindow()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_Once);
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::End();
	}
}