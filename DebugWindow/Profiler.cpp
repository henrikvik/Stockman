#include "Profiler.h"

#include <algorithm>

#include <stdio.h>
#include <math.h>

#include <psapi.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <Engine\Constants.h>

Profiler *g_Profiler;

#define PROFILE_GPU_START_INDEX 0
#define PROFILE_GPU_START_WORK_INDEX 1
#define PROFILE_GPU_END_INDEX 2
#define PROFILE_GPU_FREE_INDICES 3

// per-thread variable (TLS) that each begin()/end() call for each thread
// accesses
__declspec(thread) ProfilingThread t_LocalThreadProfiler;

// color gradients for color enums going from light to dark
static ImVec4 EVENT_COLORS[] =
{
	ImVec4(0.925f, 0.616f, 0.682f, 1.0f), // pink/dark
	ImVec4(0.773f, 0.384f, 0.467f, 1.0f),
	ImVec4(0.616f, 0.204f, 0.294f, 1.0f),
	ImVec4(0.463f, 0.078f, 0.161f, 1.0f),
	ImVec4(0.310f, 0.000f, 0.067f, 1.0f),

	ImVec4(1.000f, 0.929f, 0.667f, 1.0f), // pear
	ImVec4(0.831f, 0.745f, 0.416f, 1.0f),
	ImVec4(0.667f, 0.573f, 0.224f, 1.0f),
	ImVec4(0.502f, 0.412f, 0.082f, 1.0f),
	ImVec4(0.333f, 0.263f, 0.000f, 1.0f),

	ImVec4(0.420f, 0.580f, 0.624f, 1.0f), // sea/cyan/blue
	ImVec4(0.267f, 0.463f, 0.518f, 1.0f),
	ImVec4(0.149f, 0.357f, 0.416f, 1.0f),
	ImVec4(0.059f, 0.255f, 0.310f, 1.0f),
	ImVec4(0.008f, 0.165f, 0.208f, 1.0f),

    ImVec4(0.467f, 0.796f, 0.600f, 1.0f), // grass/green
	ImVec4(0.278f, 0.651f, 0.427f, 1.0f),
	ImVec4(0.157f, 0.533f, 0.310f, 1.0f),
	ImVec4(0.063f, 0.392f, 0.196f, 1.0f),
	ImVec4(0.000f, 0.212f, 0.086f, 1.0f),

    ImVec4(0.855f, 0.502f, 0.702f, 1.0f), // pink/bright
	ImVec4(0.725f, 0.310f, 0.545f, 1.0f),
	ImVec4(0.596f, 0.176f, 0.416f, 1.0f),
	ImVec4(0.439f, 0.067f, 0.278f, 1.0f),
	ImVec4(0.235f, 0.000f, 0.133f, 1.0f),

    ImVec4(0.604f, 0.455f, 0.792f, 1.0f), // magenta
	ImVec4(0.447f, 0.302f, 0.627f, 1.0f),
	ImVec4(0.329f, 0.196f, 0.494f, 1.0f),
	ImVec4(0.220f, 0.110f, 0.353f, 1.0f),
	ImVec4(0.122f, 0.051f, 0.208f, 1.0f),

    ImVec4(1.000f, 0.961f, 0.537f, 1.0f), // dark yellow
	ImVec4(0.925f, 0.882f, 0.404f, 1.0f),
	ImVec4(0.729f, 0.690f, 0.251f, 1.0f),
	ImVec4(0.522f, 0.490f, 0.129f, 1.0f),
	ImVec4(0.310f, 0.286f, 0.051f, 1.0f),

    ImVec4(1.000f, 0.690f, 0.639f, 1.0f), // red
	ImVec4(0.961f, 0.522f, 0.451f, 1.0f),
	ImVec4(0.827f, 0.353f, 0.278f, 1.0f),
	ImVec4(0.710f, 0.224f, 0.149f, 1.0f),
	ImVec4(0.561f, 0.122f, 0.051f, 1.0f),
};

inline void DrawTextLine(ImDrawList *list, ImVec2 start, ImVec2 end, const char *text, const char *len);

Profiler::Profiler(ID3D11Device *device, ID3D11DeviceContext *cxt)
	: m_Session({1.f}), m_Context(cxt), m_Frame({}), m_CaptureThisFrame(false), m_ThreadCount(0)
{
	QueryPerformanceFrequency(&m_Frequency);

	m_Self = GetCurrentProcess();

	GetSystemInfo(&m_Info);
	m_Processors = m_Info.dwNumberOfProcessors;
	
	HRESULT ret_code = ::CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_Factory));

	if (SUCCEEDED(ret_code)) {
		IDXGIAdapter* firstAdapter;
		ret_code = m_Factory->EnumAdapters(0, &firstAdapter);
		firstAdapter->QueryInterface(__uuidof(IDXGIAdapter3), (void**)&m_Adapter);
	}


	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.19f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.19f, 0.95f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.41f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.11f, 0.59f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.47f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.47f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.48f, 0.78f, 0.32f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.11f, 0.59f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.95f, 0.92f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.92f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.11f, 0.59f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.47f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	
	style.AntiAliasedLines = false;

	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 0.f;
}

