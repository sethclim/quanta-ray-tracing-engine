#include "Editor.hpp"
#include "MapleUI/maple.hpp"

Editor::Editor() : maple(new Maple())
{

}

std::vector<int> Editor::CalculateLayout(int width, int height)
{
	dimensions.emplace_back(width - 240);
	dimensions.emplace_back(height - 240);

	return dimensions;
}

DrawData Editor::RenderEditor()
{
	DrawData dd;

	dd.vertices = vertices;
	dd.indices = indices;


	return dd;
}
