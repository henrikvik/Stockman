#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <mutex>

#include <d3d11_3.h>
#include <dxgi1_4.h>

#include <imgui.h>
#include <tbb\tbb.h>

#pragma comment(lib, "dxgi.lib")

// use sized arrays where possible, to avoid dynamic allocation as much as
// possible
//
// change constants when needed
#define PROFILER_MAX_THREAD_EVENTS 256
#define PROFILER_MAX_THREAD_MARKERS 64
#define PROFILER_MAX_GPU_QUERIES 32
#define PROFILER_MAX_THREADS 8

// we use pre-processor macros to invoke the profiler, because it's much
// simpler to replace a macro with an empty body compared to if-deffing out a
// class method body
#define PROFILE_BEGINC(msg, col) g_Profiler->begin(msg, col);
#define PROFILE_BEGINF(msg, ...) g_Profiler->beginf(msg, __VA_ARGS__);
#define PROFILE_BEGIN(msg) g_Profiler->begin(msg);
#define PROFILE_END() g_Profiler->end();

enum class EventColor {
	Inherit = 0,
	PinkDark = 1,
	Pear = 2,
	Cyan = 3,
	Green = 4,
	PinkLight = 5,
	Magenta = 6,
	Yellow = 7,
	Red = 8,
};

struct Marker {
	char description[128];
	LARGE_INTEGER time;
};

struct Event {
	char name[32];
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	EventColor color;
	short parent, child, next;

	Event()
		: start({}), end({}), color(EventColor::Inherit), parent(0), child(0), next(0)
	{
	}

	Event(const char *ename, EventColor color)
		: start({}), end({}), color(color), parent(0), child(0), next(0)
	{
		strncpy_s(name, ename, strlen(ename));
	}
};

struct Thread {
	char name[32];
	int count;
	int markerCount;
	int depth;
	Event events[PROFILER_MAX_THREAD_EVENTS];
	Marker markers[PROFILER_MAX_THREAD_MARKERS];
};

struct Frame {
	Thread m_Threads[PROFILER_MAX_THREADS];

	LARGE_INTEGER start;
	LARGE_INTEGER end;

	double startGPU;
	double startWorkGPU;
	double endGPU;

	bool finished;
};

struct ProfilingThread {
	char name[32];
	Thread *thread;
	std::thread::id tid;
	//tbb::tbb_thread::id tid;
	int current_idx;
	int level;

	bool active;
};

class Profiler
{
public:
	Profiler(ID3D11Device *device, ID3D11DeviceContext *cxt);
	~Profiler();

	void start();
	void frame();

    void begin(const char *name, EventColor color = EventColor::Inherit);
    void beginf(const char *fmt, ...);
    void end();

	void registerThread(const char *fmt, ...);
	void unregisterThread();

	void capture();

	void poll();
	void render();

	double getCPU() const { return m_CPU; }
	size_t getRAM() const { return m_RAM; }
	size_t getVRAM() const { return m_VRAM; }
private:
	void RenderEventNodes(Thread thread, LARGE_INTEGER base, int idx, int depth, bool children);

	float ToMilliseconds(LARGE_INTEGER time) const {
		double ms = double(time.QuadPart) / double(m_Frequency.QuadPart);
		
		return float(ms * 1000.0);
	}

	float ToMilliseconds(LARGE_INTEGER start, LARGE_INTEGER end) const {
		double ms = double(end.QuadPart - start.QuadPart) / double(m_Frequency.QuadPart);

		return float(ms * 1000.0);
	}

	std::atomic<int> m_ThreadCount;
	std::unordered_map<std::thread::id, ProfilingThread*> m_ThreadLocalProfile;

	std::mutex m_ThreadLocalMutex;
	std::vector<ProfilingThread*> m_ThreadLocalProfilers;


	struct TempRenderData {
		ImVec2 outerCursor;
		ImVec2 innerCursor;
		float factor;
		float yoffset;
	} temp;

	struct ProfilerSession {
		float zoom;
	} m_Session;

	bool m_CaptureThisFrame;

	Frame m_Frame;

	int m_Processors;
	double m_CPU;
	size_t m_RAM;
	size_t m_VRAM;

	HANDLE m_Self;
	LARGE_INTEGER m_Frequency;

	SYSTEM_INFO m_Info;

	IDXGIFactory *m_Factory;
	IDXGIAdapter3 *m_Adapter;

	ID3D11DeviceContext *m_Context;
};

extern Profiler *g_Profiler;

class ScopedProfile {
public:
	ScopedProfile(const char *msg, EventColor col = EventColor::Inherit) {
		g_Profiler->begin(msg, col);
	}
	~ScopedProfile() {
		g_Profiler->end();
	}
};

class TbbProfilerObserver : public tbb::task_scheduler_observer {
public:
	TbbProfilerObserver(Profiler *profiler)
		: m_Profiler(profiler)
	{
		observe(true);
	}

	void on_scheduler_entry(bool worker) override {
		if (worker)
			m_Profiler->registerThread("Worker Thread #%d", m_ThreadId.fetch_and_increment());
		//else
		//	m_Profiler->registerThread("Main Thread");

	}
	void on_scheduler_exit(bool worker) override {
		m_Profiler->unregisterThread();
	}
private:
	tbb::atomic<int> m_ThreadId;
	Profiler *m_Profiler;
};