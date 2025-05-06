#include "Editor.hpp"
#include "MapleUI/maple.hpp"

Editor::Editor() : maple(new Maple())
{


	std::cout << "SETUP IMGUI" << std::endl;
}

std::vector<int> Editor::CalculateLayout(int width, int height)
{
	dimensions.emplace_back(width);
	dimensions.emplace_back(height);

	return dimensions;
}

DrawData Editor::RenderEditor()
{
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
		{{left_x, bottom_y}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{right_x, bottom_y}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{right_x, top_y}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{left_x, top_y}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		//// button
		//{{btn_left_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_right_x, btn_bottom_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_right_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		//{{btn_left_x, btn_top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
	};

	dd.vertices = _vertices;
	dd.indices = indices;

	glm::vec2 mouse = Input::InputManager::GetInstance().GetMousePosition();

	// ((coords_screen_space / width_or_height_of_screen) * 2) - 1
	// ndc.y = 1.0 - ((2.0 * screen.y) - (2.0 * y)) / h)

	float ndc_x = ((mouse.x / dimensions[0]) * 2) - 1;
	float ndc_y = -(((mouse.y / dimensions[1]) * 2) - 1);

	// std::cout << "ndc_x " << ndc_x << " ndc_y " << ndc_y << std::endl;

	if (btn_left_x < ndc_x && ndc_x < btn_right_x && ndc_y < btn_top_y && ndc_y > btn_bottom_y)
	{
		// std::cout << "BUTTON CLICKED" << std::endl;
	}

	return dd;
}
