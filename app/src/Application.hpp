#ifndef APPLICATION_H
#define APPLICATION_H

#include "StandardIncludes.hpp"
#include "quanta.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "WindowController.hpp"
#include "VulkanBackend.hpp"

void check_vk_result(VkResult err);

class Application
{
public:
	Application();
	~Application();

	void Run();
	void Close();

	static VkInstance GetInstance();
	static VkPhysicalDevice GetPhysicalDevice();
	static VkDevice GetDevice();

	static VkCommandBuffer GetCommandBuffer(bool begin);
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

	static void SubmitResourceFree(std::function<void()> &&func);

private:
	void Init();
	void Shutdown();

	bool  m_Running = false;
	float m_LastFrameTime;

	VulkanBackend::Quanta_ImplVulkanH_RenderContext m_Quanta_ImplVulkanH_RenderContext;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
};

#endif