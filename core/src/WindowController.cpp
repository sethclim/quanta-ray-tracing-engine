
#include "WindowController.hpp"

#include "VulkanBackend.hpp"

namespace Window
{
	static WindowController *s_Instance = nullptr;

	WindowController& WindowController::GetInstance()
	{
		assert(s_Instance);
		return *s_Instance;
	};

	void WindowController::Init(VulkanBackend &vkBackend)
	{
		assert(!s_Instance);

		s_Instance = new WindowController(vkBackend);
	};

	void WindowController::ShutDown()
	{
		delete s_Instance;
		s_Instance = nullptr;
	};

	WindowController::WindowController(VulkanBackend &vkBackend) : m_VKBackend(vkBackend)
	{
		m_WindowHandle = nullptr;
	}

	WindowController::~WindowController()
	{
		if (m_WindowHandle != nullptr)
		{
			glfwTerminate();
			m_WindowHandle = nullptr;
		}
	}

	void WindowController::NewWindow()
	{

		M_ASSERT(glfwInit(), "Failed To Initialize GLFW"); // Initialize GLFW
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Open a window and create its OpenGL context
		// glfwWindowHint(GLFW_SAMPLES, 4);
		// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		M_ASSERT((m_WindowHandle = glfwCreateWindow(2048, 1536, "Quanta Raytracing Engine", NULL, NULL)) != nullptr, "Failed to open GLFW window.");
		glfwMakeContextCurrent(m_WindowHandle);
		glfwSetWindowUserPointer(m_WindowHandle, this);
		glfwSetFramebufferSizeCallback(m_WindowHandle, framebufferResizeCallback);
	}

	Resolution WindowController::GetResolution()
	{
		// const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		// return Resolution(mode->width, mode->height);
		return Resolution(1024, 768);
	}

	glm::vec2 WindowController::GetSize()
	{
		int width, height;
		glfwGetWindowSize(m_WindowHandle, &width, &height);
		return glm::vec2(width, height);
	}

	void WindowController::UpdateFrameBufferResized()
	{
		m_VKBackend.SetFrameBufferResized(true);
	}
}