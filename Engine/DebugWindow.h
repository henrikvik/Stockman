#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <imgui.h>
#include <vector>

class DebugWindow
{
private:
	//https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp my information to make this shit

	char						m_inputBuf[256];
	bool						m_scrollToBottom;
	std::vector<char*>			m_items;
	std::vector<char*>			m_history;
	std::vector<const char*>	m_command;

public:
	DebugWindow();
	~DebugWindow();
	void clearLog();
	void playDebugWindow();
};
#endif