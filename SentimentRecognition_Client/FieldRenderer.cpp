#include "Rect.h"
#include <iostream>
#include <vector>

using std::vector;

class FieldRenderer
{
private:
	Rect*** m_Tiles;
	const int m_xTiles, m_yTiles;

public:
	int** matrix;
	bool staticDrawed = false;

	FieldRenderer(int xTiles, int yTiles) : m_xTiles(xTiles), m_yTiles(yTiles)
	{
		matrix = new int* [xTiles];
		m_Tiles = new Rect * *[xTiles];
		for (int i = 0; i < xTiles; i++)
		{
			matrix[i] = new int[yTiles];
			m_Tiles[i] = new Rect * [yTiles];
			for (int j = 0; j < yTiles; j++)
			{
				m_Tiles[i][j] = new Rect(i, j);
				matrix[i][j] = 0;
			}
		}
	}

	void PrintMatrix()
	{
		for (int i = 0; i < m_xTiles; i++)
		{
			for (int j = 0; j < m_yTiles; j++)
				std::cout << matrix[i][j] << " ";
			std::cout << std::endl;
		}
	}

	void UpdateField()
	{
		for (int i = 0; i < m_xTiles; i++)
			for (int j = 0; j < m_yTiles; j++)
			{
				if (m_Tiles[i][j]->MouseIsInRect())
					m_Tiles[i][j]->SetFilled();
				else
					m_Tiles[i][j]->SetEmpty();
			}
	}

	void UpdateFieldStatic()
	{
		staticDrawed = true;
		for (int i = 0; i < m_xTiles; i++)
			for (int j = 0; j < m_yTiles; j++)
			{
				if (m_Tiles[i][j]->MouseIsInRect())
				{
					matrix[j][i] = 1;
					m_Tiles[i][j]->SetFilled();
				}
			}
	}

	void ClearField()
	{
		staticDrawed = false;
		for (int i = 0; i < m_xTiles; i++)
			for (int j = 0; j < m_yTiles; j++)
			{
				matrix[j][i] = 0;
				m_Tiles[i][j]->SetEmpty();
			}
	}

	void Render()
	{
		for (int i = 0; i < m_xTiles; i++)
			for (int j = 0; j < m_yTiles; j++)
				m_Tiles[i][j]->Render();
	}

	vector<double> GetMatrixVector()
	{
		vector<double> vec;
		for (int i = 0; i < m_xTiles; i++)
			for (int j = 0; j < m_yTiles; j++)
				vec.push_back(matrix[i][j]);
		return vec;
	}


};