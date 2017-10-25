#include "DebugWindow.h"
#define ARRAYSIZE(ARR) ((int)(sizeof(ARR)/sizeof(*ARR)))
DebugWindow::DebugWindow()
{
	clearLog();
	m_historyPos = -1;
	m_command.push_back("HELP");
	m_command.push_back("HISTORY");
	m_command.push_back("CLEAR");
	addLog("Welcome to the Debug Window");
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

void DebugWindow::draw(const char * title)
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_Always);
	if (ImGui::Begin(title))
	{
		ImGui::TextWrapped("This is Dr.Stockman Studios Debug Window");
		ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

		if (ImGui::SmallButton("Clear"))
		{
			clearLog();
		}

		ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy");
		ImGui::SameLine();

		if (ImGui::SmallButton("Scroll to bottom"))
		{
			m_scrollToBottom = true;
		}

		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		static ImGuiTextFilter filter;
		filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::PopStyleVar();

ImGui::Separator();

ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
if (ImGui::BeginPopupContextWindow())
{
	if (ImGui::Selectable("Clear"))
	{
		clearLog();
	}
	ImGui::EndPopup();
}
ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

if (copy_to_clipboard)
{
	ImGui::LogToClipboard();
}

for (int i = 0; i < m_items.size(); i++)
{
	const char* item = m_items[i];

	if (!filter.PassFilter(item))
	{
		continue;
	}

	ImVec4 col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (strstr(item, "[error]"))
	{
		col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
	}
	else if (strncmp(item, "# ", 2) == 0)
	{
		col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
	}

	ImGui::PushStyleColor(ImGuiCol_Text, col);
	ImGui::TextUnformatted(item);
	ImGui::PopStyleColor();
}

if (copy_to_clipboard)
{
	ImGui::LogFinish();
}

if (m_scrollToBottom)
{
	ImGui::SetScrollHere();
}

m_scrollToBottom = false;
ImGui::PopStyleVar();
ImGui::EndChild();

ImGui::Separator();

ImGuiTextEditCallback callback = [](ImGuiTextEditCallbackData* data) -> int
{
	DebugWindow* temp = (DebugWindow*)data->UserData;
	return temp->TextEditCallback(data);
};

if (ImGui::InputText("Input", m_inputBuf, ARRAYSIZE(m_inputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
	ImGuiInputTextFlags_CallbackHistory, callback, (void*)this))
{
	char* input_end = m_inputBuf + strlen(m_inputBuf);
	while (input_end < m_inputBuf && input_end[-1] == ' ')
	{
		input_end--;
	}

	*input_end = 0;
	if (m_inputBuf[0])
	{
		doCommand(m_inputBuf);
	}
	strcpy_s(m_inputBuf, "");
}
	}

	if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
	{
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
}

void DebugWindow::doCommand(const char* command_line)
{
	addLog("# %s\n", command_line);

	// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
	m_historyPos = -1;

	for (int i = m_history.size() - 1; i >= 0; i--)
	{
		if (Stricmp(m_history[i], command_line) == 0)
		{
			free(m_history[i]);
			m_history.erase(m_history.begin() + i);
			break;
		}
	}

	m_history.push_back(Strdup(command_line));

	// Process command
	if (Stricmp(command_line, "CLEAR") == 0)
	{
		clearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0)
	{
		addLog("Commands:");
		for (int i = 0; i < m_command.size(); i++)
			addLog("- %s", m_command[i]);
	}
	else if (Stricmp(command_line, "HISTORY") == 0)
	{
		int first = m_history.size() - 10;
		for (int i = first > 0 ? first : 0; i < m_history.size(); i++)
			addLog("%3d: %s\n", i, m_history[i]);
	}
	else
	{
		addLog("Unknown command: '%s'\n", command_line);
	}

}

int DebugWindow::TextEditCallback(ImGuiTextEditCallbackData * data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		// Locate beginning of current word
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;

		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		// Build a list of candidates
		std::vector<const char*> candidates;

		for (int i = 0; i < m_command.size(); i++)
		{
			if (Strnicmp(m_command[i], word_start, (int)(word_end - word_start)) == 0)
			{
				candidates.push_back(m_command[i]);
			}
		}

		if (candidates.size() == 0)
		{
			// No match
			addLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		}
		else if (candidates.size() == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
		}
		else
		{
			int match_len = (int)(word_end - word_start);

			int j = 0;
			while (j == 0)
			{
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
				{
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				}
				if (!all_candidates_matches)
				{
					break;
				}
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}

			// List matches
			addLog("Possible matches:\n");
			for (int i = 0; i < candidates.size(); i++)
			{
				addLog("- %s\n", candidates[i]);
			}
		}

		break;
	}

	case ImGuiInputTextFlags_CallbackHistory:
	{
		const int prev_history_pos = m_historyPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (m_historyPos == -1)
				m_historyPos = m_history.size() - 1;
			else if (m_historyPos > 0)
				m_historyPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (m_historyPos != -1)
				if (++m_historyPos >= m_history.size())
					m_historyPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != m_historyPos)
		{
			data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (m_historyPos >= 0) ? m_history[m_historyPos] : "");
			data->BufDirty = true;
		}

		break;
	}
	}
	return 0;
}
