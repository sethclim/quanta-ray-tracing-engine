#include "Editor.hpp"
#include "MapleUI/maple.hpp"

Editor::Editor() : maple(new Maple()){ }

std::vector<int> Editor::CalculateLayout(int width, int height)
{
	dimensions.emplace_back(width - 240);
	dimensions.emplace_back(height - 240);

	return dimensions;
}

std::vector<int> Editor::RenderEditor()
{
	std::vector<int> yo;
	yo.emplace_back(200);
	return yo;
}
