#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <imgui.h>
#include <vector>
#include <ctype.h>

class DebugWindow
{
private:
	//https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp my information to make this shit

	char						m_inputBuf[256];
	bool						m_scrollToBottom;
	std::vector<char*>			m_items;
	std::vector<char*>			m_history;
	int							m_historyPos;
	std::vector<const char*>	m_command;

	static int Stricmp(const char* str1, const char* str2) 
	{
		int d; 
		while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) 
		{ 
			str1++; 
			str2++; 
		} 
		return d; 
	}
	static int Strnicmp(const char* str1, const char* str2, int n) 
	{ 
		int d = 0; 
		while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) 
		{ 
			str1++; 
			str2++; 
			n--; 
		} 
		return d; 
	}
	static char* Strdup(const char *str) 
	{ 
		size_t len = strlen(str) + 1;
		void* buff = malloc(len); 
		return (char*)memcpy(buff, (const void*)str, len); 
	}

public:
	DebugWindow();
	~DebugWindow();
	void clearLog();
	void addLog(const char* command_line, ...) IM_FMTARGS(2);
	void draw(const char* title);
	void doCommand(const char* command_line);
	int TextEditCallback(ImGuiTextEditCallbackData* data);
};
#endif