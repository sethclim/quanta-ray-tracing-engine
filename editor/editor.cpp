#include "Editor.hpp"
#include "MapleUI/maple.hpp"

Editor::Editor() : maple(new Maple())
{
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
	float right_x = 1.0 * 0.5f;
	float top_y = 1.0;
	float bottom_y = -1.0;

	const std::vector<Vertex> _vertices = {
		//Viewport
		{{left_x, bottom_y}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{right_x, bottom_y}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{right_x, top_y}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{left_x, top_y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// button
		{{1.0 * 0.55f, 0.80f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{0.9, 0.80f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{0.9, 0.85f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{1.0 * 0.55f, 0.85f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
	};

	dd.vertices = _vertices;
	dd.indices = indices;

	glm::vec2 mouse = Input::InputManager::GetInstance().GetMousePosition();

	return dd;
}
