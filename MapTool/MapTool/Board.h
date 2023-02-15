#pragma once

#include "CSprite.h"

#include <vector>

class CMouse;

class Board
{
	std::vector<std::vector<CSprite>*>* m_pVecBoardTile;
	std::vector<std::vector<CSprite>*>* m_pVecBoardObject;
	std::vector<std::vector<CSprite>*>* m_pVecBoardObject;
	std::vector<std::vector<CSprite>*>* m_pVecBoardObject;

	int m_gridX = 0, m_gridY = 0;

public:
	void RenderBoard(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush);
	void RenderObject(ID2D1RenderTarget* _pRenderTarget);

	void PutSprite(int _xpos, int _ypos, CMouse* _mouse);
	void SetBoard(int _gridX, int _gridY);
	void DestroyBoard();
};