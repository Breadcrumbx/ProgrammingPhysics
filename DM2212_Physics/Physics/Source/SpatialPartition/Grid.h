#ifndef GRID_H
#define GRID_H

#include "../GameObject.h"
#include <vector>

struct Cell
{
	std::vector<GameObject*> m_goLists;
};

class Grid
{
	friend class SceneSimulation;
public:
	Grid(unsigned int width,unsigned int height, unsigned int cellSize);
	~Grid();

	// Adds a gameobject and determines which cell it belongs to
	void AddGameObject(GameObject* go);

	// Adds a gameobject to a specific cell
	void AddGameObject(GameObject* go, Cell* cell);
	
	// Updates the new position of gameobject in grid
	void UpdateGrid(GameObject* go);
	// Get the cell based on cell coordinates
	Cell* GetCell(int x, int y);

	// Get the cell based on world coordinates
	Cell* GetCell(const Vector3& pos);

	void RemoveGameObject(GameObject* go);
private:
	unsigned int m_width, m_height, m_cellSize, m_numXCells, m_numYCells;
	std::vector<Cell> m_cells;
};

#endif

