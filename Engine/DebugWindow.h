#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <imgui.h>
#include <vector>
#include <ctype.h>
#include <functional>
#include <iostream>
#include <string>
#pragma region Comment

/*
Class: DebugWindow
Author: Andreas Eriksson

Description:
This class creates a debugwindow which is a singleton. This system can have functions and commands inserted into it by easily using the registerCommand which in turn uses lambda, all functions aqquiered
returns strings for some kind of input into the system. Make sure to always use caps when you create your command and that you only call the ammount of arguments that you have placed within it.

*/

#pragma endregion Description of class
class DebugWindow
{
private:
	//https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp my information to make this shit
	typedef std::function < std::string(std::vector<std::string>&)> CommandFunction;
	char								m_inputBuf[256];
	bool								m_scrollToBottom;
	std::vector<char*>					m_items;
	std::vector<char*>					m_history;
	int									m_historyPos;
	std::vector<const char*>			m_command;
	std::vector<CommandFunction>		m_functions;

	bool isDrawing = false;

	static DebugWindow*				instance;

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
	static DebugWindow* getInstance();
	static void releaseInstance();
	void clearLog();
	void addLog(const char* command_line, ...) IM_FMTARGS(2);
	void draw();
	void doCommand(const char* command_line);
	int TextEditCallback(ImGuiTextEditCallbackData* data);
	void registerCommand(const char* command, CommandFunction function);
	void toggleDebugToDraw() { isDrawing = !isDrawing; }
	bool isOpen() const { return isDrawing; }
};
#endif