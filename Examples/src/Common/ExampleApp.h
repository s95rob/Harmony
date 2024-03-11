#pragma once

#define APP_WINDOW_WIDTH 800
#define APP_WINDOW_HEIGHT 800
#define APP_WINDOW_SIZE APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT

#include <glfw/glfw3.h>

extern int main(int, const char**);

enum EventType {
	EVENT_NONE = 0,
	EVENT_WINDOW_RESIZE, 
	EVENT_KEYPRESS, EVENT_KEYRELEASE
};

struct Event {
	EventType Type = EVENT_NONE;
	int Params[2] = { 0 };
};

class ExampleApp {
public:
	friend int main(int, const char**);

	virtual ~ExampleApp() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void OnEvent(Event& event) = 0;

	static ExampleApp* Create(int, const char**);

protected:
	bool m_Running = true;

	GLFWwindow* m_Window = nullptr;
	const char* m_WindowTitle = nullptr;

private:
	void Run();
};

#define EXAMPLE_NAME(name) Example##name##App

#define EXAMPLE_ENTRY(exampleClass) ExampleApp* ExampleApp::Create(int argc, const char** argv) \
	{ \
		auto* exampleInstance = new EXAMPLE_NAME(exampleClass)(); \
		exampleInstance->m_WindowTitle = "Harmony Example: " #exampleClass; \
		return exampleInstance; \
	} 

#define EXAMPLE_DECL(exampleClass) class EXAMPLE_NAME(exampleClass) : public ExampleApp \
	{	public: EXAMPLE_NAME(exampleClass)() = default; virtual void Init() override; virtual void Update() override; virtual void OnEvent(Event& event) override; }; \
	EXAMPLE_ENTRY(exampleClass) 


#define EXAMPLE_INIT(exampleClass) void EXAMPLE_NAME(exampleClass)::Init()
#define EXAMPLE_UPDATE(exampleClass) void EXAMPLE_NAME(exampleClass)::Update()
#define EXAMPLE_EVENT(exampleClass, eventName) void EXAMPLE_NAME(exampleClass)::OnEvent(Event& eventName)