Profiler::~Profiler()
{
}

void Profiler::start()
{
	if (m_CaptureThisFrame) {
		ZeroMemory(&m_Frame, sizeof(Frame));

		int i = 0;
		for (auto it = m_ThreadLocalProfilers.begin(); it != m_ThreadLocalProfilers.end(); ++it) {
			auto local = (*it);

			CopyMemory(&m_Frame.m_Threads[i].name, local->name, 32);
			local->current_idx = 0;
			local->thread = nullptr;
			local->level = 0;
			local->thread = &m_Frame.m_Threads[i++];
			local->active = true;
		}

		QueryPerformanceCounter(&m_Frame.start);
	}
}

void Profiler::frame()
{
	if (m_CaptureThisFrame) {
		m_CaptureThisFrame = false;

		QueryPerformanceCounter(&m_Frame.end);

		for (auto it = m_ThreadLocalProfilers.begin(); it != m_ThreadLocalProfilers.end(); ++it) {
			auto local = (*it);
			local->active = false;
		}

		m_Frame.finished = true;
	}
}

void Profiler::begin(const char * name, EventColor color)
{
	Thread *thread = t_LocalThreadProfiler.thread;
	if (thread && t_LocalThreadProfiler.active && m_CaptureThisFrame) {
		Event ev(name, color);
		QueryPerformanceCounter(&ev.start);

		if (t_LocalThreadProfiler.level == 0) {
			thread->events[thread->count++] = ev;
		}
		else {
			ev.parent = t_LocalThreadProfiler.current_idx;
			thread->events[thread->count++] = ev;
		}

		t_LocalThreadProfiler.level++;
		thread->depth = max(thread->depth, t_LocalThreadProfiler.level);
		t_LocalThreadProfiler.current_idx = thread->count - 1;
	}
}

void Profiler::beginf(const char * fmt, ...)
{
    Thread *thread = t_LocalThreadProfiler.thread;
    if (thread && t_LocalThreadProfiler.active && m_CaptureThisFrame) {
        Event ev;
        ev.color = EventColor::Inherit;
        va_list args;
        va_start(args, fmt);
        vsprintf_s(ev.name, fmt, args);
        va_end(args);
        
        
        QueryPerformanceCounter(&ev.start);

        if (t_LocalThreadProfiler.level == 0) {
            thread->events[thread->count++] = ev;
        }
        else {
            ev.parent = t_LocalThreadProfiler.current_idx;
            thread->events[thread->count++] = ev;
        }

        t_LocalThreadProfiler.level++;
        thread->depth = max(thread->depth, t_LocalThreadProfiler.level);
        t_LocalThreadProfiler.current_idx = thread->count - 1;
    }
}

void Profiler::end()
{
	Thread *thread = t_LocalThreadProfiler.thread;
	if (thread && t_LocalThreadProfiler.active && m_CaptureThisFrame) {
		auto &ev = thread->events[t_LocalThreadProfiler.current_idx];
		t_LocalThreadProfiler.current_idx = ev.parent;
		t_LocalThreadProfiler.level--;

		QueryPerformanceCounter(&ev.end);
	}
}

void Profiler::registerThread(const char * fmt, ...)
{
	// get current thread id
	auto tid = std::this_thread::get_id();

	m_ThreadLocalMutex.lock();
	
	bool exists = false;
	for (auto it = m_ThreadLocalProfilers.begin(); it != m_ThreadLocalProfilers.end(); it++) {
		if ((*it)->tid == tid)
			exists = true;
	}

	if (!exists) {
		va_list args;
		va_start(args, fmt);
		vsprintf_s(t_LocalThreadProfiler.name, fmt, args);
		va_end(args);

		m_ThreadCount++;
		m_ThreadLocalProfilers.push_back(&t_LocalThreadProfiler);
	}

	m_ThreadLocalMutex.unlock();
}

void Profiler::unregisterThread()
{
	// get current thread id
	auto tid = std::this_thread::get_id();

	m_ThreadLocalMutex.lock();

	for (auto it = m_ThreadLocalProfilers.begin(); it != m_ThreadLocalProfilers.end(); it++) {
		if (tid == (*it)->tid) {
			m_ThreadLocalProfilers.erase(it);
			m_ThreadCount--;
			break;
		}
	}

	m_ThreadLocalMutex.unlock();
}

