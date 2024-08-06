#include "Application.hpp"

// static ImGui_ImplVulkanH_Window g_MainWindowData;
static int g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

// Per-frame-in-flight
static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

static uint32_t s_CurrentFrameIndex = 0;

Application::Application()
{
	Init();
}

Application::~Application()
{
	Shutdown();
}

void Application::Init()
{
	// Setup GLFW window
	glfwSetErrorCallback(glfw_error_callback);

	WindowController::GetInstance().NewWindow();

	{
		Scene::SceneGraph scene;

		Scene::Shapes::Sphere sphere;
		sphere.Origin = Math::Vector3<float>(1, 2, 0);

		scene.Spheres.push_back(sphere);

		renderer = std::make_unique<Renderer>(scene);
	}

	// Setup Vulkan
	if (!glfwVulkanSupported())
	{
		std::cerr << "GLFW: Vulkan not supported!\n";
		return;
	}
	uint32_t extensions_count = 0;
	const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

	VulkanBackend& vulkanBackend = VulkanBackend::GetInstance();
	auto& renderContext = VulkanBackend::GetInstance().GetRenderContext();

	glfwGetFramebufferSize(
		WindowController::GetInstance().GetWindow(),
		&renderContext.Width,
		&renderContext.Height);

	VulkanBackend::GetInstance().SetupVulkan(extensions, extensions_count);


	/*ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;*/
	// SetupVulkanWindow(wd, surface, w, h);

	// s_AllocatedCommandBuffers.resize(wd->ImageCount);
	// s_ResourceFreeQueue.resize(wd->ImageCount);
}

void Application::Run()
{
	double t = 0.0;

	const double fpsLimit = 1.0 / 60.0;

	m_Running = true;

	VulkanBackend& vulkanBackend = VulkanBackend::GetInstance();


	while (glfwWindowShouldClose(WindowController::GetInstance().GetWindow()) == 0 && m_Running)
	{
		// PROCESS INPUT
		double newTime = glfwGetTime();
		double frameTime = newTime - m_LastFrameTime;
		m_LastFrameTime = newTime;

		float deltaTime = std::min(frameTime, fpsLimit);

		/*     m_gameController->ProcessInput(deltaTime);*/
		// m_gameController->Update(deltaTime);

		// Utilities::FPSCounter::CalculateFrameRate();

		// RENDER
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*      m_gameController->Render();*/

		// glfwSwapBuffers(WindowController::GetInstance().GetWindow());

		glfwPollEvents();

		if (!m_Image || VulkanBackend::GetInstance().GetRenderContext().Width != m_Image->GetWidth() || VulkanBackend::GetInstance().GetRenderContext().Height != m_Image->GetHeight())
		{

			m_Image = std::make_shared<Image>(VulkanBackend::GetInstance().GetRenderContext().Width, VulkanBackend::GetInstance().GetRenderContext().Height, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[VulkanBackend::GetInstance().GetRenderContext().Width * VulkanBackend::GetInstance().GetRenderContext().Height];
		}

		for (uint32_t y = 0; y < VulkanBackend::GetInstance().GetRenderContext().Height; y++)
		{
			for (uint32_t x = 0; x < VulkanBackend::GetInstance().GetRenderContext().Width; x++)
			{

				float normalizedX = (float)x / (float)VulkanBackend::GetInstance().GetRenderContext().Width;
				float normalizedY = (float)y / (float)VulkanBackend::GetInstance().GetRenderContext().Height;

				uint32_t idx = x + (y * VulkanBackend::GetInstance().GetRenderContext().Width);
				m_ImageData[idx] = renderer->PerPixel(normalizedX, normalizedY);
			}
		}

		m_Image->SetData(m_ImageData);


		VulkanBackend::GetInstance().drawFrame();
	}

	// m_gameController->CleanUp();
}

void Application::Close()
{
	m_Running = false;
}

void Application::Shutdown()
{
	// Cleanup
	VkResult err = vkDeviceWaitIdle(VulkanBackend::GetInstance().GetDevice());
	check_vk_result(err);

	// Free resources in queue
	for (auto &queue : s_ResourceFreeQueue)
	{
		for (auto &func : queue)
			func();
	}

	s_ResourceFreeQueue.clear();

	// ImGui_ImplVulkan_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();

	// CleanupVulkanWindow();
	// CleanupVulkan();

	glfwDestroyWindow(WindowController::GetInstance().GetWindow());
	glfwTerminate();

	m_Running = false;
}
