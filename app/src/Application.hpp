#ifndef APPLICATION_H
#define APPLICATION_H

#include "StandardIncludes.hpp"
#include "quanta.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "WindowController.hpp"
#include "VulkanBackend.hpp"
#include "Random.hpp"
#include "image.hpp"
#include "InputManager.hpp"
#include "../../editor/editor.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

// #include "MapleUI/maple.hpp"

void check_vk_result(VkResult err);

class Application
{
public:
	Application();
	~Application();

	void Run();
	void Close();

	static VkInstance GetInstance();

	static VkCommandBuffer GetCommandBuffer(bool begin);
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

	static void SubmitResourceFree(std::function<void()> &&func);

private:
	void Init();
	void Shutdown();

	bool m_Running = false;
	float m_LastFrameTime;

	uint32_t m_FrameIndex = 1;

	std::shared_ptr<Image> m_Image;
	uint32_t *m_ImageData = nullptr;
	glm::vec4 *m_AccumulationData = nullptr;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Editor> editor;

	Scene::SceneGraph scene;

	bool drawn;
	bool debug;
	glm::vec2 debug_trace_coord;
};

#endif
