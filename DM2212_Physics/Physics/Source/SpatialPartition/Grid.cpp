#include "Grid.h"

Grid::Grid(unsigned int width, unsigned int height, unsigned int cellSize)
	: m_width(width),
	m_height(height),
	m_cellSize(cellSize)
{
	m_numXCells = (int)ceil((float)m_width / m_cellSize);
	m_numYCells = (int)ceil((float)m_height / m_cellSize);
	std::cout << " X cells: " << m_numXCells << ", Y cells: " << m_numYCells << std::endl;
	// Allocate cells
	const int RESERVE = 20;
	m_cells.resize(m_numXCells * m_numYCells);
	for (int i = 0; i < m_cells.size(); i++)
	{
		m_cells[i].m_goLists.reserve(RESERVE);
	}
}

Grid::~Grid()
{
}

void Grid::AddGameObject(GameObject* go)
{
	Cell* cell = GetCell(go->pos);
	cell->m_goLists.push_back(go);
	go->currentCell = cell;
	go->cellVectorIndex = cell->m_goLists.size() - 1;
}

void Grid::AddGameObject(GameObject* go, Cell* cell)
{
	cell->m_goLists.push_back(go);
	go->currentCell = cell;
	go->cellVectorIndex = cell->m_goLists.size() - 1;
}

void Grid::UpdateGrid(GameObject* go)
{
	Cell* newCell = GetCell(go->pos);
	if (newCell != go->currentCell)
	{
		RemoveGameObject(go);
		AddGameObject(go, newCell);
	}
}

Cell* Grid::GetCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::GetCell(const Vector3& pos)
{
	int cellX = (int)pos.x / m_cellSize;
	int cellY = (int)pos.y / m_cellSize;
	std::cout << "Cell X: " << cellX << " , Cell y: " << cellY << std::endl;
	return GetCell(cellX, cellY);
}

void Grid::RemoveGameObject(GameObject* go)
{
	// Just for readability cus this shit is painful for the eyes
	std::vector<GameObject*>& gameObjects = go->currentCell->m_goLists;
	// Vector swap
	gameObjects[go->cellVectorIndex] = gameObjects.back();
	gameObjects.pop_back();

	if(go->cellVectorIndex < gameObjects.size())
		gameObjects[go->cellVectorIndex]->cellVectorIndex = go->cellVectorIndex;

	go->cellVectorIndex = -1;
	go->currentCell = nullptr;
	// Update vector index

}
