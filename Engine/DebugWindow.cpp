#include "DebugWindow.h"
#define ARRAYSIZE(ARR) ((int)(sizeof(ARR)/sizeof(*ARR)))
DebugWindow::DebugWindow()
{
	clearLog();
	m_historyPos = -1;
	m_command.push_back("HELP");
	m_command.push_back("HISTORY");
	m_command.push_back("CLEAR");
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
	addLog("Welcome to the Debug Window");
}

void DebugWindow::addLog(const char* command_line, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, command_line);
	vsnprintf(buf, ARRAYSIZE(buf), command_line, args);
	buf[ARRAYSIZE(buf) - 1];
	va_end(args);
	m_items.push_back(Strdup(buf));
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

void DebugWindow::doCommand(const char* command_line)
{
}
