#include "ExampleApp.h"

#define SEND_APP_EVENT(eventName) static_cast<ExampleApp*>(glfwGetWindowUserPointer(window))->OnEvent(eventName)

void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void ExampleApp::Run() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_Window = glfwCreateWindow(800, 800, m_WindowTitle, nullptr, nullptr);
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetKeyCallback(m_Window, WindowKeyCallback);

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	this->Init();
	
	while (!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();
		this->Update();
		glfwSwapBuffers(m_Window);
	}

	glfwTerminate();
}

int main(int argc, const char** argv) {
	auto* app = ExampleApp::Create(argc, argv);

	app->Run();

	delete app;
	return 0;
}

void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Event keyEvent;
	keyEvent.Params[0] = key;

	switch (action) {
	case GLFW_PRESS: 
		keyEvent.Type = EVENT_KEYPRESS;
		break;
	case GLFW_RELEASE:
		keyEvent.Type = EVENT_KEYRELEASE;
		break;
	}

	SEND_APP_EVENT(keyEvent);
}