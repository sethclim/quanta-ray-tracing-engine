#include "Editor.hpp"

Editor::Editor()
{

	// // std::cout << "SETUP IMGUI" << std::endl;
	DrawData dd;

	// float vp_w = width * 0.2f;
	// float vp_h = height * 0.2f;

	float left_x = -1.0;
	float right_x = 1.0;
	// float right_x = 1.0;
	float top_y = 1.0;
	float bottom_y = -1.0;

	float btn_left_x = 1.0 * 0.55f;
	float btn_right_x = 0.9;
	float btn_top_y = 0.85f;
	float btn_bottom_y = 0.80f;

	const std::vector<Vertex> _vertices = {
		// Viewport
		{{left_x, bottom_y}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{right_x, bottom_y}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{right_x, top_y}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{left_x, top_y}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
		//// button
		//{{btn_left_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_right_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_right_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_left_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
	};

	dd.vertices = _vertices;
	dd.indices = indices;
	Init(dd);

	GLFWwindow *glfwWindow = Window::WindowController::GetInstance().GetWindow();

	glfwSetCursorPosCallback(glfwWindow, QuantaEditor::Input::cursor_position_callback);
	glfwSetMouseButtonCallback(glfwWindow, QuantaEditor::Input::mouse_button_callback);

	Core::Input::InputManager::GetInstance().AddCallback([this](const Core::Input::MouseEvent &e)
														 {
		debug_trace_coord = glm::vec2(e.x, e.y);
		drawn = false; });

	init_imgui();

	vulkanBackend.recordCallback = [this](VkCommandBuffer cmd)
	{
		this->OnRecordCommandBuffer(cmd);
	};

	drawn = false;
	useRaytracer = false;
	item_current = 2;
	samples_per_pixel = 6;
	max_bounces = 3;
	accumulate = false;
}

void Editor::RenderLoop(double t, double fpsLimit)
{
	m_Timer.Reset();

	// PROCESS INPUT
	double newTime = glfwGetTime();
	double frameTime = newTime - m_LastFrameTime;
	m_LastFrameTime = newTime;

	float deltaTime = std::min(frameTime, fpsLimit);

	/* m_gameController->ProcessInput(deltaTime);*/
	// m_gameController->Update(deltaTime);

	// Utilities::FPSCounter::CalculateFrameRate();
	// Utilities::FPSCounter::CalculateFrameRate();

	// RENDER
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*      m_gameController->Render();*/

	// glfwSwapBuffers(WindowController::GetInstance().GetWindow());

	glfwPollEvents();
	int width = vulkanBackend.GetRenderContext().Width;
	int height = vulkanBackend.GetRenderContext().Height;

	std::vector<Utilities::DebugLine> d_lines;

	Core::Input::InputManager::GetInstance().ProcessEvents();

	// if (!drawn)
	//{
	if (!m_Image || width != m_Image->GetWidth() || height != m_Image->GetHeight())
	{
		std::cout << "[dimensions x: " << width << " y: " << height << std::endl;

		m_Image = std::make_shared<Image>(width, height, ImageFormat::RGBA, vulkanBackend);
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
		delete[] m_AccumulationData;
		m_AccumulationData = new glm::vec4[width * height];

		renderer->UpdateImageDimensions(width, height);
	}

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, static_cast<size_t>(m_Image->GetWidth()) * static_cast<size_t>(m_Image->GetHeight()) * sizeof(glm::vec4));

	glm::vec2 mouse = Core::Input::InputManager::GetInstance().GetMousePosition();

	// std::thread::hardwareconcurrency();

	renderer->SetRenderSettings(samples_per_pixel, max_bounces, accumulate);

	if (useRaytracer)
	{
		renderer->Render(reinterpret_cast<float *>(m_AccumulationData), m_ImageData, m_FrameIndex);
	} // useRaytracer

	// drawn = true;
	// std::cout << "image generated " << std::endl;
	// std::cout << " First pixel: " << std::hex << m_ImageData[0] << std::endl;

	m_Image->SetData(vulkanBackend, m_ImageData);

	d_lines = renderer->GetDebugLines();
	if (debug)
	{
		vulkanBackend.updateDebugBuffer(d_lines);
	}

	glm::vec2 size = Window::WindowController::GetInstance().GetSize();
	// editor->CalculateLayout(size.x, size.y);
	/*DrawData drawData = editor->RenderEditor();*/

	RenderUI();

	//----------------------------------------------------------------------------

	vulkanBackend.drawFrame(m_DrawData.indices, d_lines.size());

	m_FrameIndex++;

	m_LastRenderTime = m_Timer.ElapsedMillis();
	//}
}