void Profiler::capture()
{
	m_CaptureThisFrame = true;
}

void Profiler::poll()
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(m_Self, &pmc, sizeof(pmc));
	m_RAM = pmc.WorkingSetSize;

	DXGI_QUERY_VIDEO_MEMORY_INFO info;
	if (SUCCEEDED(m_Adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
	{
		m_VRAM = info.CurrentUsage;
	}
}

void Profiler::render()
{
	if (m_CaptureThisFrame) return;

	ImGui::Begin("Profiler");

	Frame frame = m_Frame;
	int threadCount = m_ThreadCount;

	ImGui::PushItemWidth(142);
	ImGui::BeginGroup();
	size_t limit = 1024 * 1024 * 256;

	auto vram = getVRAM();
	auto ram = getRAM();
	auto cpu = getCPU();

	char overlay_buf[64];

	_snprintf_s(overlay_buf, 64, "%.1f MB", ram / 1024 / 1024.f);
	ImGui::ProgressBar(ram / (float)limit, ImVec2(142, 0), overlay_buf);
	ImGui::SameLine();
	ImGui::Text(" RAM");

	_snprintf_s(overlay_buf, 64, "%.1f MB", vram / 1024 / 1024.f);
	ImGui::ProgressBar(vram / (float)limit, ImVec2(142, 0), overlay_buf);
	ImGui::SameLine();
	ImGui::Text("VRAM");

	ImGui::SliderFloat("scale", &m_Session.zoom, .5f, 3.f, "%.1fx zoom");
	ImGui::EndGroup();
	ImGui::PopItemWidth();

	ImGui::SameLine();

	auto outsideDim = ImGui::GetContentRegionAvail();
	auto outsideCursor = ImGui::GetCursorScreenPos();

	ImGui::BeginChild("atimelineview", ImVec2(0, 70), true);

	auto insideCursor = ImGui::GetCursorScreenPos();
	auto insideDim = ImGui::GetContentRegionAvail();

	ImGui::EndChild();

	bool threads_open[16] = { true, false, false, false, false };
	ImGui::BeginChild("threads", ImVec2(180, 0), true);
	{
		for (int i = 0; i < threadCount; i++) {
			Thread &thread = frame.m_Threads[i];
			if (ImGui::CollapsingHeader(thread.name, "asdasdasd", true, threads_open[i])) {
				threads_open[i] = true;
				if (thread.depth > 0) {
					ImGui::BeginChild(thread.name + 1, ImVec2(0, max(1.f, float(thread.depth - 1) * 19)), true);
					ImGui::EndChild();
				}
            }
            else {
                threads_open[i] = false;
            }
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("timeline", ImVec2(0, 0), true, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_HorizontalScrollbar);

	auto dend = ToMilliseconds(frame.start, frame.end);
	auto factor = (insideDim.x / dend) * m_Session.zoom;
	auto frameEnd = factor * dend;
	float y = 0.f;

	if (frame.finished) {
		for (int i = 0; i < threadCount; i++) {
			Thread &thread = frame.m_Threads[i];
			temp.outerCursor = outsideCursor;
			temp.innerCursor = insideCursor;
			temp.factor = factor;
			temp.yoffset = y;

			if (thread.count >= 1) {
				RenderEventNodes(thread, frame.start, 0, 0, threads_open[i]);
			}

			if (threads_open[i])
				y += (thread.depth) * 19 + 8;
			else
				y += 19 + 4;
		}
	}

	auto scroll = ImGui::GetScrollX();// / fmax(1.f, ImGui::GetScrollMaxX());
	auto maxW = ImGui::GetScrollMaxX() - ImGui::GetWindowContentRegionWidth();

	ImGui::EndChild();

	if (scroll > 0) frameEnd -= 8.f;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->PushClipRect(outsideCursor, ImVec2(outsideCursor.x + outsideDim.x, outsideCursor.y + outsideDim.y));
	auto c = min(1000, (int)ceil(dend));
	for (int i = 0; i < c + 1; i++) {
		bool special = (i % 5 == 0) || (i == c);
		float downset = (special ? 10.f : 6.f);

		float f = (i == c) ? dend : (float)i;
		float w = frameEnd * (f / dend) - scroll;
		drawList->AddLine(
			ImVec2(insideCursor.x + w, insideCursor.y),
			ImVec2(insideCursor.x + w, insideCursor.y + downset),
			special ? 0xff888888 : 0xff676767,
			1.f
		);

		if (special) {
			char text_ms[16];
			auto len = sprintf_s(text_ms, "%.1fms", (i == c ? dend : (float)i));
			auto sz = ImGui::CalcTextSize(text_ms, text_ms + len);

			drawList->AddText(
				ImVec2(insideCursor.x + w - sz.x * (f / dend), insideCursor.y + downset),
				0xff848484,
				text_ms,
				text_ms + len
			);
		}
	}


	{
		auto frameLineOffset = 52.f;
		char line_text[32];
		auto len = sprintf_s(line_text, "CPU %.1fms", (float)dend);
		auto sz = ImGui::CalcTextSize(line_text, line_text + len);
		auto lineEnd = frameEnd;

		DrawTextLine(
			drawList,
			ImVec2(insideCursor.x - scroll, insideCursor.y + frameLineOffset),
			ImVec2(insideCursor.x + lineEnd - scroll, insideCursor.y + frameLineOffset),
			line_text,
			line_text + len
		);
	}

	drawList->PopClipRect();

	ImGui::End();
}

// danger ahead
void Profiler::RenderEventNodes(Thread thread, LARGE_INTEGER base, int idx, int depth, bool children)
{
	Event *prev = nullptr;
	Event *parent = nullptr;

	LONGLONG maxEnd = 0;
	int colDepth = 0;

	for (int i = idx; i < thread.count; i++) {
		Event &evt = thread.events[i];
		float start = ToMilliseconds(base, evt.start);
		float end = ToMilliseconds(base, evt.end);

		if (prev) {
			if (evt.end.QuadPart <= prev->end.QuadPart) {
				if (!children) continue;
				depth += 1;
				
				if (prev->color != EventColor::Inherit) {
					colDepth = depth;
				}

				parent = prev;
				maxEnd = prev->end.QuadPart;
			}
			else if (evt.end.QuadPart >= maxEnd) {
				if (depth > 0) {
					depth--;
					parent = &thread.events[parent->parent];
					maxEnd = parent->end.QuadPart;
				}
			}
		}

		ImGui::SetCursorPosX((temp.innerCursor.x - temp.outerCursor.x) + start * temp.factor);
		ImGui::SetCursorPosY(temp.yoffset + 8 + 19 * depth);

		if (evt.color == EventColor::Inherit) {
            // TODO: add lazy random color if we dont have a parent
			evt.color = parent ? parent->color : (EventColor)((i % 4) * 5 + 1 + (depth % 4));
		}
		auto col_idx = ((int)evt.color - 1) * 5;
		auto col = EVENT_COLORS[col_idx + 1 + (depth) % 4];
		auto col_hover = EVENT_COLORS[col_idx];
		auto col_active = EVENT_COLORS[col_idx];

		ImGui::PushStyleColor(ImGuiCol_Button, col);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);

		float f = 0.5f * temp.factor;
		ImGui::ButtonEx(evt.name, ImVec2(max(ToMilliseconds(evt.start, evt.end) * temp.factor, 0.1), 0));

		ImGui::PopStyleColor(3);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s:\nduration: %.3fms\nstart: %.3fms\nend: %.3fms", evt.name, end - start, start, end);

		prev = &thread.events[i];
		if (evt.end.QuadPart > maxEnd) {
			//parent = prev;
			maxEnd = evt.end.QuadPart;
		}
	}
}

// draws a line with centered text using imgui, looks something like this:
// |---- text ----|
inline void DrawTextLine(ImDrawList *list, ImVec2 start, ImVec2 end, const char *text, const char *len)
{
	auto sz = ImGui::CalcTextSize(text, len);

	auto lineEnd = end.x - start.x;
	auto lineMidFirst = lineEnd / 2.f - (sz.x / 2.f) - 4.f;
	auto lineMidEnd = lineEnd / 2.f + (sz.x / 2.f) + 4.f;

	// left line butt
	list->AddLine(
		ImVec2(start.x, start.y - 5.f),
		ImVec2(start.x, start.y + 5.f),
		0xffeeeeee
	);

	// left line part
	list->AddLine(
		ImVec2(start.x, start.y),
		ImVec2(start.x + lineMidFirst, start.y),
		0xffeeeeee
	);

	// center text
	list->AddText(
		ImVec2(start.x + lineMidFirst + 4.f, start.y - sz.y / 2.f),
		0xffeeeeee,
		text,
		len
	);

	// right light butt
	list->AddLine(
		ImVec2(end.x, end.y - 5.f),
		ImVec2(end.x, end.y + 5.f),
		0xffeeeeee
	);

	// right line part
	list->AddLine(
		ImVec2(start.x + lineMidEnd, start.y),
		ImVec2(end.x, end.y),
		0xffeeeeee
	);
}