void Editor::RenderUI()
{
	// imgui new frame
	ImGui_ImplVulkan_NewFrame();
	// ImGui_ImplSDL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//----------------------------------------------------------------------------
	// ImGui UI Instructions
	ImGuiH::Panel::Begin();

	ImGui::SeparatorText("RENDER SETTINGS");
	ImGui::DragInt("Samples per Pixel 0..50", &samples_per_pixel, 1, 1, 50, "%d%", ImGuiSliderFlags_AlwaysClamp);
	ImGui::DragInt("Max Bounces 0..30", &max_bounces, 1, 1, 30, "%d%", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Checkbox("Accumulate", &accumulate);

	ImGui::SeparatorText("DEBUG SETTINGS");
	bool changed = false;

	ImGui::Combo("Debug Mode", &item_current, items, IM_ARRAYSIZE(items));

	ImGui::SeparatorText("SCENE SETTINGS");

	ImGui::Text("Objects");
	for (size_t i = 0; i < scene.ray_targets.size(); ++i)
	{
		ImGui::PushID(i);
		std::shared_ptr<Scene::Shapes::RayTarget> val = scene.ray_targets[i];
		auto sphere = std::dynamic_pointer_cast<Scene::Shapes::Sphere>(val);
		if (sphere)
		{
			ImGui::Text("Sphere \n");
			float *_origin_ptr = &sphere.get()->Origin.x;
			ImGui::DragFloat3("Origin", _origin_ptr, 0.1f);
			ImGui::DragFloat("Radius", &sphere.get()->Radius, 1.0f, 0.01f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

			if (ImGui::BeginCombo("Material", sphere.get()->Material ? sphere.get()->Material->GetName().c_str() : "None"))
			{
				for (size_t i = 0; i < scene.materials.size(); ++i)
				{
					bool isSelected = (scene.materials[i] == sphere.get()->Material);
					if (ImGui::Selectable(scene.materials[i]->GetName().c_str(), isSelected))
					{
						sphere.get()->Material = scene.materials[i];
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::PopID();
	}

	ImGui::Text("Materials");

	for (size_t i = 0; i < scene.materials.size(); i++)
	{
		ImGui::PushID(i);

		std::shared_ptr<Materials::Material> material = scene.materials[i];

		ImGui::Text(material.get()->GetName().c_str());

		float *color_ptr = &material.get()->Albedo.x;
		ImGui::ColorEdit3("Color", color_ptr);

		auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(material);

		if (emissive)
		{
			ImGui::DragFloat("Emission Strength", &emissive.get()->EmissionStrength, 0.05f, 0.0f, 1.0f);
			float *emission_color_ptr = &emissive.get()->EmissionColor.x;
			ImGui::ColorEdit3("Emission Color", emission_color_ptr);
		}
		else
		{

			auto metal = std::dynamic_pointer_cast<Materials::Metal>(material);
			if (metal)
				ImGui::DragFloat("Fuzz Amt", &metal.get()->fuzz, 0.05f, 0.0f, 1.0f);
		}

		// ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		// ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);

		ImGui::Separator();
		ImGui::PopID();
	}

	ImGui::Text("Last Render: %.3fms", m_LastRenderTime);

	changed |= ImGui::Checkbox("Trace rays", &useRaytracer);

	if (ImGui::Button("Save scene"))
		sceneManager.SaveScene(scene);

	ImGuiH::Panel::End();
}

void Editor::OnRecordCommandBuffer(VkCommandBuffer cmd)
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

// std::vector<int> Editor::CalculateLayout(int width, int height)
//{
//	dimensions.emplace_back(width);
//	dimensions.emplace_back(height);
//
//	return dimensions;
// }
//
// DrawData Editor::RenderEditor()
//{
//	DrawData dd;
//
//	// float vp_w = width * 0.2f;
//	// float vp_h = height * 0.2f;
//
//	float left_x = -1.0;
//	float right_x = 1.0;
//	// float right_x = 1.0;
//	float top_y = 1.0;
//	float bottom_y = -1.0;
//
//	float btn_left_x = 1.0 * 0.55f;
//	float btn_right_x = 0.9;
//	float btn_top_y = 0.85f;
//	float btn_bottom_y = 0.80f;
//
//	const std::vector<Vertex> _vertices = {
//		// Viewport
//		{{left_x, bottom_y}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
//		{{right_x, bottom_y}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
//		{{right_x, top_y}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
//		{{left_x, top_y}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
//		//// button
//		//{{btn_left_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//		//{{btn_right_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//		//{{btn_right_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//		//{{btn_left_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//	};
//
//	dd.vertices = _vertices;
//	dd.indices = indices;
//
//	glm::vec2 mouse = Input::InputManager::GetInstance().GetMousePosition();
//
//	// ((coords_screen_space / width_or_height_of_screen) * 2) - 1
//	// ndc.y = 1.0 - ((2.0 * screen.y) - (2.0 * y)) / h)
//
//	float ndc_x = ((mouse.x / dimensions[0]) * 2) - 1;
//	float ndc_y = -(((mouse.y / dimensions[1]) * 2) - 1);
//
//	// std::cout << "ndc_x " << ndc_x << " ndc_y " << ndc_y << std::endl;
//
//	if (btn_left_x < ndc_x && ndc_x < btn_right_x && ndc_y < btn_top_y && ndc_y > btn_bottom_y)
//	{
//		// std::cout << "BUTTON CLICKED" << std::endl;
//	}
//
//	return dd;
// }

void Editor::init_imgui()
{
	// 1: create descriptor pool for IMGUI
	//  the size of the pool is very oversize, but it's copied from imgui demo
	//  itself.
	VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
										 {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
										 {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
										 {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	VkResult err = vkCreateDescriptorPool(vulkanBackend.GetDevice(), &pool_info, nullptr, &imguiPool);
	check_vk_result(err);

	// 2: initialize imgui library

	// this initializes the core structures of imgui
	ImGui::CreateContext();
	ImGuiH::setStyle(true);

	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Scale style sizes for high-DPI monitors
	ImGuiStyle &imgui_style = ImGui::GetStyle();
	imgui_style.ScaleAllSizes(ImGuiH::getDPIScale());

	// this initializes imgui for SDL
	// ImGui_ImplSDL2_InitForVulkan(_window);

	ImGui_ImplGlfw_InitForVulkan(Window::WindowController::GetInstance().GetWindow(), false);

	// this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = vulkanBackend.GetVkInstance();
	init_info.PhysicalDevice = vulkanBackend.GetPhysicalDevice();
	init_info.Device = vulkanBackend.GetDevice();
	init_info.Queue = vulkanBackend.GetGraphicsQueue();
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.RenderPass = vulkanBackend.GetRenderContext().RenderPass;

	// init_info.UseDynamicRendering = true;

	// VkPipelineRenderingCreateInfoKHR vkPipelineREnderingCreateInfoKHR = {};
	// vkPipelineREnderingCreateInfoKHR.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	// vkPipelineREnderingCreateInfoKHR.colorAttachmentCount = 1;
	// vkPipelineREnderingCreateInfoKHR.pColorAttachmentFormats = &swapChainImageFormat;

	//// dynamic rendering parameters for imgui to use
	// init_info.PipelineRenderingCreateInfo = vkPipelineREnderingCreateInfoKHR;
	// init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&init_info);

	ImGui_ImplVulkan_CreateFontsTexture();

	vkDeviceWaitIdle(vulkanBackend.GetDevice());
	// ImGui_ImplVulkan_DestroyFontUploadObjects();
}
